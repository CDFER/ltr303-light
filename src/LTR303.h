/*
	LTR303 sensor library for Arduino
	Lovelesh, thingTronics
	Chris Dirks, keaStudios

Shared under the MIT License

Copyright (c) 2015 thingTronics Limited
Copyright (c) 2023 Chris Dirks

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef LTR303_h
#define LTR303_h

#include <Arduino.h>
#include <Wire.h>

#define LTR303_ADDR 0x29  // default i2c address

// LTR303 register addresses
#define LTR303_CONTR 0x80
#define LTR303_MEAS_RATE 0x85
#define LTR303_PART_ID 0xA0
#define LTR303_MANUFACTURER_ID 0x05
#define LTR303_DATA_CH1_0 0x88
#define LTR303_DATA_CH1_1 0x89
#define LTR303_DATA_CH0_0 0x8A
#define LTR303_DATA_CH0_1 0x8B
#define LTR303_STATUS 0x8C

// gain = 0  => 1X gain (default)
// gain = 1  => 2X gain
// gain = 2  => 4X gain
// gain = 3  => 8X gain
// gain = 4, invalid
// gain = 5, invalid
// gain = 6  => 48X gain
// gain = 7  => 96X gain
enum ltr303Gain {
	GAIN_1X = 0,
	GAIN_2X = 1,
	GAIN_4X = 2,
	GAIN_8X = 3,
	GAIN_48X = 6,
	GAIN_96X = 7
};

// Exposure = 0 => 100ms (default)
// Exposure = 1 => 50ms
// Exposure = 2 => 200ms
// Exposure = 3 => 400ms
// Exposure = 4 => 150ms
// Exposure = 5 => 250ms
// Exposure = 6 => 300ms
// Exposure = 7 => 350ms
enum ltr303Exposure {
	EXPOSURE_50ms = 1,
	EXPOSURE_100ms = 0,
	EXPOSURE_150ms = 4,
	EXPOSURE_200ms = 2,
	EXPOSURE_250ms = 5,
	EXPOSURE_300ms = 6,
	EXPOSURE_350ms = 7,
	EXPOSURE_400ms = 3
};

// interval = 0 =>50ms
// interval = 1 =>100ms
// interval = 2 =>200ms
// interval = 3 =>500ms (default)
// interval = 4 =>1000ms
// interval = 5 =>2000ms
// interval = 6 =>2000ms
// interval = 7 =>2000ms
enum ltr303Interval {
	LTR303_50ms_INTERVAL = 0,
	LTR303_100ms_INTERVAL = 1,
	LTR303_200ms_INTERVAL = 2,
	LTR303_500ms_INTERVAL = 3,
};

class LTR303 {
   public:
	/**
	 * initializes the sensor.
	 *
	 * @param gain (enum) how much to amplify the reading (GAIN_1X)
	 * @param exposure (enum) how long to keep the "shutter" open (EXPOSURE_100ms)
	 * @param enableAutoGain change gain when new data received
	 * @param port Wire instance (e.g Wire or Wire1)
	 * @param addr i2c address of sensor
	 * @retval 0 success
	 * @retval 1 i2c data too long to fit in transmit buffer
	 * @retval 2 i2c received NACK on transmit of address
	 * @retval 3 i2c received NACK on transmit of data
	 * @retval 4 i2c other error
	 * @retval 5 i2c timeout
	 */
	uint8_t begin(ltr303Gain gain, ltr303Exposure exposure, bool enableAutoGain, TwoWire &port = Wire, uint8_t addr = LTR303_ADDR);

	/**
	 * puts the sensor into active mode
	 *
	 * @retval 0 success
	 * @retval 1 i2c data too long to fit in transmit buffer
	 * @retval 2 i2c received NACK on transmit of address
	 * @retval 3 i2c received NACK on transmit of data
	 * @retval 4 i2c other error
	 * @retval 5 i2c timeout
	 */
	uint8_t startPeriodicMeasurement();

	/**
	 * puts the sensor into idle mode
	 *
	 * @retval 0 success
	 * @retval 1 i2c data too long to fit in transmit buffer
	 * @retval 2 i2c received NACK on transmit of address
	 * @retval 3 i2c received NACK on transmit of data
	 * @retval 4 i2c other error
	 * @retval 5 i2c timeout
	 */
	uint8_t endPeriodicMeasurement();

	/**
	 * checks for correct response, manufacturer id and part id.
	 *
	 * @param port Wire instance (e.g Wire or Wire1)
	 * @param stream debug output pointer (e.g. &Serial)
	 * @param addr i2c address of sensor
	 * @returns true if device correctly connected, otherwise false
	 */
	bool isConnected(TwoWire &port = Wire, Stream *stream = &Serial, uint8_t addr = LTR303_ADDR);

	/**
	 * gets Raw light count from sensor (no gain or exposure compensation and no checking if data is valid)
	 *
	 * @param visibleAndIRraw where to send the visible and IR sensor data
	 * @param IRraw where to send the IR sensor output data
	 * @retval 0 success
	 * @retval 1 i2c data too long to fit in transmit buffer
	 * @retval 2 i2c received NACK on transmit of address
	 * @retval 3 i2c received NACK on transmit of data
	 * @retval 4 i2c other error
	 * @retval 5 i2c timeout
	 * @retval 6 no data received on request
	 * @retval 7 no new data available
	 */
	uint8_t getData(uint16_t &visibleAndIRraw, uint16_t &IRraw);

	/**
	 * gets Raw, checks validity, adjusts for gain and exposure and scales to passes back a approximate lux
	 *
	 * @param lux where to send the output value
	 * @returns true if data returned is valid, otherwise false
	 */
	bool getApproximateLux(double &lux);

   private:
	uint8_t reset();

	bool newDataAvailable();

	// Sets the gain, SW reset and mode of LTR303
	uint8_t setControlRegister(bool reset = false, bool mode = true);

	bool autoGain(uint16_t visibleAndIRraw);
	uint8_t setExposureTime();

	uint8_t read16bitInt(uint8_t address, uint16_t &value);

	uint8_t _i2c_address;
	TwoWire *_i2cPort;
	Stream *_debug_output_stream = &Serial;
	uint8_t _error;
	bool _dataValid;
	ltr303Gain _gain;
	double _gainCompensation;
	ltr303Exposure _exposure;
	double _exposureCompensation;
	bool _autoGainEnabled;
};

#endif