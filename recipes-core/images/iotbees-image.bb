SUMMARY = "IoT Bees base image - terminal-first IoT gateway OS"
LICENSE = "MIT"

require recipes-core/images/core-image-minimal.bb

# Field protocols + transport + remote access baked into every image.
# Steps 4-5 of the build guide flesh this out (add the iotbees-agent recipe).
IMAGE_INSTALL:append = " \
    openssh \
    libmodbus \
    bluez5 \
    "
