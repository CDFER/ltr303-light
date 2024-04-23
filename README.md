# LTR303 Library

The LTR303 Library provides a simple and easy-to-use interface for integrating the LTR303 ambient light sensor into your Arduino project. This compact and low-cost sensor measures the surrounding light intensity, making it suitable for a wide range of applications such as home automation, robotics, and IoT projects.

With its small size (2.0mm x 2.0mm x 0.7mm) and low power consumption (220uA active supply current), the LTR303 is an attractive option for projects that require a lightweight and energy-efficient solution. The sensor features a I2C interface with a fast mode of 400kbit/s, making it quick to communicate with your Arduino board.

The LTR303 also boasts several features, including built-in temperature compensation, immunity to IR/UV light sources, and automatic rejection of 50/60Hz flicker. With a dynamic range from 0.01 lux to 64k lux and 16-bit effective resolution, this sensor is capable of providing fairly accurate and precise measurements, however it is not suitable for precise lux measurements; for that use case, consider the VEML7700 sensor.

## Features

* Supports multiple I2C buses
* Option to auto-adjust the gain setting during operation
* Includes doxygen comments for all public functions
* Returns error codes for easier debugging
* Provides the `isConnected()` function to check the sensor connection status

## Warnings

* Only tested with the ESP32 and ESP32S2 (but should work with all arduino boards)
* Interrupt based triggers not implemented

### Setup

```cpp
#include "ltr303.h"

LTR303 lightSensor;
double lux = 0.0;

Wire.begin();
lightSensor.begin(GAIN_48X, EXPOSURE_400ms, true, Wire);
lightSensor.startPeriodicMeasurement();
```

### Loop

```cpp
if(lightSensor.getApproximateLux(lux)){
Serial.printf("%8.4f,\n\r", lux);
}
delay(1000);
```

### Verify Correct Sensor Connection

This function checks for the correct I2C response, manufacturer ID, and part ID. It prints a human-readable error report to the provided interface stream (Serial by default) and returns `true` if the device is correctly connected; otherwise, it returns `false`.

```cpp
if (lightSensor.isConnected(Wire, &Serial) == true) {}
```

## Schematic

![Schematic](/images/schematic.png)

- The library has been tested at 400khz (for over a year) with the ESP32 using internal pullups with no errors.

## Acknowledgements

This library is based on the original work of Lovelesh Patel in 2015. You can find the original repository at [https://github.com/automote/LTR303](https://github.com/automote/LTR303).

To support my work, consider visiting my store at [https://keastudios.co.nz/](https://keastudios.co.nz/).