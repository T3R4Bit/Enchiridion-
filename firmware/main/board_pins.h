/*
 * board_pins.h — Enchiridion hardware pin map (ESP32-S3-WROOM-1-N16R8)
 *
 * Single source of truth for GPIO assignments, mirroring the schedule in
 * docs/project_blueprint.md (§4). Peripheral drivers should include this
 * header rather than hard-coding pin numbers.
 *
 * NOTE: GPIO 19/20 are hardware-dedicated to native USB D-/D+ and must not
 * be repurposed.
 */
#ifndef ENCHIRIDION_BOARD_PINS_H
#define ENCHIRIDION_BOARD_PINS_H

/* ---- Native USB (fixed by silicon) ---- */
#define PIN_USB_DM        19   /* USB D-  -> USB-A pin 2 */
#define PIN_USB_DP        20   /* USB D+  -> USB-A pin 3 */

/* ---- Shared SPI bus (display, SX1262 LoRa, SD card) ---- */
#define PIN_SPI_SCK       12
#define PIN_SPI_MOSI      11
#define PIN_SPI_MISO      13

/* ---- SX1262 LoRa ---- */
#define PIN_LORA_CS       10
#define PIN_LORA_RST       9
#define PIN_LORA_BUSY     14
#define PIN_LORA_DIO1     21

/* ---- ST7789 display ---- */
#define PIN_TFT_CS        15
#define PIN_TFT_DC        16
#define PIN_TFT_RST       17

/* ---- MicroSD card ---- */
#define PIN_SD_CS         18

/* ---- Shared I2C bus (PN532 NFC) ---- */
#define PIN_I2C_SDA        4
#define PIN_I2C_SCL        5

/* ---- D-pad navigation (active low) ---- */
#define PIN_BTN_UP         1
#define PIN_BTN_DOWN       2
#define PIN_BTN_LEFT       3
#define PIN_BTN_RIGHT      6
#define PIN_BTN_CENTER     7

/* ---- Infrared ---- */
#define PIN_IR_TX          8
#define PIN_IR_RX         38

#endif /* ENCHIRIDION_BOARD_PINS_H */
