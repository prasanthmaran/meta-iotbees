<!-- Replace prasanthmaran with your real GitHub username throughout. -->

# 🐝 IoT Bees

**An open-source, Yocto-based Linux distribution for IoT gateways.**
Terminal-first. Built to talk to industrial and IoT devices out of the box —
Modbus, BLE, and MQTT — and push their data to a server you configure.

> Status: early development (v0.x). Building in public. Contributions welcome.

---

## Why IoT Bees?

Most IoT deployments stitch together a generic Linux, a pile of protocol
libraries, and custom glue scripts on every gateway. IoT Bees bundles that into
one reproducible, security-patchable Linux image so a gateway boots ready to
collect field data and ship it upstream.

- **Field protocols:** Modbus TCP, Modbus RTU (RS-485), BLE — with LoRa and
  cellular planned.
- **Transports out:** MQTT by default; HTTP and others pluggable.
- **Terminal-first:** full setup from the console. Optional remote web UI via a
  pairing URL + short-lived 6-digit code (planned).
- **Built on Yocto LTS (Scarthgap 5.0):** long-term security support, a real
  license manifest, reproducible builds.

## Supported hardware

| Target | Status |
|--------|--------|
| Raspberry Pi 4 / 5 (ARM64) | Primary target (v1) |
| x86_64 industrial gateways (ADLINK, Neousys, etc.) | Planned (v2) |

**Hardware notes:** Modbus RTU needs an RS-485 adapter; BLE on x86 gateways
needs a USB Bluetooth dongle; LoRa/cellular (v2) require their respective radio
modules.

## Quick start (flash a prebuilt image)

```bash
# 1. Download the latest image (replace with the real release URL)
wget https://github.com/prasanthmaran/meta-iotbees/releases/download/v0.1.0/iotbees-image-rpi4-0.1.0.wic.bz2

# 2. Write it to an SD card (find your card with: lsblk — wrong device wipes data!)
bzcat iotbees-image-rpi4-0.1.0.wic.bz2 | sudo dd of=/dev/sdX bs=4M conv=fsync status=progress

# 3. Insert the SD card into the Pi and boot. Log in at the terminal.
```

> Note: `wget` downloads the image — it does not install the OS by itself. The
> image must be written to an SD card and booted, just like Raspberry Pi OS.

## Build it yourself

See **BUILD.md** (or the IoT Bees Build Guide) for the full Yocto setup. Short
version:

```bash
git clone -b scarthgap git://git.yoctoproject.org/poky.git
git clone -b scarthgap git://git.openembedded.org/meta-openembedded.git
git clone -b scarthgap git://git.yoctoproject.org/meta-raspberrypi.git
git clone https://github.com/prasanthmaran/meta-iotbees.git

source poky/oe-init-build-env
bitbake-layers add-layer ../meta-openembedded/meta-oe
bitbake-layers add-layer ../meta-openembedded/meta-python
bitbake-layers add-layer ../meta-openembedded/meta-networking
bitbake-layers add-layer ../meta-raspberrypi
bitbake-layers add-layer ../meta-iotbees
# set DISTRO = "iotbees" and MACHINE = "raspberrypi4-64" in conf/local.conf
bitbake iotbees-image
```

## What's in the image (the install list)

The set of protocol libraries and tools is defined **at build time** in the
image recipe (`recipes-core/images/iotbees-image.bb`) via `IMAGE_INSTALL` — so
every gateway gets the same, reproducible software set. Current list:

- `libmodbus` / modbus tools — Modbus TCP & RTU
- `bluez5` — BLE
- an MQTT client — telemetry transport
- the IoT Bees agent — collects field data, pushes to the configured server
- `openssh` — remote access

## Roadmap

- [x] Bootable Raspberry Pi image identifying as IoT Bees
- [ ] Modbus + BLE recipes
- [ ] Core agent (collect → internal model → MQTT/HTTP out)
- [ ] Terminal onboarding (server URL, credentials)
- [ ] Remote UI pairing (URL + short-lived 6-digit code)
- [ ] OTA updates + image hardening
- [ ] x86 gateway target (ADLINK / Neousys)
- [ ] LoRa + cellular support

## Contributing

Bug reports and pull requests are welcome — this project is open for everyone to
improve. Open an Issue for bugs/ideas, or submit a PR. (A `CONTRIBUTING.md` will
follow.)

## License

IoT Bees' own code is released under the **MIT License** (see `LICENSE`).
Upstream components included in the built image (the Linux kernel, glibc,
BusyBox, systemd, etc.) retain their own licenses — see the per-image
`license.manifest` Yocto generates.

---

Created and maintained by **Prasanth** ([@prasanthmaran](https://github.com/prasanthmaran)).
