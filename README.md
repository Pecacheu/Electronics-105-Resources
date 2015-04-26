# Electronics 102 Class Resources
Slides, links, and code for our E102 class!

## Links:
###### Download, but don't install anything before the class!

1. Arduino Software at: www.arduino.cc/main/software
2. DCCduino USB-to-Serial chip driver: www.wch.cn/download/CH341SER_EXE.html
3. Mac OS X version: www.wch.cn/download/CH341SER_MAC_ZIP.html
4. Download NeoPixel Library: https://github.com/adafruit/Adafruit_NeoPixel

After installing the DCCduino driver on Mac OS X 10.9 and higher, an extra terminal command is required.
Open Terminal, enter **sudo nvram boot-args="kext-dev-mode=1"**, and hit return.

Place the library in `[Sketchbook Location]/libraries`.
The default sketchbook location is `Documents/Arduino`. You can change this in the Arduino IDE's preferances.
