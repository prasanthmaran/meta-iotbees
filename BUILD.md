# Building IoT Bees

IoT Bees is built with the **Yocto Project (Scarthgap 5.0 LTS)**. You need a
native Linux x86_64 host (Ubuntu 22.04/24.04), ~100 GB free disk, and 8 GB+ RAM.
On Windows, use **WSL2 + Ubuntu** and build inside the Linux home directory
(never under `/mnt/c`). macOS cannot build Yocto natively — use a Linux VM/host.

## 1. Host dependencies (Ubuntu)
```bash
sudo apt update
sudo apt install -y gawk wget git diffstat unzip texinfo gcc build-essential \
  chrpath socat cpio python3 python3-pip python3-pexpect xz-utils debianutils \
  iputils-ping python3-git python3-jinja2 python3-subunit zstd liblz4-tool file locales
sudo locale-gen en_US.UTF-8
```

## 2. Get the sources (Scarthgap branch)
```bash
mkdir -p ~/iotbees && cd ~/iotbees
git clone -b scarthgap git://git.yoctoproject.org/poky.git
git clone -b scarthgap git://git.openembedded.org/meta-openembedded.git
git clone -b scarthgap git://git.yoctoproject.org/meta-raspberrypi.git
git clone https://github.com/prasanthmaran/meta-iotbees.git
```

## 3. Init the build env and add layers
```bash
cd ~/iotbees/poky
source oe-init-build-env
bitbake-layers add-layer ../../meta-openembedded/meta-oe
bitbake-layers add-layer ../../meta-openembedded/meta-python
bitbake-layers add-layer ../../meta-openembedded/meta-networking
bitbake-layers add-layer ../../meta-raspberrypi
bitbake-layers add-layer ../../meta-iotbees
bitbake-layers show-layers   # confirm meta-iotbees is listed
```

## 4. Configure the target
In `build/conf/local.conf`:
```
MACHINE = "raspberrypi4-64"
DISTRO  = "iotbees"
```

## 5. Build the image
```bash
bitbake iotbees-image
```
Output: `tmp/deploy/images/raspberrypi4-64/iotbees-image-*.wic.bz2`

## 6. Flash to an SD card
Replace `/dev/sdX` with your real device (check `lsblk` — wrong device wipes data):
```bash
bzcat tmp/deploy/images/raspberrypi4-64/iotbees-image-*.wic.bz2 \
  | sudo dd of=/dev/sdX bs=4M conv=fsync status=progress
```

## 7. First boot
Insert the SD card, power the Pi, log in at the terminal, then run:
```bash
sudo iotbees-setup
```
to point the gateway at your device and your cloud broker. `cat /etc/os-release`
should show `NAME="IoT Bees"`.

## Notes
- `libmodbus` and `paho-mqtt-c` come from `meta-openembedded`. If a recipe is
  missing in your layer set, add it before building.
- See `ROADMAP.md` for what's built vs planned.
