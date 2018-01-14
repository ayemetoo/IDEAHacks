# IDEAHacks

## SmartWindow
The SmartWindow is an automated air regulation system that employs natural differences in temperature and humidity. The user can also check the outside and inside values via an OLED display, as well as toggle the window system on and off.

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
The SmartWindow makes its decision on whether or not to open the window based on the temperature first, then the humidity. [IDEAHacksTeensy.ino](https://github.com/ayemetoo/IDEAHacks/blob/master/IDEAHacksTeensy/IDEAHacksTeensy.ino) is used on the outside Teensy and sensor system, while [IDEAHacksTeensy2.ino](https://github.com/ayemetoo/IDEAHacks/blob/master/IDEAHacksTeensy2/IDEAHacksTeensy2.ino) is used on the inside Teensy and sensor system. The SmartWindow default is set to attempt to maintain a target temperature of 70 degrees Fahrenheit and target humidity of 20 percent; these targets can be changed via the matrix numpad. 

#### Modifying the Target Temperature and Humidity
To put the system in select mode, hit on the numpad '**\***' to change the temperature or '**#**' to change the humidity. The display should change to show you the numbers you are entering. 
If you made a mistake and wish to undo a digit, press '**#**'. When done entering, press '**\***'. 

### Notes
* The system starts in an *on* state
* You can only enter up to three digits when selecting the new target temperature or humidity
* Values for humidity above 100 will be ignored...you can't have over 100% humidity after all!
* Once the temperature and humidity reach above a certain threshold (80 degrees Farhenheit and 40 perceent humidity), the temperature value is calculated based on the [Heat Index](https://en.wikipedia.org/wiki/Heat_index); this will be indicated by a star next to the temperature on the OLED display




