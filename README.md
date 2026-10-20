# Enchiridion

A custom, handheld hardware multi-tool and field node built around the
**ESP32-S3**. Inspired by devices like the Flipper Zero, it combines 2.4 GHz
Wi-Fi/Bluetooth analysis, Sub-GHz LoRa (Meshtastic) mesh communication,
NFC/RFID, infrared, BadUSB payload execution, and a USB-Ethernet Tailscale
bridge into a pocketable, 18650-powered package.

> **Enchiridion** — a handbook or manual small enough to be carried in the
> hand. The name fits a field tool meant to go wherever you do.

## Scope & intended use

This is a personal hardware/firmware project for **authorized security
research, education, amateur radio / Meshtastic, and electronics
tinkering**. Several of the radios and interfaces are dual-use. Operate the
device only on networks, systems, and RF bands you own or are explicitly
permitted to test, and in accordance with local RF regulations (e.g. FCC Part
15 / Part 97, CE). You are responsible for how you use it.

## Hardware at a glance

| Subsystem        | Part / module                     | Interface      |
| :--------------- | :-------------------------------- | :------------- |
| MCU              | ESP32-S3-WROOM-1-N16R8            | native         |
| LoRa (Sub-GHz)   | Semtech SX1262 (e.g. EBYTE E22)   | SPI            |
| NFC / HF RFID    | PN532                             | I²C / SPI      |
| LF RFID          | 125 kHz inductive coil driver     | GPIO / PWM     |
| Display          | ST7789 1.3"–2.0" color TFT        | SPI            |
| Storage          | MicroSD card slot                 | SPI            |
| Infrared         | IR LED + TSOP receiver            | GPIO           |
| Power            | 18650 Li-ion + TP4056/DW01A/FS8205A | power rail   |
| Data port        | USB-A female (OTG)                | native USB     |
| Charge port      | USB-C female (power only)         | —              |

Full bill of materials, rationale, and the GPIO pin-mapping schedule live in
[`docs/project_blueprint.md`](docs/project_blueprint.md).

## Repository layout

```
.
├── docs/         # Architecture blueprint, BOM, pin map, design notes
├── hardware/     # KiCad 8 schematic & 2-layer PCB project
├── firmware/     # ESP-IDF firmware (ESP32-S3)
│   └── main/     # Application entry point + board pin definitions
└── enclosure/    # Fusion 360 / exported STEP & 3D-print files
```

## Toolchain

- **PCB:** [KiCad 8](https://www.kicad.org/) — 2-layer, all SMD on the top
  layer for single-sided reflow; the 18650 holder is hand-soldered to the
  bottom after delivery.
- **Firmware:** [ESP-IDF](https://docs.espressif.com/projects/esp-idf/) v5.x.
- **Enclosure:** Fusion 360, fed by `.STEP` exports from KiCad for precise
  fitment inside a 3D-printed PETG/ABS shell.

## Building the firmware

```bash
cd firmware
idf.py set-target esp32s3
idf.py build
idf.py -p <PORT> flash monitor
```

The firmware is an early scaffold: `firmware/main/board_pins.h` captures the
GPIO assignments from the blueprint so peripheral drivers have a single source
of truth.

## Status

Early scaffolding. Schematic, PCB, firmware, and enclosure are works in
progress — see `docs/project_blueprint.md` for the target architecture.

## License

Enchiridion is **source-available**, dual-licensed:

- **Noncommercial use is free** under the
  [PolyForm Noncommercial License 1.0.0](LICENSE) — personal, hobby, research,
  educational, and noncommercial-organization use.
- **Commercial use requires a separate paid license** from the copyright
  holder. See [`LICENSING.md`](LICENSING.md) for how to arrange one.

Note: this is *source-available*, not "open source" in the strict OSI sense,
because it restricts commercial use.
