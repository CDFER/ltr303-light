# LTR303 Library

This is a library to interface with the LTR303 light sensor in Arduino using the I2C protocol.

## Features
- use multiple I2C Busses -> scd4x.begin(Wire1);

## Warnings
- not all functions are implemented
- not compatible with other ltr303 arduino libraries
- only tested with the esp32
- under active development

### Setup
```c++
#include "scd4x.h"
scd4x scd4x;
double CO2 = 0, temperature = 0, humidity = 0;


Wire.begin();
scd4x.begin(Wire);
scd4x.startPeriodicMeasurement();
```
### Loop
```c++
while (scd4x.isDataReady() == false) {
	vTaskDelay(50 / portTICK_PERIOD_MS);
}

if (scd4x.readMeasurement(CO2, temperature, humidity) == 0) {
	Serial.printf("%4.0f,%2.1f,%1.0f\n", CO2, temperature, humidity);
}
vTaskDelay(4750 / portTICK_PERIOD_MS);
```

## 🖼️ Schematic
![Schematic](/images/schematic.png)
- the scd4x sensor can draw up to 205ma peaks at 3.3V (only 18ma average) so make sure you have a robust power source (the above schematic has been tested to work)
- you only need to solder the 6 pins shown and the thermal pad on the underside to get it working
- unfortunately I have not found a way to easily solder these sensors with a soldering iron, a oven or hotplate seems to be the only way
- look out for a temperature offset if you place the sensor in a case of sorts
