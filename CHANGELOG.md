# Changelog

All notable changes to IoT Bees are documented here.
Format follows [Keep a Changelog](https://keepachangelog.com/).

## [0.1.0] — in development (not yet built/released)

First scaffolding of the IoT Bees distribution. **Status: code complete for the
items below, but not yet built into an image or tested on hardware.**

### Added
- **`meta-iotbees` Yocto layer** — built on Yocto **Scarthgap 5.0 LTS**.
- **Distro definition** — the OS identifies as **"IoT Bees"** (`DISTRO = iotbees`),
  systemd-based, with a per-image license manifest.
- **Image recipe** (`iotbees-image`) targeting Raspberry Pi (ARM64).
- **`iotbees-agent` v0.1 (C)** — reads a Modbus TCP register and publishes
  telemetry **and** a device-health heartbeat to MQTT (libmodbus + Paho MQTT C),
  runs as a systemd service.
- **`iotbees-setup`** — terminal onboarding wizard: configure the device source
  and the cloud MQTT broker, then it writes the config and restarts the agent.
- **Bundled packages:** openssh, libmodbus (Modbus), bluez5 (BLE), paho-mqtt-c (MQTT).
- **Docs:** `README.md`, `BUILD.md`, `CONTRIBUTING.md`, `ROADMAP.md`, MIT `LICENSE`.

### Known limitations (v0.1)
- Not yet built into a bootable image or tested on hardware.
- Single Modbus TCP source / single register (multi-device planned).
- No BLE, LoRa, or cellular support yet (on the roadmap).
- No remote web UI / 6-digit pairing yet.
- No OTA update mechanism or TLS to the broker yet.

### Next
- **Step 2:** build `iotbees-image` with Yocto and boot it on a Raspberry Pi.
- See `ROADMAP.md` for the full plan.

---

_Releases will be tagged here (e.g. `v0.1.0`) once an image is built, tested, and
published to GitHub Releases._
