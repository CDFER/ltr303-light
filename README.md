# LTR303 Library
This is a library to interface with the LTR303 light sensor in Arduino using the I2C protocol.

## Features
- use multiple I2C Busses
- option to auto-adjust the gain setting in flight
- doxygen comments for all public functions
- returns error codes
- isConnected() function 

## Warnings
- not all functions are implemented
- not compatible with other ltr303 Arduino libraries
- only tested with the esp32
- under development (as of March 2023)

### Setup
```c++
#include "ltr303.h"
LTR303 light;
double lux = 0;

Wire.begin();
light.begin(GAIN_48X, EXPOSURE_400ms, true);
```

### Loop
```c++
light.getLux(lux);
Serial.printf("%8.4f,\n\r",lux);

vTaskDelay(400 / portTICK_PERIOD_MS);
```

### Verify Correct Sensor Connection
checks for correct  i2c response, manufacturer id and part id.
Prints a human-readable error report to provided interface stream (serial by default)
@returns true if the device is correctly connected, otherwise false
```c++
if (light.isConnected(Wire, &Serial) == true){...
```

## 🖼️ Schematic
![Schematic](/images/schematic.png)
- I have tested this up to 400kbps with the esp32 and it works great (only using esp32 internal pullups)


## Based on the awesome work of Lovelesh Patel @thingtronics
Origin created by Lovelesh Patel in 2015
https://github.com/automote/LTR303

To help support my work check out my store: https://keastudios.co.nz/
