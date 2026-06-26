# Contributing to IoT Bees

Thanks for your interest in IoT Bees 🐝 — an open-source Linux distribution for
IoT gateways. Contributions of all kinds are welcome: bug reports, hardware
testing, new protocol support, docs, and code.

## Ways to help
- **Report bugs** — open an Issue with what you did, what happened, your board,
  and the IoT Bees version.
- **Test on hardware** — try a build on a Raspberry Pi (or another board) and
  tell us what worked or broke.
- **Add protocols** — recipes/agent support for Modbus RTU, BLE, LoRa, etc.
- **Improve docs** — `README.md`, `BUILD.md`, `ROADMAP.md`.

## Development setup
See `BUILD.md` to set up the Yocto build environment and produce an image.
The IoT Bees layer is `meta-iotbees`; the agent lives in
`recipes-iotbees/iotbees-agent/`.

## Pull requests
1. Fork the repo and create a branch: `git checkout -b my-change`.
2. Keep changes focused; one logical change per PR.
3. For recipes, follow Yocto/OpenEmbedded style. For the agent (C), keep it
   warning-clean (`-Wall`).
4. Describe what you changed and how you tested it.
5. Open the PR against `main`.

## Code style
- **Yocto recipes:** standard BitBake conventions; pin `LAYERSERIES_COMPAT` to
  the supported release (`scarthgap`).
- **Agent (C/C++):** C standard library + libmodbus + Paho MQTT C; no unnecessary
  dependencies; compile cleanly with `-Wall -O2`.

## License
By contributing, you agree your contributions are licensed under the **MIT
License**, the same as the project. Upstream components included in built images
keep their own licenses.

## Code of conduct
Be respectful and constructive. We want IoT Bees to be a welcoming project.
