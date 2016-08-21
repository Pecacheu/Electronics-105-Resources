# Electronics 105 Class Resources
Slides, links, and code for our E105 class!

## Links:
###### Download, but don't install anything before the class!

- 1. Arduino Software at: www.arduino.cc/main/software
- 2a. DCCduino USB-to-Serial chip driver: www.wch.cn/download/CH341SER_EXE.html
- 2b. (or) Mac OS X version: www.wch.cn/download/CH341SER_MAC_ZIP.html
- 3. Download NeoPixel Library: https://github.com/adafruit/Adafruit_NeoPixel
- 4. TimerOne Library: https://github.com/PaulStoffregen/TimerOne

After installing the DCCduino driver on Mac OS X 10.9 and higher, an extra terminal command is required.
Open Terminal, enter **sudo nvram boot-args="kext-dev-mode=1"**, and hit return.

Place the libraries in `[Sketchbook Location]/libraries`.
The default sketchbook location is `Documents/Arduino`. You can change this in the Arduino IDE's preferances.