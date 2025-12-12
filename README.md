# ESP32-C3 WiFi SD Card File Server

This project turns an **ESP32-C3** into a **WiFi-accessible SD card file server**.  
Files on a microSD card can be **uploaded, downloaded, listed, and deleted** through a simple web interface, without removing the SD card.

The ESP32 hosts its own WiFi Access Point and serves a browser-based file manager.

---

## Features

- 📡 WiFi Access Point (no external router required)
- 📂 Browse files on the SD card
- ⬆️ Upload files via browser
- ⬇️ Download files
- ❌ Delete files
- 💾 FAT32 SD card support
- ⚡ SPI-based SD interface
- 🧠 Designed for ESP32-C3 boards (tested on ESP32-C3-042-OLED)

---

## Hardware Requirements

- ESP32-C3 board (e.g. **ESP32-C3-042-OLED**)
- SPI microSD card module (e.g. MH-SD Card Module)
- microSD card (FAT32 formatted)
- USB cable for flashing and power

---

## Wiring (SPI)

| SD Module | ESP32-C3 GPIO |
|----------|---------------|
| 3V3      | 3V3           |
| GND      | GND           |
| CS       | GPIO10        |
| MOSI     | GPIO3         |
| SCK      | GPIO4         |
| MISO     | GPIO1         |

⚠️ **Do not connect 5V to the SD module. Use 3.3V only.**

---

## Software Requirements

- Arduino IDE 2.x
- ESP32 Arduino Core (by Espressif)
- Libraries:
  - `AsyncTCP`
  - `ESPAsyncWebServer`

Install libraries via **Arduino Library Manager**.

---

## Installation & Flashing

1. Install Arduino IDE
2. Add ESP32 board support: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
3. Select board:  **Tools → Board → ESP32C3 Dev Module**
4. Connect ESP32-C3 via USB
5. Open the sketch and upload  
(hold the BOOT button if required)

---

## Usage

1. Power the ESP32-C3
2. Connect to WiFi:
- **SSID:** `ESP32C3-SD`
- **Password:** `12345678`
3. Open a browser and go to: http://192.168.4.1
4. Use the web interface to manage files on the SD card

---

## Notes & Limitations

- The SD card is mounted in **SPI mode**, not SDMMC
- USB Mass Storage is **not supported** on ESP32-C3
- Avoid accessing the SD card from multiple sources simultaneously
- Large file uploads depend on SD card quality and SPI speed

---

## Tested Hardware

- ESP32-C3-042-OLED
- MH-SD Card Module
- SanDisk / Kingston microSD (FAT32)

---

## License

Copyright (c) 2025 Hammerbyte DE

Licensed under the MIT License.

---

## Credits

- Espressif Systems
- ESPAsyncWebServer & AsyncTCP by Hristo Gochkov