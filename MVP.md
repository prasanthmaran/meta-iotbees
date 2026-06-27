# IoT Bees — MVP & Product Scope

> Purpose: stop scope creep. This defines the smallest thing worth shipping, the
> one metric that proves it, and exactly what is IN and OUT. Everything not in the
> MVP is "later" — no exceptions until the MVP ships.

---

## MVP in one sentence
**An IoT Bees image that boots on a Raspberry Pi (or QEMU), reads a Modbus device,
pushes the data + device health to a cloud (ThingsBoard), and is configured from
the terminal.**

## The one success metric
> A stranger can flash IoT Bees, connect a Modbus device, and **see their data in
> a cloud dashboard in under 30 minutes** — without help.

If that works, the MVP is real. If it doesn't, nothing else matters yet.

---

## IN scope (MVP)
- Builds with Yocto and **boots** (QEMU first, then Raspberry Pi).
- Identifies as IoT Bees (`/etc/os-release`).
- **Modbus TCP** read via the agent.
- **MQTT push** to a cloud (default target: **ThingsBoard**, free/open).
- **Device health** (temp/mem/disk/uptime) on a separate channel.
- **Terminal onboarding** (`iotbees-setup`) to set device + cloud.
- Token auth on the MQTT connection.
- Docs: README, BUILD, TEST-QEMU.

## OUT of scope (post-MVP — deferred on purpose)
- Modbus RTU, BLE, LoRa, cellular (need hardware).
- Plugin/driver framework refactor (do after MVP proves the loop).
- Cloud connectors beyond generic MQTT (AWS, Azure, Zoho) — generic MQTT only for MVP.
- Remote web UI + 6-digit pairing.
- WireGuard identity/connectivity fabric.
- OTA updates, secure boot, encryption at rest (security hardening = fast-follow).
- x86 gateways (ADLINK/Neousys).
- Offline buffering polish, backup/export polish (basic versions exist; don't gold-plate).

> Note: most of these are already scaffolded in the repo. "Out of scope" means
> **don't keep building them** until the MVP boots and demos.

---

## The demo (what you record once it works) — ~2 minutes
1. Flash / boot IoT Bees (QEMU or Pi). Show `/etc/os-release` → "IoT Bees".
2. Run `sudo iotbees-setup` → point at a Modbus source + a ThingsBoard broker.
3. Show a Modbus value changing.
4. Cut to the **ThingsBoard dashboard** showing the live value + device health.
5. Tagline overlay: *"Flash one image. Connect any device. Your data, your cloud. Open source."*

---

## Pitch one-pager

**What:** IoT Bees — an open-source, Yocto-based Linux distribution for IoT
gateways. Flash it, connect industrial devices (Modbus, BLE…), push data to *your*
cloud. MIT-licensed, no vendor lock-in.

**The hook:** *"Flash one image, connect any industrial device, see your data in
your own cloud in 10 minutes — open source, no vendor lock-in."*

**Who it's for:** system integrators, hardware OEMs, and factories/utilities with
mixed equipment who don't want to be locked into a single vendor's cloud.

**Why now:** IoT gateways are either expensive proprietary boxes or DIY scripts.
IoT Bees is a reproducible, secure, open middle path.

**Business model (open core):** the OS + agent are free/MIT. Revenue later from
support, certified hardware, and an optional managed fleet/cloud service.

---

## Path after MVP
1. Ship MVP + record demo.
2. Get **3–5 pilot users** (r/embedded, r/selfhosted, LinkedIn) and listen.
3. Security hardening (fast-follow).
4. Then breadth: more protocols + cloud connectors, by real demand.
