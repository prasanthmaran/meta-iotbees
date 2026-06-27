# Partner outreach email — TEMPLATE (send AFTER you have a working demo)

> Do NOT send these yet. Reaching out before you can show a working gateway +
> dashboard demo gets ignored. Send once the MVP demo video and a live repo with
> a tagged release exist. Then this opens doors.
>
> Personalize the bracketed parts. Keep it short — busy people skim.

---

## Generic version (Zoho IoT / any MQTT platform)

**Subject:** Open-source IoT gateway OS — adding a [Platform] connector

Hi [Team / name],

I'm Prasanth, creator of **IoT Bees** — an open-source (MIT) Linux distribution
for IoT gateways. It flashes onto hardware like a Raspberry Pi, reads industrial
devices (Modbus, BLE, …), and pushes data to a cloud over MQTT.

I'm adding first-class connectors for the major IoT platforms, and I'd like
**[Platform]** to be one of them, so users can point an IoT Bees gateway at
[Platform] in a couple of clicks.

A quick demo of it pushing live data to a dashboard: [demo video link]
Project: https://github.com/prasanthmaran/meta-iotbees

Two small things would help:
1. A pointer to your current device/MQTT onboarding docs (endpoint, auth, topics).
2. Whether you'd be open to listing IoT Bees as a supported gateway / integration.

Happy to share more. The project is open source and vendor-neutral by design —
the goal is to make it easy for your users to get devices connected.

Thanks,
Prasanth
[email] · [LinkedIn]

---

## Notes per platform
- **Zoho IoT** — ask for their device/MQTT onboarding docs; mention multi-protocol
  (they support MQTT, OPC-UA, Modbus, BACnet). zoho.com/iot
- **AWS IoT Core / Azure IoT Hub** — usually no email needed; just use their public
  docs + a free account. Email only matters for partner/marketplace listing.
- **ThingsBoard / EMQX / HiveMQ** — open communities; a GitHub issue/forum post or
  Discord works better than email for technical questions.

## Reminder
The strongest "outreach" is a great demo + a public repo people can try. Build that
first; the emails land far better afterward.
