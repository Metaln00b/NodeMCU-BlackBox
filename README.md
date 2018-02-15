# NodeMCU-BlackBox
ESP8266 based CAN-Bus Diagnostic Tool

## Introduction

# I2C Pinout

* SDA -> D2
* SCL -> D1

# MCP2515 Pinout

* INT: D4
* SCK: CLK (SCLK) or D5 (HSCLK)
* MOSI: SD1 (MOSI) or D7 (HMOSI)
* MISO: SD0 (MISO) or D6 (HMISO)
* CS: D3 or D8

# Libraries

* https://github.com/esp8266/Arduino

* https://github.com/coryjfowler/MCP_CAN_lib

* https://github.com/esp8266/Arduino/blob/master/doc/filesystem.rst

* https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home *customized*

# Configuration

* Board: WeMos D1 R2 & mini
* Flash Size: 4M *3M SPIFFS*
* CPU Freq.: 160 MHz
* Upload Speed: 921600