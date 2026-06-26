SUMMARY = "IoT Bees agent - reads field protocols and publishes telemetry over MQTT"
DESCRIPTION = "v0.1 skeleton: reads a Modbus TCP register and publishes data + \
device health to an MQTT broker. Roadmap adds BLE, multi-device, and remote UI."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "libmodbus paho-mqtt-c"

SRC_URI = "file://iotbees-agent.c \
           file://Makefile \
           file://agent.conf \
           file://iotbees-agent.service"

S = "${WORKDIR}"

inherit systemd
SYSTEMD_SERVICE:${PN} = "iotbees-agent.service"
SYSTEMD_AUTO_ENABLE = "enable"

do_compile() {
    oe_runmake
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/iotbees-agent ${D}${bindir}/iotbees-agent

    install -d ${D}${sysconfdir}/iotbees
    install -m 0644 ${WORKDIR}/agent.conf ${D}${sysconfdir}/iotbees/agent.conf

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/iotbees-agent.service ${D}${systemd_system_unitdir}/iotbees-agent.service
}

FILES:${PN} += "${sysconfdir}/iotbees/agent.conf ${systemd_system_unitdir}/iotbees-agent.service"
