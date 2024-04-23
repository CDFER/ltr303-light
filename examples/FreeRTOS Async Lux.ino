#include <Arduino.h>
#include <LTR303.h>

// Create a queue to store the lux values
xQueueHandle xLuxQueue;

// Task to read and calculate lux values from the LTR303 sensor
void ltr303Task(void *pvParameter) {
    // Create an instance of the LTR303 sensor
    LTR303 ltr303;

    Wire1.begin();  //Uses default pins
    //Wire1.begin(/*Write SDA_PIN here*/, /*Write SCL_PIN here*/); //custom i2c pins
    //Wire1.begin(/*Write SDA_PIN here*/, /*Write SCL_PIN here*/, 500000);  // tested to be 380khz irl on the esp32 (<400khz per data sheet)

    uint8_t error = ltr303.begin(GAIN_1X, EXPOSURE_100ms, true, Wire1);
    if (error != 0) {
        Serial.printf("Error initializing LTR303: %s\r\n", ltr303.getErrorText(error));
        return;
    }

    // Start periodic measurement mode
    ltr303.startPeriodicMeasurement();

    double newLux = 0.0;
    while (true) {
        // Get an approximate lux value from the LTR303 sensor
        if (ltr303.getApproximateLux(newLux)) {
            xQueueSend(xLuxQueue, &newLux, portMAX_DELAY);
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);  // check the sensor every 50ms (this is the minimum measurement interval)
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println(__FILE__);
    Serial.printf("LTR303 Library Version: %s\r\n\r\n", LTR303_LIB_VERSION);

    xTaskCreatePinnedToCore(ltr303Task, "LTR303 Task", 4096, NULL, tskIDLE_PRIORITY + 1, NULL, 0);
}

void loop() {
    double lux = 0.0;
    while (xQueueReceive(xLuxQueue, &lux, portMAX_DELAY)) {
        Serial.printf("Light Level = %8.4flux\r\n", lux);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);  // check for new items in the queue every 10ms
}