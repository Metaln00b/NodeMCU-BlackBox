# NodeMCU-BlackBox
ESP8266 based CAN-Bus Diagnostic Tool

**ID-Config for SMART CDI 2003**

## Introduction

### I2C Display Connection *level shifter*

| Display | ESP8266 |
| ------- | ------- |
| SDA     | D2      |
| SCL     | D1      |
| GND     | GND     |
| VCC     | 5V *external* |

Please note: The NodeMCU is not official 5V tolerant.
Please using a *level shifter* for the backpack.

### MCP2515 Connection

| MCP2515 | ESP8266 |
| ------- | ------- |
| INT     | D4      |
| SCK     | D5      |
| MOSI    | D7      |
| MISO    | D6      |
| CS      | D3      |
| GND     | GND     |
| VCC     | 3V3     |
| VCC-*TJA5010* | 5V *separately/external* |

Please note: The NodeMCU is not official 5V tolerant.
You need to cut off the power supply of the *TJA1050-Chip* and
connect it separately to the 5V of an external power supply,
because the *TJA1050-Chip* can not run with 3V3.

For more informations:
* https://www.raspberrypi.org/forums/viewtopic.php?t=141052

### Libraries

* https://github.com/esp8266/Arduino

* https://github.com/coryjfowler/MCP_CAN_lib

* https://github.com/esp8266/Arduino/blob/master/doc/filesystem.rst

* https://github.com/fmalpartida/New-LiquidCrystal *customized*

### Configuration

* Board: WeMos D1 R2 & mini or NodeMCU 0.9/1.0
* Flash Size: 4M *3M SPIFFS*
* CPU Freq.: 160 MHz
* Upload Speed: 921600

### Parts

* MCP2515
* NodeMCU
* I2C Display 4x20
* Level Shifter

## License

This project is licensed under the MIT License - see the [license file](LICENSE) for details.
