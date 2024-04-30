> [!CAUTION]
> This does not fulfill any safety standards. Use this at your own risk!

# FAQ

## What is this?

This project aims to create a retrofit chamber heater for any 3D printer that doesn't have one. 
Maybe even the printers which already have one... I won't stop you.

## Why would you do this?

Some filaments require high ambient temperatures to prevent/reduce warping. 

## Why not just buy a printer with chamber heater?

I don't know. Maybe for the thrill of the possibility to burn down your house?

## What if I don't want to burn down my house?

See the [safety section](#safety)

# Spec Sheet (aka. Wishlist)

- [ ] relatively cheap solution
- [ ] Heats chambers up to 80 °C (maybe only up to 60 °C, lets see)
- [ ] PID regulator for the heater unit
- [ ] extended [safety features](#safety)
- [ ] can be controlled via [Telegram Bot](https://core.telegram.org/bots)
- [ ] (maybe sometime) can be set to automatic mode for bambulab printers (predefined chamber temperatures depending on the loaded filament type; automatic cooldown after a print finished; ...)

# Safety

- [ ] a thermo fuse cuts the mains power to the heater even if the micro controller fails completely
- [ ] configurable high-prio-timeout: heater unit will be shut down after the configured duration, independent of any other state
- [ ] galvanic isolation between low-voltage (micro controller, sensors, fan) and high-voltage (heater unit)
- [x] multiple different temperature sensors are used to verify the readings (currently DHT22 and DS18B20 are supported)
- [ ] temperature readings for each sensor can be checked at any time using the telegram bot
- [ ] if the readings differ more than 5 °C for an extended period of time, the heater is shut down
- [ ] if the temperature does not rise within a reasonable time, the heater is shut down
- [ ] fan RPM supervision for failures
- [ ] cyclic plausibility checks for the fan (every few minutes the µC changes the PWM duty cycle and checks if the RPM follow the change)

# Required hardware

- Thermofuse for mains, i.e [one of these](https://www.amazon.de/gp/product/B0759G3P85/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&th=1)
- mains powered heater, i.e [this 300W unit](https://www.amazon.de/gp/product/B0BWRHHYKV/ref=ppx_yo_dt_b_asin_title_o00_s01?ie=UTF8&psc=1)
- PWM controller board for the heater. *Must* have galvanic isolation linke [this one](https://www.amazon.de/gp/product/B0BLS653TW/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)
- a [Wemos D1](https://www.wemos.cc/en/latest/d1/index.html) or compatible micro controller
- power supply for the micro controller
- some temperature sensors like DHT22 and/or DS18B20 (at least 2 are *required*)
- fan with tacho signal (3-pin or 4-pin) for the heater unit
- additional discrete parts for the chosen components, i.e. pull-up resistors for the chosen temperature sensor types

# Wiring

*TODO*

# Setup

## Sensor configuration

*TODO*

## Telegram bot integration

*TODO*

## PID tuning

*TODO*
