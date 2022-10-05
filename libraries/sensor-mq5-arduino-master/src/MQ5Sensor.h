#ifndef MQ5Sensor_h
#define MQ5Sensor_h

#include <Arduino.h>

class MQ5Sensor {
private:
    uint8_t _pinA;
    uint8_t _pinD;
public:
    MQ5Sensor(uint8_t pinA, uint8_t pinD);
    float getGas();
    bool isDetect();
};
#endif
