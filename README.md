# LTR303 Library

This library provides an interface for the LTR303 light sensor in Arduino using the I2C protocol. The LTR303 is a low-cost and compact ambient light sensor. Please note that it is not suitable for precise lux measurements; for that use case, consider the VEML7700 sensor.

## Features

- Supports multiple I2C buses
- Option to auto-adjust the gain setting during operation
- Includes doxygen comments for all public functions
- Returns error codes for easier debugging
- Provides the `isConnected()` function to check the sensor connection status

## Warnings

- Not all functions are implemented
- Not compatible with other LTR303 Arduino libraries
- Only tested with the ESP32 platform
- Under development (as of March 2023)

### Setup

```cpp
#include "ltr303.h"
LTR303 lightSensor;
double lux = 0;

Wire.begin();
lightSensor.begin(GAIN_48X, EXPOSURE_400ms, true, Wire);
```

### Loop

```cpp
lightSensor.getApproximateLux(lux);
Serial.printf("%8.4f,\n\r", lux);

vTaskDelay(400 / portTICK_PERIOD_MS); // Wait for the next exposure to finish
```

### Verify Correct Sensor Connection

This function checks for the correct I2C response, manufacturer ID, and part ID. It prints a human-readable error report to the provided interface stream (Serial by default) and returns `true` if the device is correctly connected; otherwise, it returns `false`.

```cpp
if (light.isConnected(Wire, &Serial) == true) {
    // Sensor is correctly connected
    // Perform actions accordingly
} else {
    // Sensor connection is not successful
    // Handle the error
}
```

## Schematic

![Schematic](/images/schematic.png)

- The library has been tested up to 400kbps with the ESP32 platform using internal pullups.

## Acknowledgements

This library is based on the original work of Lovelesh Patel in 2015. You can find the original repository at [https://github.com/automote/LTR303](https://github.com/automote/LTR303).

To support the developer's work, please visit their store at [https://keastudios.co.nz/](https://keastudios.co.nz/).
