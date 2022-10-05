#include "MQ5Sensor.h"

MQ5Sensor::MQ5Sensor(uint8_t pinA, uint8_t pinD) {
    _pinA = pinA;
    _pinD = pinD;
    pinMode(_pinD, INPUT);
}

bool MQ5Sensor::isDetect() {
    return digitalRead(_pinD) == LOW;
}

float MQ5Sensor::getGas() {
    unsigned int value = analogRead(_pinA);

    if (value >= 550) {
        value = 550;
    }

    return map(value, 0, 550, 100.0, 0.0);
}
