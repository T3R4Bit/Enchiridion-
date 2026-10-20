/*
 * board_pins.h — Enchiridion hardware pin map (ESP32-S3-WROOM-1-N16R8)
 *
 * CORRECTED map (rev B). Supersedes the blueprint's §4 table, which had
 * conflicts and omissions — see docs/design-review.md for the full rationale.
 *
 * Hard constraints on this module:
 *   - GPIO19/20 are hardware-dedicated to native USB D-/D+.
 *   - GPIO26..37 are consumed by the SPI flash + OCTAL PSRAM. DO NOT USE.
 *   - Strapping pins (0, 3, 45, 46) are avoided for anything boot-sensitive.
 *   - ADC1 is GPIO1..10 only; ADC2 is unusable while WiFi is active, so
 *     battery monitoring is done with an I2C fuel gauge, not the ADC.
 *   - UART0 (GPIO43/44) is reserved for the debug console.
 *
 * Changes vs. blueprint:
 *   - D-pad LEFT moved off strapping GPIO3 -> GPIO42.
 *   - Added PN532 IRQ/RST, LoRa RF-switch (TXEN/RXEN), TFT backlight PWM.
 *   - Battery sensing via MAX17048 fuel gauge on the existing I2C bus
 *     (no GPIO/ADC pin needed).
 *   - USB target port is USB-C (data+power+flash); the blueprint's USB-A
 *     female receptacle is dropped (wrong USB role for BadUSB/gadget mode).
 *   - 125 kHz LF RFID deferred out of v1 (needs an analog front end; not
 *     budgeted here).
 */
#ifndef ENCHIRIDION_BOARD_PINS_H
#define ENCHIRIDION_BOARD_PINS_H

/* ---- Native USB (fixed by silicon) ---- */
#define PIN_USB_DM        19   /* USB D-  (to USB-C data) */
#define PIN_USB_DP        20   /* USB D+  (to USB-C data) */

/* ---- Shared SPI bus (display, SX1262 LoRa, SD card) ---- */
#define PIN_SPI_SCK       12
#define PIN_SPI_MOSI      11
#define PIN_SPI_MISO      13   /* add series R / buffer on SD side */

/* ---- SX1262 LoRa (EBYTE E22) ---- */
#define PIN_LORA_CS       10
#define PIN_LORA_RST       9
#define PIN_LORA_BUSY     14
#define PIN_LORA_DIO1     21
#define PIN_LORA_TXEN     47   /* RF switch TX enable */
#define PIN_LORA_RXEN     48   /* RF switch RX enable */

/* ---- ST7789 display ---- */
#define PIN_TFT_CS        15
#define PIN_TFT_DC        16
#define PIN_TFT_RST       17
#define PIN_TFT_BL        39   /* backlight, PWM (LEDC) for dimming */

/* ---- MicroSD card ---- */
#define PIN_SD_CS         18

/* ---- Shared I2C bus (PN532 NFC + MAX17048 fuel gauge) ---- */
#define PIN_I2C_SDA        4   /* 4.7k pull-up to 3V3 */
#define PIN_I2C_SCL        5   /* 4.7k pull-up to 3V3 */
#define PIN_NFC_IRQ       40   /* PN532 IRQ */
#define PIN_NFC_RST       41   /* PN532 RSTPDN */

/* ---- D-pad navigation (active low, internal pull-ups) ---- */
#define PIN_BTN_UP         1
#define PIN_BTN_DOWN       2
#define PIN_BTN_LEFT      42   /* moved off strapping GPIO3 */
#define PIN_BTN_RIGHT      6
#define PIN_BTN_CENTER     7

/* ---- Infrared ---- */
#define PIN_IR_TX          8   /* drive LED via transistor, not direct */
#define PIN_IR_RX         38

/*
 * Reserved / do not reassign:
 *   GPIO0        - BOOT strap / programming button
 *   GPIO3        - strap (freed from LEFT button; leave as test pad)
 *   GPIO43/44    - UART0 TX/RX debug console
 *   GPIO45/46    - strap (45 = VDD_SPI; leave as-is)
 *   GPIO26..37   - flash + octal PSRAM
 *
 * Not a GPIO, but required in hardware (see design-review.md):
 *   - Buck-boost 3V3 regulator (e.g. TPS63020) between cell and rail.
 *   - USB-C CC1/CC2 5.1k pull-downs.
 *   - USB TVS array on D+/D-.
 *   - Status: use the TFT; optional WS2812 on an expansion pad if wanted.
 */

#endif /* ENCHIRIDION_BOARD_PINS_H */
