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
| 6 | Terminal onboarding: set cloud domain + credentials | ✅ (v0.1) |
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

## Reliability & data safety (field-grade)

These make IoT Bees trustworthy on a remote gateway that may lose power or
connectivity. **All planned.**

- ⬜ **Offline support / store-and-forward** — when the cloud is unreachable, the
  agent keeps reading sensors and **buffers data locally**, then auto-syncs in
  order once the connection returns. No data lost during outages.
- ⬜ **Local sensor-data storage** — readings persisted on-device (e.g. SQLite or
  a size-capped ring buffer) with configurable retention, so data survives
  reboots and brief outages independent of the cloud.
- ⬜ **Easy data export / recovery** — if the OS has an issue, the locally stored
  sensor data can be **exported in one command** (CSV/JSON) to USB, SD, or over
  the network (`iotbees-export`), so data is recoverable even from a degraded
  device.
- ✅ (v0.1) **Quick backup & restore** — `iotbees-backup` saves config + local
  data to a single archive; `iotbees-restore` restores it onto a re-flashed or
  replacement gateway, so swapping hardware is fast and lossless.
- ✅ (v0.1) **Emergency disk-space guard** — `iotbees-spaceguard` (timer-driven)
  watches disk usage and, when it crosses a threshold, **compresses old data
  first, then prunes the oldest archives** until there's room — so a full disk
  can't take the gateway down.
- ⬜ **Resilient boot** — read-only root + writable data partition and atomic OTA
  with rollback, so a bad update or power cut can't brick the gateway.

## Security (strong, non-negotiable)

IoT gateways sit on customer networks — security is a first-class requirement,
not an afterthought. **All planned.**

- ⬜ **No default passwords** — forced credential setup on first boot.
- ⬜ **Secure boot + signed images** — only signed IoT Bees images run; signed OTA
  updates only.
- ⬜ **Encrypted data at rest** — local sensor buffer and credentials encrypted on
  the device.
- ⬜ **TLS everywhere** — encrypted MQTT/HTTPS to the cloud; certificate-based
  device identity.
- ⬜ **Least privilege** — the agent runs unprivileged; minimal installed packages
  to shrink attack surface.
- ⬜ **Timely patching** — track upstream CVEs via the Yocto LTS and ship security
  updates over OTA.
- ⬜ **Audit + firewall defaults** — sane firewall rules and logging out of the box.

## Device identity & connectivity fabric

So every gateway is uniquely identifiable and reachable from anywhere — even
behind NAT/firewalls — without exposing it to the public internet. **All planned.**
Build on proven components; do **not** write a VPN from scratch.

- ⬜ **Unique device identity (bound to hardware)** — each gateway's ID is derived
  from a stable hardware identifier: the **MAC address** (Ethernet/Wi-Fi) or, on
  cellular gateways, the modem **IMEI** (and SIM **IMSI**). This binds identity to
  the physical device.
  - Caveat: MAC/IMEI can be *read* and spoofed, so they are used as the human-
    readable identity, **paired with a per-device certificate** (ideally in a
    secure element / TPM) for the cryptographic identity that actually
    authenticates the device. Hardware ID = the name; certificate = the proof.
- ⬜ **Overlay mesh network (WireGuard)** — every device joins a private virtual
  network (via WireGuard — e.g. Tailscale / NetBird / ZeroTier) and gets a stable
  private IP, so any gateway is reachable by identity from anywhere, securely.
- ⬜ **Reverse-tunnel + rendezvous** (alt/complement) — device dials out to a
  coordination server; remote access works through outbound connections only.
  Pairs with the remote URL + short-lived 6-digit login (Step 7).
- ⬜ **MQTT control channel** — per-device command topic for lightweight remote
  control, reusing the existing broker.
- ⬜ **Control plane** — a coordination service (self-hosted or a managed tier)
  that brokers connections and tracks which devices are online. Runs in the
  cloud; **not** part of the OS image.
- ⬜ **Enrollment & revocation** — secure onboarding of new devices and the
  ability to instantly revoke a lost/compromised gateway's access.

> Note: like the cloud backend, the control plane is a companion service, not
> part of the flashable image. The device ships with the identity + WireGuard
> client; the coordinator lives in the cloud.

## Agent roadmap (the `iotbees-agent`)

- 🟡 v0.1 — single Modbus TCP register → MQTT data + health heartbeat (C, libmodbus, Paho).
- ⬜ Config-driven multi-device, multiple registers, register maps / device templates.
- ⬜ BLE source via BlueZ.
- ⬜ Local buffering + store-and-forward (offline support).
- ⬜ Local data store + `iotbees-export` (CSV/JSON) and `iotbees-backup`.
- ⬜ Pluggable output transports (HTTP, etc.) behind one internal data model.
- ⬜ TLS + encrypted credentials to the broker.

## Tech stack (locked)

- **Distribution build:** Yocto Project, Scarthgap 5.0 LTS
- **First target:** Raspberry Pi (ARM64); x86 gateways (ADLINK / Neousys) = target #2
- **Agent language:** C/C++ (native, lean) — links libmodbus + Paho MQTT C directly
- **Default transport:** MQTT
- **License:** MIT (IoT Bees' own code; upstream components keep their licenses)

## Notes

- The `paho-mqtt-c` and `libmodbus` recipes come from `meta-openembedded`
  (already added in the build guide).
- v0.1 of the agent is a skeleton meant to prove the Modbus→MQTT loop end to end,
  not a finished product. Reliability and security items above are the path from
  "works on a bench" to "trustworthy in the field."
