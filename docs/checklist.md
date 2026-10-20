# Build Checklist — Everything Needed for a Working Physical v1

Ordered so each step unblocks the next. Check items off as you go.
Background and rationale live in `design-review.md` and `roadmap.md`.

---

## 0. Decisions to lock first (do these before spending money)

- [ ] Confirm **USB-C** as the single data + power + flash port (drop USB-A).
- [ ] Pick the **sub-GHz band** for your region (e.g. 915 MHz US / 868 MHz EU)
      and the **E22 variant / TX power** (22 dBm recommended, not 1 W).
- [ ] Decide **LF 125 kHz RFID: out of v1** (recommended) or in (needs analog
      front end).
- [ ] Choose **status indication**: TFT only, or add one WS2812.
- [ ] Set the **copyright name** in `LICENSE` (legal name vs. T3R4Bit).

## 1. Buy — breadboard prototype kit (~$60–120)

- [ ] ESP32-S3-DevKitC-1 **N16R8** (16 MB flash / 8 MB octal PSRAM)
- [ ] SX1262 LoRa module (EBYTE E22, correct band)
- [ ] PN532 NFC module
- [ ] ST7789 SPI TFT (1.3"–2.0")
- [ ] MicroSD breakout + a card
- [ ] IR LED + TSOP IR receiver + small NPN transistor
- [ ] Buck-boost 3.3 V module (TPS63020-based)
- [ ] TP4056 charger module + 18650 holder + one 18650 cell
- [ ] Antennas/pigtails for the LoRa band (+ NFC antenna on the PN532)
- [ ] Breadboard, jumpers, 4.7 kΩ (I2C) + assorted resistors

## 2. Buy / own — tools

- [ ] Multimeter *(required)*
- [ ] Fine-tip soldering iron + flux; hot-air or hotplate for SMD rework
- [ ] USB-C power meter *(recommended)*
- [ ] Cheap logic analyzer ~$15 *(recommended — saves hours on SPI/I2C)*
- [ ] NanoVNA ~$50 *(for RF antenna tuning, needed by Phase 4)*

## 3. Install software

- [ ] ESP-IDF v5.x toolchain
- [ ] KiCad 8
- [ ] Fusion 360 (free personal license)
- [ ] Clone this repo; `cd firmware && idf.py set-target esp32s3 && idf.py build`

## 4. Breadboard bring-up (prove it before a PCB)

- [ ] Power: verify the buck-boost outputs a stable 3.3 V **before** anything else
- [ ] Blink + serial log on the DevKit; confirm **octal PSRAM inits**
- [ ] ST7789 display driver
- [ ] MicroSD read/write over SPI (share bus with display)
- [ ] PN532 over I2C (read a tag)
- [ ] SX1262 LoRa: send/receive a packet (TXEN/RXEN switching correct)
- [ ] IR: capture a remote code and replay it
- [ ] USB: enumerate as a device on a PC (HID and/or network class)
- [ ] D-pad input + a basic TFT menu tying subsystems together

## 5. Schematic + PCB (KiCad 8)

- [ ] Schematic from the **corrected pin map** (`design-review.md`), including:
  - [ ] Buck-boost 3.3 V regulator
  - [ ] TP4056 + DW01A + FS8205A charge/protection
  - [ ] USB-C with **5.1 kΩ CC1/CC2 pull-downs**
  - [ ] USB **TVS array** on D+/D-
  - [ ] MAX17048 fuel gauge on I2C
  - [ ] I2C 4.7 kΩ pull-ups, PN532 IRQ/RST, LoRa TXEN/RXEN, TFT backlight PWM
  - [ ] IR TX transistor driver
- [ ] ERC clean
- [ ] 2-layer layout, all SMD top-side; SMD 18650 holder bottom (hand-soldered)
- [ ] 50 Ω USB diff pair; RF traces + antenna keep-outs; 18650 holder away from
      antennas
- [ ] DRC clean
- [ ] Export Gerbers + BOM + CPL (pick-and-place)
- [ ] Export **STEP** for the enclosure

## 6. Order boards

- [ ] Upload to JLCPCB / PCBWay with assembly; order **3–5 boards**
- [ ] Order the through-hole / hand-placed parts (18650 holder, connectors)

## 7. Board bring-up

- [ ] **Measure the 3.3 V rail with a meter before connecting a cell**
- [ ] Verify charging (current via USB-C meter) and protection cutoffs
- [ ] Re-run every Phase 4 test on the real board
- [ ] Fix issues → **rev B** (expect at least one revision)

## 8. Enclosure

- [ ] Model in Fusion 360 from the KiCad STEP export
- [ ] Cutouts for display, USB-C, SD, buttons, antennas
- [ ] Print PETG/ABS; check fit; iterate
- [ ] Ventilation so the cell/charger don't cook in a sealed shell

## 9. Firmware to "usable"

- [ ] Driver per subsystem behind `board_pins.h`
- [ ] Battery % from the fuel gauge shown on the TFT
- [ ] Menu UI / settings persisted to SD
- [ ] Networking via **WireGuard** (not embedded Tailscale)
- [ ] Meshtastic: integrate with, or talk to, a Meshtastic network

## 10. Before you call it "working"

- [ ] Runs from battery through a full charge/discharge without brownout
- [ ] All radios function at expected range (LoRa verified with NanoVNA-tuned
      antenna)
- [ ] Charges safely; protection verified
- [ ] Confirm sub-GHz band + TX power are legal for your region
- [ ] Only used on systems/networks you own or are authorized to test

---

### Critical-path summary
Decisions → buy kit → **prove 3.3 V rail** → breadboard every subsystem →
schematic/PCB → order → bring-up → enclosure → firmware. The 3.3 V regulator
and USB-C data port are the two things that must be right or nothing else works.
