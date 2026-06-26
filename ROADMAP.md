# IoT Bees — Roadmap

> Status legend: ✅ done · 🟡 in progress · ⬜ planned

IoT Bees is an open-source, Yocto-based Linux **distribution** for IoT gateways.
Field protocols come IN (Modbus, BLE, …), the **agent** normalizes them, and data
goes OUT to a configured cloud over MQTT/HTTPS — with device data and health.

---

## Architecture (end to end)

```
Field devices  ->  IoT Bees gateway  ->  your cloud
(Modbus, BLE,      (agent: collect ->     (MQTT broker / platform:
 LoRa, cellular)    normalize -> push)     device data + health)
```

- **On the device (part of the distribution):** OS image, protocol drivers,
  the `iotbees-agent`, terminal setup, remote-UI pairing.
- **In the cloud (separate, not part of the distro):** a broker/platform that
  receives, stores, and displays the data (e.g. ThingsBoard, EMQX, Node-RED, or
  a custom backend). The gateway pushes to it; it is not shipped in the image.

---

## Build & platform roadmap

| # | Milestone | Status |
|---|-----------|--------|
| 1 | Foundation: Yocto (Scarthgap 5.0 LTS), Raspberry Pi target, MIT, MQTT default | ✅ |
| 2 | Yocto build environment → minimal bootable Pi image | ⬜ |
| 3 | `meta-iotbees` layer (distro, image recipe, license) | ✅ |
| 4 | Protocol stacks as recipes (libmodbus, bluez5) | 🟡 |
| 5 | Core agent: Modbus → MQTT + device health (C, v0.1) | 🟡 |
| 6 | Terminal onboarding: set cloud domain + credentials | ⬜ |
| 7 | Remote UI pairing: remote URL + short-lived 6-digit code | ⬜ |
| 8 | Image hardening + OTA updates + security review | ⬜ |
| 9 | Release pipeline: versioned images + install docs | ⬜ |

## Protocol roadmap

| Protocol | Layer | Hardware needed | Status |
|----------|-------|-----------------|--------|
| Modbus TCP | field (in) | none | 🟡 (agent v0.1) |
| Modbus RTU | field (in) | RS-485 adapter | ⬜ |
| BLE | field (in) | Bluetooth radio (built-in on Pi) | ⬜ |
| LoRa / LoRaWAN | field (in) | SX127x / SX130x radio | ⬜ (v2) |
| Cellular (NB-IoT/LTE-M/4G) | field (in) | modem + SIM | ⬜ (v2) |
| MQTT | transport (out) | none | 🟡 (agent v0.1) |
| HTTP/HTTPS | transport (out) | none | ⬜ |
| CoAP / AMQP | transport (out) | none | ⬜ |

## Agent roadmap (the `iotbees-agent`)

- 🟡 v0.1 — single Modbus TCP register → MQTT data + health heartbeat (C, libmodbus, Paho).
- ⬜ Config-driven multi-device, multiple registers, register maps / device templates.
- ⬜ BLE source via BlueZ.
- ⬜ Local buffering / store-and-forward when the cloud is unreachable.
- ⬜ Pluggable output transports (HTTP, etc.) behind one internal data model.
- ⬜ Secure credentials, TLS to the broker.

## Tech stack (locked)

- **Distribution build:** Yocto Project, Scarthgap 5.0 LTS
- **First target:** Raspberry Pi (ARM64); x86 gateways (ADLINK / Neousys) = target #2
- **Agent language:** C/C++ (native, lean) — links libmodbus + Paho MQTT C directly
- **Default transport:** MQTT
- **License:** MIT (IoT Bees' own code; upstream components keep their licenses)

## Notes

- The `paho-mqtt-c` and `libmodbus` recipes come from `meta-openembedded`
  (already added in the build guide). If `paho-mqtt-c` is unavailable in your
  layer set, add the recipe before building `iotbees-agent`.
- v0.1 of the agent is a skeleton meant to prove the Modbus→MQTT loop end to end,
  not a finished product.
