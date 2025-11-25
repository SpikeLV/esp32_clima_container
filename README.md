# ESP32 Climate Container Controller

ESP32-based climate control system for storage containers with temperature and humidity monitoring, ventilation, and heating control.

## Features

- **Temperature & Humidity Monitoring**: DHT22/AM2305 sensor for real-time environmental data
- **Climate Control**: Automated ventilation and heating control
- **Nextion Display**: Touchscreen interface for monitoring and control
- **WiFi Connectivity**: Network connection for remote monitoring (configurable)
- **Relay Control**: Independent control of ventilation fan and heater

## Hardware Requirements

- ESP32 development board
- DHT22/AM2305 temperature and humidity sensor
- Nextion display (with TFT file: `main/data/sushka.tft`)
- Relay modules for ventilation and heating
- Power supply

## Pin Configuration

| Component | Pin | Description |
|-----------|-----|-------------|
| Ventilation Relay | GPIO 14 | Controls ventilation fan |
| Heater Relay | GPIO 27 | Controls heating element |
| DHT22/AM2305 | GPIO 15 | Temperature & humidity sensor |
| Nextion RX | GPIO 16 | Display communication (RX) |
| Nextion TX | GPIO 17 | Display communication (TX) |

## Project Structure

```
container-climat-esp32/
├── main/                    # Main source code
│   ├── main.ino            # Main entry point
│   ├── 1_libs.ino          # Library includes
│   ├── 2_define.ino        # Definitions and pin assignments
│   ├── 3_variables.ino     # Global variables
│   ├── 4_main.ino          # Setup and main loop
│   ├── 5_relay.ino         # Relay control functions
│   ├── 6_DHT_AM2305.ino    # Sensor reading functions
│   ├── 7_screen.ino        # Nextion display functions
│   └── data/
│       └── sushka.tft      # Nextion display TFT file
└── ekran/                   # Display assets and configuration
    ├── container-climat-esp32.HMI  # Nextion project file
    ├── art/                 # UI assets (buttons, icons, backgrounds)
    └── fonts/               # Custom fonts
```

## Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/SpikeLV/esp32_clima_container.git
   ```

2. Open the project in Arduino IDE or PlatformIO

3. Install required libraries:
   - DHT sensor library
   - HardwareSerial (built-in)

4. Upload the TFT file to your Nextion display using Nextion Editor

5. Configure WiFi credentials in `main/2_define.ino`:
   ```cpp
   #define NET_SSID "YourWiFiSSID"
   #define NET_PASSWORD "YourWiFiPassword"
   ```

6. Upload the code to your ESP32

## Configuration

### WiFi Settings
Edit `main/2_define.ino` to set your WiFi credentials. Different networks can be configured for debug and production modes.

### Debug Mode
Set `DEBUG` to `1` in `main/2_define.ino` to enable serial debugging output.

## Usage

1. Power on the ESP32
2. The system will initialize:
   - Relays are set up
   - Sensor is calibrated
   - Display shows current temperature and humidity
3. Use the Nextion touchscreen to:
   - Monitor temperature and humidity
   - Control ventilation manually
   - Control heating manually
   - View system status

## Device States

The system operates in the following states:
- `NONE`: No active operation
- `STARTING`: System initialization
- `WARMUP`: Heating phase
- `RUNNING`: Normal operation
- `COOLDOWN`: Cooling phase
- `STOPPING`: Shutdown sequence
- `STOPPED`: System stopped

## License

This project is open source. Please check the repository for license details.

## Author

SpikeLV

## Repository

https://github.com/SpikeLV/esp32_clima_container.git

