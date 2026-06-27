SUMMARY = "IoT Bees backup, restore & disk space guard"
DESCRIPTION = "iotbees-backup/iotbees-restore save and restore config + local data; \
iotbees-spaceguard compresses and prunes old data when the disk runs low (timer-driven)."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://iotbees-backup \
           file://iotbees-restore \
           file://iotbees-spaceguard \
           file://iotbees-spaceguard.service \
           file://iotbees-spaceguard.timer"

S = "${WORKDIR}"

inherit systemd
SYSTEMD_SERVICE:${PN} = "iotbees-spaceguard.timer"
SYSTEMD_AUTO_ENABLE = "enable"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/iotbees-backup     ${D}${bindir}/iotbees-backup
    install -m 0755 ${WORKDIR}/iotbees-restore    ${D}${bindir}/iotbees-restore
    install -m 0755 ${WORKDIR}/iotbees-spaceguard ${D}${bindir}/iotbees-spaceguard

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/iotbees-spaceguard.service ${D}${systemd_system_unitdir}/iotbees-spaceguard.service
    install -m 0644 ${WORKDIR}/iotbees-spaceguard.timer   ${D}${systemd_system_unitdir}/iotbees-spaceguard.timer
}

FILES:${PN} += "${systemd_system_unitdir}/iotbees-spaceguard.service \
                ${systemd_system_unitdir}/iotbees-spaceguard.timer"
