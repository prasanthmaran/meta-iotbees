SUMMARY = "IoT Bees base image - terminal-first IoT gateway OS"
LICENSE = "MIT"

require recipes-core/images/core-image-minimal.bb

# Field protocols + transport + the IoT Bees agent, setup wizard, and
# backup/restore tools, baked into every image.
IMAGE_INSTALL:append = " \
    openssh \
    libmodbus \
    bluez5 \
    paho-mqtt-c \
    iotbees-agent \
    iotbees-setup \
    iotbees-backup \
    "
