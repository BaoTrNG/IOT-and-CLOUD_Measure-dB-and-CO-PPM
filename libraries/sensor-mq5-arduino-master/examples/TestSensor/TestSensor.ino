#include <Arduino.h>
#include <MQ5Sensor.h>

#define debug_port Serial
#define BAUD_RATE 115200
#define PIN_ANALOG_MQ5_SENSOR A0
#define PIN_DIGITAL_MQ5_SENSOR D0

MQ5Sensor mq5(PIN_ANALOG_MQ5_SENSOR, PIN_DIGITAL_MQ5_SENSOR);

void setup() {
    debug_port.begin(BAUD_RATE);
}

void loop() {
    debug_port.printf("Gas = %0.2f\tDetect = %d\n", mq5.getGas(), mq5.isDetect());
    delay(1000);
}
