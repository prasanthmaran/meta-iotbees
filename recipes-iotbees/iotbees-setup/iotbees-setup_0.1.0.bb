SUMMARY = "IoT Bees terminal onboarding wizard"
DESCRIPTION = "Interactive setup: configures the gateway (device + cloud broker) \
and writes /etc/iotbees/agent.conf."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://iotbees-setup"

S = "${WORKDIR}"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/iotbees-setup ${D}${bindir}/iotbees-setup
}
