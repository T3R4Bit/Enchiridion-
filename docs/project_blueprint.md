# Project Architecture: Handheld Cyber-Multi-Tool & Field Node

## 1. Project Summary
This project outlines the development of a custom, handheld hardware multi-tool inspired by devices like the Flipper Zero, but upgraded with modern **2.4 GHz Wi-Fi analysis**, **Sub-GHz LoRa (Meshtastic)**, **NFC/RFID**, **BadUSB payload execution**, and a **USB Ethernet Tailscale VPN bridge**.

The form factor uses an **18650 cylindrical battery** inside a **3D-printed enclosure** (similar in size to a Baofeng UV-5R), optimizing durability, cost, and physical safety.

---

## 2. Core Functional Requirements

* **Processing Unit:** ESP32-S3 (Dual-core Xtensa LX7, 240MHz, 16MB Flash, 8MB PSRAM).
* **Wi-Fi & Bluetooth (2.4 GHz):** Native ESP32-S3 radio for packet sniffing, deauth detection, rogue AP scanning, and upstream internet connectivity.
* **Long-Range Mesh Communication (Sub-GHz):** Semtech SX1262 LoRa module for Meshtastic network support.
* **Proximity & Access Control:**
  * **13.56 MHz (HF NFC/RFID):** PN532 IC for reading/emulating smart cards and tags.
  * **125 kHz (LF RFID):** Inductive coil driver for building access tags (EM4100 / HID Prox).
* **Target Computer Interaction:** Native USB OTG via a dedicated **USB-A port** for BadUSB ducky script injection and USB Ethernet (RNDIS/CDC-ECM) Tailscale tunneling.
* **Power & Charging:** Dedicated **USB-C power port** feeding a TP4056 charger and protection circuit, powering a single 18650 Li-ion cell.
* **User Interface:** 1.3"–2.0" SPI TFT color display paired with a 5-way D-pad navigation button setup.
* **Storage:** Onboard MicroSD card slot for payload, log, and configuration storage.

---

## 3. Detailed Bill of Materials (BOM) & Justification

| Category | Component / Module | Interface | Primary Function / Technical Rationale |
| :--- | :--- | :--- | :--- |
| **Microcontroller** | ESP32-S3-WROOM-1-N16R8 | Native | Main processor. PSRAM (8MB) is critical for running embedded Tailscale (`MicroLink`) without out-of-memory errors. |
| **LoRa Module** | Semtech SX1262 (e.g., EBYTE E22) | SPI | Meshtastic node support and long-range Sub-GHz communication. |
| **NFC / HF RFID** | PN532 Module | I2C / SPI | High-frequency smart card and MIFARE reading/cloning. |
| **LF RFID** | 125kHz Inductive Coil Circuit | GPIO / PWM | Low-frequency proximity badge interaction. |
| **Display** | 1.3" - 2.0" ST7789 Color TFT LCD | SPI | Navigation, UI display, traffic visualization, and status readouts. |
| **Storage** | MicroSD Card Slot | SPI | Wordlists, ducky scripts, UI graphics, and log files. |
| **Infrared** | IR LED Transmitter + TSOP Receiver | GPIO | Universal TV / AC remote code capturing and replaying. |
| **Battery** | 18650 3.7V Li-ion Cell (2500–3500mAh) | Power Rail | High capacity, puncture resistance, and easy replacement. |
| **Battery Holder** | Surface-Mount (SMD) 18650 Holder | Power Rail | Solderable battery compartment attached to the board assembly. |
| **Battery Charger** | TP4056 + DW01A + FS8205A | Analog / USB | Linear charging controller with integrated battery protection. |
| **Data Connector** | USB-A Female Receptacle | USB OTG | Connects directly to target PCs for BadUSB or USB Ethernet routing. |
| **Power Connector**| USB-C Female Receptacle | Power Only | Dedicated charging port with isolated power lines. |

---

## 4. GPIO Pin Mapping Schedule (ESP32-S3)

*Note: Pins 19 and 20 are hardware-dedicated to native USB D-/D+ signals.*

| Subsystem | Signal Name | ESP32-S3 GPIO Pin | Notes |
| :--- | :--- | :--- | :--- |
| **Native USB Data** | USB D- | GPIO 19 | Connects to USB-A Port Pin 2 |
| **Native USB Data** | USB D+ | GPIO 20 | Connects to USB-A Port Pin 3 |
| **SPI Bus (Shared)** | SPI SCK | GPIO 12 | Main SPI Clock (Display, SX1262, SD Card) |
| | SPI MOSI | GPIO 11 | Main SPI Data Out |
| | SPI MISO | GPIO 13 | Main SPI Data In |
| **SX1262 LoRa** | CS (Chip Select) | GPIO 10 | Dedicated Chip Select |
| | RST (Reset) | GPIO 9 | Reset pin |
| | BUSY | GPIO 14 | Status pin |
| | DIO1 | GPIO 21 | Interrupt pin |
| **Display (ST7789)**| CS (Chip Select) | GPIO 15 | Dedicated Chip Select |
| | DC (Data/Command)| GPIO 16 | Control line |
| | RST (Reset) | GPIO 17 | Reset line |
| **MicroSD Card** | CS (Chip Select) | GPIO 18 | Dedicated Chip Select |
| **I2C Bus (NFC)** | SDA | GPIO 4 | Shared I2C Data line |
| | SCL | GPIO 5 | Shared I2C Clock line |
| **User Inputs** | D-Pad Up / Down / L / R / Center | GPIO 1, 2, 3, 6, 7 | Active low navigation buttons |
| **Infrared** | IR TX / RX | GPIO 8 / GPIO 38 | Transmit / Receive GPIOs |

---

## 5. Architectural Diagram

```text
                  +-----------------------------------+
                  |   ESP32-S3-N16R8 Microcontroller  |
                  |     (Dual Core + 8MB PSRAM)       |
                  +-----------------+-----------------+
                                    |
     +------------------------------+------------------------------+
     |                              |                              |
[SPI Bus / Native]              [I2C / UART]                 [GPIO Lines]
     |                              |                              |
 +---+----------------+        +----+---------------+        +-----+--------------+
 | - SX1262 LoRa      |        | - PN532 NFC        |        | - IR TX/RX         |
 | - MicroSD Card     |        | - 125kHz RFID      |        | - D-Pad / Buttons  |
 | - SPI TFT Screen   |        |                    |        | - Status LEDs      |
 +--------------------+        +--------------------+        +--------------------+
     |
     +---> [USB-A Port (Data)] ---> Target PC (BadUSB / Tailscale Bridge)
     
     +---> [18650 Cell + TP4056] <--- [USB-C Port (Power Only)]
```

---

## 6. Manufacturing & Assembly Strategy

1. **Schematic & PCB Design:** Designed using **KiCad 8**.
2. **Factory Assembly Optimization:**
   * Design as a **2-layer PCB**.
   * Place **all surface-mount components (SMD)** exclusively on the **Top Layer** to eliminate double-sided SMT reflow costs during manufacturing (e.g., at JLCPCB or PCBWay).
   * Hand-solder the SMD 18650 battery holder onto the bottom side after board delivery.
3. **Enclosure:** Modeled in **Fusion 360** using exported `.STEP` files from KiCad to ensure precision fitment inside a 3D-printed PETG or ABS handheld shell.