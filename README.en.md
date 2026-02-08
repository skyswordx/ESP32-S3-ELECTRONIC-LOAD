<div align="center">

[简体中文](README.md) | **English**

![Hardware Demo](assets-of-README/image-demo.png)

# ELEVOURER

Portable intelligent electronic load based on ESP32-S3 dual-core architecture and multi-task design

[![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue?style=flat-square&logo=espressif)](https://www.espressif.com/en/products/socs/esp32-s3)
[![Framework](https://img.shields.io/badge/Framework-Arduino-00979D?style=flat-square&logo=arduino)](https://www.arduino.cc/)
[![License](https://img.shields.io/badge/License-MIT-green?style=flat-square&logo=open-source-initiative)](LICENSE)
[![Build](https://img.shields.io/badge/Build-PlatformIO-orange?style=flat-square&logo=platformio)](https://platformio.org/)
[![Display](https://img.shields.io/badge/Display-LVGL-purple?style=flat-square)](https://lvgl.io/)
[![Controller](https://img.shields.io/badge/Controller-PID%20%2B%20KalmanFilter-red?style=flat-square&logo=controlthings)](https://github.com/)

---

</div>

## Overview

This project is a portable intelligent DC electronic load controller for power supply validation, battery discharge testing, and circuit debugging scenarios. The control core is ESP32-S3, supporting three operating modes: CC/CP/CR, with a touchscreen GUI and real-time telemetry monitoring.

Note: the repository contains MPC controller code, but it is currently implemented only at code level and has not completed full system integration and end-to-end validation. The default control path is PID.

## Key Features

- Three load modes: Constant Current (CC), Constant Power (CP), Constant Resistance (CR)
- Real-time sensing: INA226 voltage/current/power monitoring
- GUI: 3.5-inch TFT touchscreen + LVGL
- Safety protection: over-voltage, over-current, and over-power protections
- Software architecture: FreeRTOS multi-task scheduling with modular components

## Project Structure

```text
ESP32-S3-ELECTRONIC-LOAD/
├── src/                 # Main entry and task scheduling
├── lib/                 # Component libraries (PID/MPC/ADC/LVGL, etc.)
├── include/             # Header files
├── test/                # Module tests and demos
├── document/            # Design/debug/algorithm documents
├── assets-of-README/    # README assets
├── platformio.ini       # PlatformIO configuration
├── README.md            # Chinese documentation
└── README.en.md         # English documentation
```

## Tech Stack

- Hardware: ESP32-S3, INA226, MCP4725, 3.5-inch ILI9488 touchscreen
- Software: PlatformIO + Arduino (with ESP-IDF components), FreeRTOS, LVGL
- Control/filtering: PID, Kalman filter; MPC implemented but not fully integrated/tested

## Hardware Connections (Default Example)

```text
ESP32-S3 pin mapping:
├── INA226: SDA=GPIO8, SCL=GPIO9
├── MCP4725: SDA=GPIO8, SCL=GPIO9
├── TFT: MOSI=GPIO11, MISO=GPIO13, SCK=GPIO12
├── Touch: CS=GPIO15, IRQ=GPIO21
├── Encoder: CLK=GPIO1, DT=GPIO2, SW=GPIO42
└── Buttons: GPIO40, GPIO41, GPIO39, GPIO38
```

## Quick Start

### 1. Environment Setup

```bash
# Install PlatformIO extension
vscode --install-extension platformio.platformio-ide

# Clone repository
git clone https://github.com/your-username/ESP32-S3-ELECTRONIC-LOAD.git
cd ESP32-S3-ELECTRONIC-LOAD
```

### 2. Build and Flash

After setting up `pio` in your PATH, you can use CLI commands or the PlatformIO panel in VS Code.

```bash
# Build project
pio run

# Upload firmware
pio run --target upload

# Monitor serial output
pio device monitor
```

### 3. Basic Usage (Example)

1. Power on and wait for initialization
2. Select mode (CC/CP/CR)
3. Adjust target value using the encoder
4. Start/stop load via control button
5. Monitor telemetry via touchscreen GUI

## Demo Videos

- 2025-06-14 Smart DC electronic load demo: https://www.bilibili.com/video/BV1d8MUzvEH5/
- 2025-05-05 Simple DC electronic load demo: https://www.bilibili.com/video/BV1UhVzzkEg6/
- 2025-04-20 System-level electronic load test: https://www.bilibili.com/video/BV1Cc5fzoE1t/
- 2025-03-27 Constant-current test: https://www.bilibili.com/video/BV1WaRXYPE3B/

## Documentation

For more design notes, debugging logs, and algorithm derivations, see `document/`.

## Contributing

Issues and PRs are welcome. See [Contribution Guide](CONTRIBUTING.md).

## License

This project is licensed under the [MIT License](LICENSE).
