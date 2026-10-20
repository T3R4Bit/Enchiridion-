# Design Review — Enchiridion v1 Blueprint

A critical review of the original blueprint (`project_blueprint.md`), worst-first.
Severity: 🔴 critical (breaks the board as drawn), 🟠 major (needs rework before
it works), 🟡 minor/polish.

The corrected GPIO map that resolves these is at the bottom and is reflected in
`firmware/main/board_pins.h`.

## 🔴 Critical

1. **No 3.3 V regulator in the BOM.** An 18650 swings 4.2 V (full) → ~3.0 V
   (empty). 4.2 V exceeds the ESP32-S3's 3.6 V absolute max, so a full cell can
   damage the MCU, and a low cell starves the radios. An LDO can't regulate once
   the cell falls below ~3.5 V.
   **Fix:** buck-boost regulator (e.g. TPS63020/TPS63060) rated ~1.5–2 A peak
   to cover WiFi (~500 mA) + LoRa TX bursts.

2. **USB-A *female* is the wrong connector for BadUSB / USB-Ethernet.** Those
   require the ESP32 to be the USB *device* and the PC the *host*; a female-A
   receptacle makes the ESP32 the host.
   **Fix:** USB-C data port (connect to target via a C-to-A cable), or a USB-A
   *male* plug. Can't do device-mode gadget and USB-host on one PHY.

3. **Only one USB PHY (GPIO19/20), and it was spent on the target port — no
   programming/debug port.**
   **Fix:** make USB-C data+power+flash (single connector), and/or break out
   UART0 (GPIO43/44) + EN/BOOT pads.

4. **125 kHz LF RFID has no pins budgeted and is hand-waved as "GPIO/PWM".**
   LF reading needs a tuned LC tank, a carrier driver, and a demod chain
   (amp/filter/comparator) — not a bit-banged GPIO.
   **Fix:** defer LF RFID out of v1 (recommended), or design a proper analog
   front end and budget pins for it.

5. **ADC1 is fully consumed, so there's no clean battery-sense pin.** ADC1 is
   GPIO1–10 only (all used); ADC2 returns busy while WiFi is active. No
   battery/charge sensing or status-LED pins were allocated despite the diagram.
   **Fix:** use an I2C fuel gauge (MAX17048) on the existing bus — no ADC/GPIO
   needed — and drive status from the TFT.

## 🟠 Major

6. **USB-C "power only" needs CC pull-downs.** Without 5.1 kΩ on CC1/CC2 to GND,
   a USB-C source won't supply 5 V (especially C-to-C).

7. **"Embedded Tailscale (MicroLink)" is likely infeasible on an ESP32.** Plan on
   **WireGuard** (esp_wireguard) instead. Native USB is full-speed (12 Mbps), so
   tunnel throughput is capped around ~8 Mbps regardless.

8. **SX1262 RF-switch control (TXEN/RXEN) unallocated.** EBYTE E22 modules need
   these driven. Also pick TX power deliberately — an E22-30S (1 W) can pull
   ~0.6–1 A on TX and wrecks the power budget; a 22 dBm part (~110 mA) is saner.

9. **PN532 IRQ and RSTPDN pins missing**, and I2C pull-ups (4.7 kΩ) unspecified.

10. **Shared-SPI contention** between the display and LoRa can delay DIO1
    servicing and hurt Meshtastic timing; SD cards can hold MISO.
    **Fix:** series R/buffer on SD MISO, chunked/DMA display writes, or put the
    display on the second SPI host.

11. **No ESD/TVS on USB data lines** that plug into arbitrary PCs. Add a
    low-capacitance USB TVS array on D+/D-.

## 🟡 Minor / polish

- **GPIO3 (D-pad Left) is a strapping pin** — moved to GPIO42 in the corrected map.
- **No backlight PWM pin** → no dimming; added TFT_BL on GPIO39.
- **IR TX on a bare GPIO has weak range** — drive the LED through a transistor.
- **No SD card-detect** (minor UX).
- **RF coexistence / antenna placement:** NFC, 2.4 GHz, and sub-GHz on one small
  board near a steel 18650 holder — budget antenna keep-outs early; verify with a
  NanoVNA.
- **Thermal/safety:** don't charge a Li-ion in a sealed shell without ventilation;
  verify the DW01A/FS8205A protection.
- **Scope:** WiFi + Meshtastic + NFC + IR + USB gadget + VPN in one firmware is a
  large integration — phase it (see `roadmap.md`).

## Corrected GPIO map (rev B)

| Subsystem | Signal | GPIO | Notes |
| :--- | :--- | :--- | :--- |
| Native USB | D- / D+ | 19 / 20 | fixed by silicon; to USB-C data |
| SPI (shared) | SCK / MOSI / MISO | 12 / 11 / 13 | series R on SD MISO |
| LoRa SX1262 | CS / RST / BUSY / DIO1 | 10 / 9 / 14 / 21 | |
| LoRa SX1262 | TXEN / RXEN | 47 / 48 | **added** — RF switch |
| Display ST7789 | CS / DC / RST | 15 / 16 / 17 | |
| Display ST7789 | BL (PWM) | 39 | **added** — dimming |
| MicroSD | CS | 18 | |
| I2C (shared) | SDA / SCL | 4 / 5 | 4.7k pull-ups; PN532 + MAX17048 |
| PN532 | IRQ / RST | 40 / 41 | **added** |
| D-pad | Up/Down/Left/Right/Center | 1 / 2 / 42 / 6 / 7 | Left moved off strap GPIO3 |
| Infrared | TX / RX | 8 / 38 | TX via transistor |
| Battery | (fuel gauge) | — | MAX17048 on I2C, no ADC |

Reserved: GPIO0 (BOOT/button), GPIO3 (freed strap/test pad), GPIO43/44 (UART0
console), GPIO45/46 (strap), GPIO26–37 (flash + octal PSRAM).

Not GPIO but required: buck-boost 3V3 reg, USB-C CC pull-downs, USB TVS.
