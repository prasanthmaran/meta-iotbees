# Testing IoT Bees without hardware (QEMU)

You do **not** need a Raspberry Pi to test the OS, the agent, and the
Modbus→MQTT data loop. Yocto can build for a virtual machine and boot it in an
emulator on your PC — free, safe, and reversible. **Do not format your Windows
PC; everything here runs in a window.**

> Note: the Pi image is ARM and won't run on your x86 PC. For emulation we build
> a separate **x86-64 virtual** image (`qemux86-64`). It tests the distro + agent,
> not Pi-specific hardware (real GPIO/serial/BLE) — that comes later on a real Pi.

Do all of this inside your **Ubuntu (WSL2)** terminal, after the build setup in
`BUILD.md` (steps A–E).

---

## 1. Build a virtual (QEMU) image

In `build/conf/local.conf`, set the machine to the emulator target:
```
MACHINE = "qemux86-64"
DISTRO  = "iotbees"
```
Then build:
```bash
bitbake iotbees-image
```

## 2. Boot it in the emulator
```bash
runqemu qemux86-64
```
A window opens and IoT Bees boots. Log in at the prompt (default user `root`,
no password on a dev image).

### Level 1 — smoke test (does it boot as IoT Bees?)
Inside the booted guest:
```bash
cat /etc/os-release          # should show NAME="IoT Bees"
systemctl status iotbees-agent
```
If you see `NAME="IoT Bees"` — the build and OS work. 🎉

---

## 3. Test the full data loop (Modbus → MQTT) — still no hardware

You'll run a fake Modbus device and an MQTT broker **on the host** (your Ubuntu),
and the agent inside QEMU will read one and publish to the other.

### 3a. Install host helpers (Ubuntu)
```bash
sudo apt install -y mosquitto mosquitto-clients
# a free Modbus TCP simulator:
sudo apt install -y diagslave   ||   pip3 install pymodbus --break-system-packages
```

### 3b. Start a fake Modbus device (host, terminal 1)
```bash
diagslave -m tcp -p 502
# (or, with pymodbus, run a simple TCP server on port 502)
```

### 3c. Start the MQTT broker + watch messages (host, terminal 2)
```bash
sudo systemctl start mosquitto
mosquitto_sub -t 'iotbees/#' -v        # leave this running to watch data
```

### 3d. Point the agent at the host (inside QEMU guest)
With default `runqemu` networking, the host is reachable at **192.168.7.1**.
Run the wizard:
```bash
sudo iotbees-setup
```
Set:
- Modbus host/IP → `192.168.7.1`
- Modbus port → `502`
- MQTT broker URL → `tcp://192.168.7.1:1883`

(If your `runqemu` uses slirp networking instead, the host is `10.0.2.2`.)

### 3e. Watch it work
Back in the host's `mosquitto_sub` window, you should see messages arriving:
```
iotbees/data   {"device":"gateway-001","ts":...,"register":0,"value":...}
iotbees/health {"device":"gateway-001","ts":...,"status":"online","modbus":"ok"}
```
That's the **entire pipeline proven end-to-end, with zero hardware.** 🐝

---

## 4. Shut down
- Close the QEMU window, or inside the guest: `poweroff`
- Stop host helpers: `Ctrl+C` the simulator; `sudo systemctl stop mosquitto`

---

## What this proves vs. what still needs a Pi
- ✅ Proven here: the image builds, IoT Bees boots, the agent runs, and the
  Modbus→MQTT data + health loop works.
- ⬜ Still needs a real Pi later: Pi BSP/boot, real serial/RS-485 Modbus, real
  BLE, GPIO, and flashing to an SD card.

## If the build or boot errors
Copy the exact error text and send it over — first-build errors (often a missing
recipe like `paho-mqtt-c`) are normal and fixable.
