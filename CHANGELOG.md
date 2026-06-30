# Changelog

All notable changes to IoT Bees are documented here.
Format follows [Keep a Changelog](https://keepachangelog.com/).

## [0.1.0] — BUILT & BOOTS ✅ (2026-06-30)

**Milestone: the first IoT Bees image builds with Yocto and boots successfully.**
Verified by building `iotbees-image` for `qemux86-64` (Yocto Scarthgap 5.0 LTS)
and booting it in QEMU — it reaches a login prompt as **"IoT Bees 0.1.0"** and the
`iotbees-agent` starts automatically as a systemd service.

### Verified in the built image
- OS identifies as **IoT Bees 0.1.0** (`/etc/os-release`: `NAME="IoT Bees"`).
- Boots via systemd to a login prompt.
- `iotbees-agent` service starts on boot ("IoT Bees Agent (field protocols -> MQTT)").
- Installed: iotbees-agent, iotbees-setup, iotbees-backup, iotbees-export,
  libmodbus5 3.1.10 (Modbus), paho-mqtt-c 1.3.13 (MQTT), bluez5 5.72 (BLE), openssh.

### Added
- `meta-iotbees` Yocto layer (Scarthgap 5.0 LTS) + "IoT Bees" distro definition.
- `iotbees-image` recipe (qemux86-64 verified; Raspberry Pi target next).
- `iotbees-agent` (C): Modbus TCP -> MQTT telemetry + device health (CPU temp,
  memory, disk, uptime, load), token auth, adaptive sample rate under disk pressure.
- `iotbees-setup` terminal onboarding wizard.
- `iotbees-backup` / `iotbees-restore` / `iotbees-export` and `iotbees-spaceguard`
  (compress/prune old data when disk is low, timer-driven).
- Docs: README, BUILD, TEST-QEMU, ROADMAP, CONTRIBUTING, MVP, docs/CONNECTORS.

### Build fixes applied (Ubuntu 24.04 host)
- Enable unprivileged user namespaces (`kernel.apparmor_restrict_unprivileged_userns=0`).
- Add `usrmerge` to DISTRO_FEATURES so systemd provides udev (required by bluez5).
- Pass `LDFLAGS` at link time in the agent Makefile (fixes GNU_HASH do_package_qa).

### Known limitations (0.1.0)
- Single Modbus TCP source / single register (multi-device planned).
- No BLE/LoRa/cellular agent integration yet; no remote UI; no OTA; no TLS.
- Raspberry Pi image not yet built (qemux86-64 verified; Pi is a re-target).

### Next
- Tag `v0.1.0`, publish the image to GitHub Releases.
- Build the Raspberry Pi target; wire BLE; add TLS + the connector framework.
