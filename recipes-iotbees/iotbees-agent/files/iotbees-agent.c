/*
 * IoT Bees agent - v0.2 skeleton
 * -------------------------------
 * - Reads a Modbus TCP holding register and publishes telemetry to MQTT.
 * - Publishes DEVICE HEALTH/METADATA (CPU temp, memory, disk, uptime, load)
 *   on a SEPARATE health topic, for remote monitoring.
 * - Supports TOKEN auth: an MQTT username + token (password) so the cloud
 *   broker only accepts authenticated pushes.
 *
 * Deps:    libmodbus, Eclipse Paho MQTT C (paho-mqtt3c)
 * License: MIT
 *
 * Early skeleton: single Modbus source/register. Roadmap (ROADMAP.md) covers the
 * pluggable multi-protocol driver framework, BLE, offline buffering, and more.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <modbus/modbus.h>
#include "MQTTClient.h"

#define CONF_DEFAULT "/etc/iotbees/agent.conf"
#define MAXLINE 256

typedef struct {
    char modbus_host[128];
    int  modbus_port;
    int  modbus_reg;
    int  modbus_count;
    char mqtt_broker[256];
    char mqtt_client_id[128];
    char mqtt_username[128];   /* token auth: username */
    char mqtt_token[256];      /* token auth: token used as MQTT password */
    char mqtt_topic_data[256];
    char mqtt_topic_health[256];
    char device_id[128];
    int  interval_sec;
} config_t;

static void set_defaults(config_t *c) {
    strcpy(c->modbus_host, "127.0.0.1");
    c->modbus_port  = 502;
    c->modbus_reg   = 0;
    c->modbus_count = 1;
    strcpy(c->mqtt_broker,       "tcp://localhost:1883");
    strcpy(c->mqtt_client_id,    "iotbees-gateway");
    c->mqtt_username[0] = 0;
    c->mqtt_token[0]    = 0;
    strcpy(c->mqtt_topic_data,   "iotbees/data");
    strcpy(c->mqtt_topic_health, "iotbees/health");
    strcpy(c->device_id,         "gateway-001");
    c->interval_sec = 5;
}

static void trim(char *s) {
    char *p = s + strlen(s) - 1;
    while (p >= s && (*p == '\n' || *p == '\r' || *p == ' ' || *p == '\t')) { *p = 0; p--; }
    while (*s == ' ' || *s == '\t') memmove(s, s + 1, strlen(s));
}

static void load_config(const char *path, config_t *c) {
    FILE *f = fopen(path, "r");
    if (!f) { fprintf(stderr, "[iotbees] config %s not found, using defaults\n", path); return; }
    char line[MAXLINE];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        char *eq = strchr(line, '=');
        if (!eq) continue;
        *eq = 0;
        char *key = line, *val = eq + 1;
        trim(key); trim(val);
        if      (!strcmp(key, "modbus_host"))       strncpy(c->modbus_host, val, sizeof(c->modbus_host) - 1);
        else if (!strcmp(key, "modbus_port"))       c->modbus_port  = atoi(val);
        else if (!strcmp(key, "modbus_reg"))        c->modbus_reg   = atoi(val);
        else if (!strcmp(key, "modbus_count"))      c->modbus_count = atoi(val);
        else if (!strcmp(key, "mqtt_broker"))       strncpy(c->mqtt_broker, val, sizeof(c->mqtt_broker) - 1);
        else if (!strcmp(key, "mqtt_client_id"))    strncpy(c->mqtt_client_id, val, sizeof(c->mqtt_client_id) - 1);
        else if (!strcmp(key, "mqtt_username"))     strncpy(c->mqtt_username, val, sizeof(c->mqtt_username) - 1);
        else if (!strcmp(key, "mqtt_token"))        strncpy(c->mqtt_token, val, sizeof(c->mqtt_token) - 1);
        else if (!strcmp(key, "mqtt_topic_data"))   strncpy(c->mqtt_topic_data, val, sizeof(c->mqtt_topic_data) - 1);
        else if (!strcmp(key, "mqtt_topic_health")) strncpy(c->mqtt_topic_health, val, sizeof(c->mqtt_topic_health) - 1);
        else if (!strcmp(key, "device_id"))         strncpy(c->device_id, val, sizeof(c->device_id) - 1);
        else if (!strcmp(key, "interval_sec"))      c->interval_sec = atoi(val);
    }
    fclose(f);
}

