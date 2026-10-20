/*
 * main.c — Enchiridion firmware entry point.
 *
 * Early scaffold: logs the build banner and the configured pin map so the
 * bring-up of individual peripherals (display, LoRa, SD, NFC, IR, input) can
 * be layered on one subsystem at a time. No peripheral drivers are wired up
 * yet; see docs/project_blueprint.md for the target architecture.
 */
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "board_pins.h"

static const char *TAG = "enchiridion";

void app_main(void)
{
    ESP_LOGI(TAG, "Enchiridion firmware starting (ESP32-S3)");
    ESP_LOGI(TAG, "SPI bus: SCK=%d MOSI=%d MISO=%d",
             PIN_SPI_SCK, PIN_SPI_MOSI, PIN_SPI_MISO);
    ESP_LOGI(TAG, "LoRa (SX1262): CS=%d RST=%d BUSY=%d DIO1=%d",
             PIN_LORA_CS, PIN_LORA_RST, PIN_LORA_BUSY, PIN_LORA_DIO1);
    ESP_LOGI(TAG, "Display (ST7789): CS=%d DC=%d RST=%d",
             PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
    ESP_LOGI(TAG, "SD card: CS=%d", PIN_SD_CS);
    ESP_LOGI(TAG, "I2C (NFC): SDA=%d SCL=%d", PIN_I2C_SDA, PIN_I2C_SCL);

    /* Idle loop until subsystem bring-up is added. */
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
