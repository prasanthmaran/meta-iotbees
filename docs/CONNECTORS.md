# IoT Bees — Cloud Connectors (reference & spec)

How IoT Bees connects to cloud / IoT platforms. Each connector is a **transport
plugin** in the agent. This file is the *spec* the connectors are built from —
exact endpoints and auth fields must be confirmed in each vendor's current docs
before implementing.

> Status: **planned**. The agent today does generic MQTT only. These connectors
> arrive with the pluggable transport framework + TLS (see ROADMAP.md). Nothing
> here is built/tested yet.

---

## Tier A — works (almost) now: generic MQTT + token

These speak standard MQTT with a username/password (often a device token). They
need little or no special code — point the broker URL + token and go (TLS pending).

| Platform | Transport | Auth | Free tier | Notes |
|----------|-----------|------|-----------|-------|
| **Mosquitto / EMQX / VerneMQ** | MQTT(S) | user/pass | open source | self-host; full control |
| **ThingsBoard** (Community) | MQTT(S) | device **access token** as MQTT username | free, open source (Apache-2.0) | topics `v1/devices/me/telemetry`; dashboards built in. **Best first target.** |
| **HiveMQ Cloud** | MQTT over TLS :8883 | user/pass | free tier | fully MQTT-compliant broker |
| **Zoho IoT** | MQTT / HTTPS (also OPC-UA, Modbus, BACnet, CoAP, LoRaWAN) | per Zoho device creds (verify in Zoho docs) | trial / paid | low-code platform; strong multi-protocol support |
| **Blynk** | MQTT (Blynk.Cloud) | auth token | free for exploration; paid from ~$99/mo | maker-friendly dashboards |
| **Datacake** | MQTT / HTTP | token | free dev tier | LPWAN/industrial dashboards |
| **TagoIO** | MQTT / HTTP | device token | free dev tier | strong visualization |
| **ThingSpeak** | MQTT (`mqtt3.thingspeak.com`) | channel API key | free (up to 13 channels) | simple, MATLAB analytics |
| **Adafruit IO / Ubidots** | MQTT / HTTP | key/token | free tiers (limited) | hobby/light industrial |

**Connector work for Tier A:** small — mostly a config profile (host, port, topic
template, credential field). These validate the plugin model.

---

## Tier B — needs a dedicated connector plugin (special auth/topics)

Still MQTT under the hood, but each has its own auth handshake and topic rules.

### AWS IoT Core
- Transport: MQTT over TLS, port **8883**.
- Auth: **mutual TLS with an X.509 device certificate** (per-device cert + private key + Amazon root CA).
- Endpoint: `xxxxx-ats.iot.<region>.amazonaws.com`.
- Topics: customizable; policies control allowed topics.
- Connector needs: cert/key provisioning, TLS mutual auth, topic mapping.

### Azure IoT Hub
- Transport: MQTT over TLS, port **8883**.
- Auth: **SAS token** (per-device key) or X.509.
- Endpoint: `<hub-name>.azure-devices.net`.
- Topics: fixed — telemetry to `devices/{deviceId}/messages/events/`.
- Connector needs: SAS token generation, Azure-specific topic format.

### Google Cloud
- Note: **Google retired Cloud IoT Core (Aug 2023).** Integration is now via a
  partner (e.g. ClearBlade IoT Core) or by bridging MQTT → **Pub/Sub**.
- Connector needs: depends on the chosen path; document before implementing.

### IBM Watson IoT / Alibaba Cloud IoT
- MQTT-based with vendor-specific auth (org/API keys, signed connections).
- Connector needs: vendor auth + topic conventions.

---

## How a connector plugin looks (planned)

Each connector implements the agent's transport interface:
- `connect()` — open the (TLS) session with the platform's auth.
- `publish(topic, payload)` — map the internal data model to the platform's topic/format.
- `subscribe(cmd_topic, cb)` — receive commands/config (the PULL path).

So adding a platform = one plugin + a config profile, without touching the core.

## Recommended order
1. **ThingsBoard** (free, open, plain MQTT) — proves the pipeline + dashboards.
2. **HiveMQ Cloud / EMQX** — generic MQTT over TLS.
3. **AWS IoT Core**, then **Azure IoT Hub** — the first two "real cloud" connectors.
4. **Zoho IoT** and others by demand.

> Reminder: implement these **after** the base image builds and the transport
> framework + TLS land. Until then this is the spec, not shipping code.
