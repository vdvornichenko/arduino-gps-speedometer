# Arduino GPS Speedometer, HUD
## _Simple and efficient without unnecessary functions_

GPS Speedometer based on Arduino Nano board and oled/led matrix screens

Uses the following libraries:
- TinyGPS++ for GPS connection
- Adafruit_SSD1306 for SSD1306 OLED display connection
- Adafruit_SH1106 for SH1106 OLED display connection
- MD\_Parola and MD_MAX72xx, SPI for 8x8 led matrix display connection
- TimeLib for a proper time formatting (oled displays only)

## Hardware
- Arduino Nano (CH340 will work as well)
- GPS Receiver (Ublox U6M, Ublox U8M,  [M10FD](https://aliexpress.ru/item/1005004132882854.html) - this one is the best)
- Oled Display - SH1106 or SSD1306
- Led Matrix 8x8 with [MAXX7219 module](https://aliexpress.ru/item/33010367698.html) - for HUD
- 3D printed enclosure (included sample for HUD)
- Couple of wires

## Notes
GPS Receiver should be configured in u-center as per documentation [here](https://oscarliang.com/gps-settings-u-center/), Baud rate should be 115200, the best results are when receiver is set to 10hz refresh rate.