/* --- device health / metadata --- */
static double read_cpu_temp_c(void) {
    FILE *f = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (!f) return -1.0;
    long milli = -1;
    if (fscanf(f, "%ld", &milli) != 1) milli = -1;
    fclose(f);
    return (milli < 0) ? -1.0 : milli / 1000.0;
}
static int mem_used_pct(struct sysinfo *si) {
    if (si->totalram == 0) return -1;
    unsigned long used = si->totalram - si->freeram - si->bufferram;
    return (int)(100.0 * used / si->totalram);
}
static int disk_used_pct(void) {
    struct statvfs s;
    if (statvfs("/", &s) != 0 || s.f_blocks == 0) return -1;
    return (int)(100.0 * (s.f_blocks - s.f_bavail) / s.f_blocks);
}

static int mqtt_publish(MQTTClient client, const char *topic, const char *payload) {
    MQTTClient_message msg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    msg.payload    = (void *)payload;
    msg.payloadlen = (int)strlen(payload);
    msg.qos        = 1;
    msg.retained   = 0;
    int rc = MQTTClient_publishMessage(client, topic, &msg, &token);
    if (rc != MQTTCLIENT_SUCCESS) return rc;
    return MQTTClient_waitForCompletion(client, token, 2000L);
}

int main(int argc, char **argv) {
    const char *conf_path = (argc > 1) ? argv[1] : CONF_DEFAULT;
    config_t cfg;
    set_defaults(&cfg);
    load_config(conf_path, &cfg);

    printf("[iotbees] agent v0.2 starting; device=%s broker=%s auth=%s\n",
           cfg.device_id, cfg.mqtt_broker, cfg.mqtt_username[0] ? "token" : "none");

    MQTTClient client;
    MQTTClient_connectOptions co = MQTTClient_connectOptions_initializer;
    MQTTClient_create(&client, cfg.mqtt_broker, cfg.mqtt_client_id,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);
    co.keepAliveInterval = 20;
    co.cleansession      = 1;
    if (cfg.mqtt_username[0]) co.username = cfg.mqtt_username;   /* token auth */
    if (cfg.mqtt_token[0])    co.password = cfg.mqtt_token;
    if (MQTTClient_connect(client, &co) != MQTTCLIENT_SUCCESS) {
        fprintf(stderr, "[iotbees] MQTT connect failed to %s\n", cfg.mqtt_broker);
        return 1;
    }

    char payload[768];
    uint16_t regs[125];

    for (;;) {
        time_t now = time(NULL);
        int ok = 0, value = -1;

        /* --- Modbus read --- */
        modbus_t *mb = modbus_new_tcp(cfg.modbus_host, cfg.modbus_port);
        if (mb && modbus_connect(mb) != -1) {
            int n = modbus_read_registers(mb, cfg.modbus_reg, cfg.modbus_count, regs);
            if (n > 0) { value = regs[0]; ok = 1; }
        }
        if (mb) { modbus_close(mb); modbus_free(mb); }

        /* --- publish telemetry --- */
        if (ok) {
            snprintf(payload, sizeof(payload),
                     "{\"device\":\"%s\",\"ts\":%ld,\"register\":%d,\"value\":%d}",
                     cfg.device_id, (long)now, cfg.modbus_reg, value);
            mqtt_publish(client, cfg.mqtt_topic_data, payload);
            printf("[iotbees] data   -> %s %s\n", cfg.mqtt_topic_data, payload);
        }

        /* --- publish device health + metadata (separate channel) --- */
        struct sysinfo si;
        int have_si = (sysinfo(&si) == 0);
        double temp  = read_cpu_temp_c();
        int    mempc = have_si ? mem_used_pct(&si) : -1;
        int    diskpc = disk_used_pct();
        long   uptime = have_si ? si.uptime : -1;
        double load1 = have_si ? si.loads[0] / 65536.0 : -1.0;

        snprintf(payload, sizeof(payload),
                 "{\"device\":\"%s\",\"ts\":%ld,\"status\":\"online\",\"modbus\":\"%s\","
                 "\"cpu_temp_c\":%.1f,\"mem_used_pct\":%d,\"disk_used_pct\":%d,"
                 "\"uptime_s\":%ld,\"load1\":%.2f}",
                 cfg.device_id, (long)now, ok ? "ok" : "error",
                 temp, mempc, diskpc, uptime, load1);
        mqtt_publish(client, cfg.mqtt_topic_health, payload);
        printf("[iotbees] health -> %s %s\n", cfg.mqtt_topic_health, payload);

        sleep(cfg.interval_sec);
    }

    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    return 0;
}
