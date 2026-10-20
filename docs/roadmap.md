# Roadmap — From Blueprint to Working Device

What it takes to make Enchiridion real. Phased so each step de-risks the next.
Severity and detail of the design issues referenced here are in
`design-review.md`.

## Phase 0 — Fix the design on paper (days, ~$0)

Can't order a board until the 🔴 critical issues are resolved, because they
change the part count and pinout.

- [ ] Add buck-boost 3.3 V regulator (TPS63020-class) — **blocker**.
- [ ] Settle USB strategy: USB-C for charge + data + flash (recommended).
- [ ] Add USB-C CC pull-downs, USB TVS, I2C fuel gauge (MAX17048), PN532
      IRQ/RST, LoRa TXEN/RXEN, backlight PWM.
- [ ] Decide LF 125 kHz in/out of v1 (recommend: out).
- [x] Corrected GPIO map — see `design-review.md` and `firmware/main/board_pins.h`.

## Phase 1 — Breadboard proof (1–2 weeks, ~$60–120)

Prove every subsystem on dev hardware before committing to a PCB.

- [ ] ESP32-S3-DevKitC (N16R8) + breakouts: SX1262 (E22), PN532, ST7789,
      microSD, IR LED/receiver, buck-boost module, TP4056 module, 18650 holder.
- [ ] Bring up one at a time: display → SD → I2C NFC → SPI LoRa → IR → USB.
- [ ] Confirm octal PSRAM init and USB device enumeration.

## Phase 2 — Schematic + PCB in KiCad 8 (2–4 weeks incl. fab)

- [ ] Schematic from the corrected design.
- [ ] 2-layer layout, all SMD top-side; 50 Ω USB + RF traces, antenna keep-outs,
      18650 holder away from antennas.
- [ ] Order 3–5 assembled boards (JLCPCB/PCBWay). Expect a rev B.

## Phase 3 — Bring-up + enclosure (ongoing)

- [ ] Verify power rails with a meter **before** connecting a cell.
- [ ] Re-run Phase 1 tests on the real board.
- [ ] Fusion 360 enclosure from the KiCad STEP export; print PETG; iterate fit.

## Firmware plan

- Driver per subsystem behind `board_pins.h`, then a menu UI on the TFT.
- Networking: **WireGuard**, not embedded Tailscale (see design-review #7).
- Meshtastic: decide integrate-with vs. talk-to a Meshtastic network.
- Build incrementally; don't boot the whole feature set at once.

## Tools you'll need

- **Required:** multimeter, fine-tip iron + flux + hot-air/hotplate, the Phase 1
  breakouts.
- **Recommended:** USB-C power meter, cheap logic analyzer (~$15).
- **For RF:** NanoVNA (~$50) to tune/verify antennas.
- **Software:** ESP-IDF v5.x, KiCad 8, Fusion 360 (free personal license).

## Rough budget / timeline

| Milestone | Cost | Time |
| :--- | :--- | :--- |
| Breadboard prototype | ~$60–120 | 1–2 weeks |
| First real PCBs | ~$100–250 | ~1 month |
| Polished v1 | + iterations | 2–3 board revs / a few months |

## Non-negotiables

- **Li-ion:** confirm the 3.3 V rail before connecting a cell; ventilate while
  charging; verify the DW01A/FS8205A protection.
- **RF:** choose the sub-GHz band and TX power for your region's rules and the
  right E22 variant; respect duty-cycle limits.
- **Use:** only on systems and networks you own or are authorized to test.
