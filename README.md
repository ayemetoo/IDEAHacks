# IDEAHacks

## SmartWindow
The SmartWindow is an automated window system that keeps a window opened or closed depending on the temperatures and humidities outside and inside a room. The user can also check the outside and inside values via an OLED display, as well as toggle the window system on and off.

### Components
* [Teensy LC](https://www.pjrc.com/teensy/teensyLC.html), 2
* [HTU21D Temperature & Humidity Sensor](https://www.adafruit.com/product/1899), 2
* [Continuous Rotation Servo](https://www.adafruit.com/product/154), 1
* [nRF24L01+ RF Transceiver](https://www.nordicsemi.com/eng/Products/2.4GHz-RF/nRF24L01P), 2
* [1.3" OLED Display](https://www.adafruit.com/product/938), 1
* [Capacitive Touch Button](https://www.sparkfun.com/products/12041), 1
* 5V regulators, 2
* Window, 1
* Various passives (wires, headers, etc) and power supplies

### Functionality
The SmartWindow makes its decision on whether or not to open the window based on the temperature first, then the humidity. [IDEAHacksTeensy.ino](https://github.com/ayemetoo/IDEAHacks/blob/master/IDEAHacksTeensy/IDEAHacksTeensy.ino) is used on the outside Teensy and sensor system, while [IDEAHacksTeensy2.ino](https://github.com/ayemetoo/IDEAHacks/blob/master/IDEAHacksTeensy2/IDEAHacksTeensy2.ino) is used on the inside Teensy and sensor system. The SmartWindow is set to attempt to maintain a target temperature of 70 degrees Fahrenheit and target humidity of 20 percent; these targets can be changed by modifying the target variables (**target** and **targethumd**) in [IDEAHacksTeensy2.ino](https://github.com/ayemetoo/IDEAHacks/blob/master/IDEAHacksTeensy2/IDEAHacksTeensy2.ino).

#### Notes
* The system starts in an *on* state
* Once the temperature and humidity reach above a certain threshold (80 degrees Farhenheit and 40 perceent humidity), the temperature value is calculated based on the [Heat Index](https://en.wikipedia.org/wiki/Heat_index); this will be indicated by a star next to the temperature on the OLED display.




