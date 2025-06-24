#include <pigpio.h>
#include "sensor.h"
#include <math.h>
#include <stdio.h>

//Double Sonic sensor
double GetDistance(sonicSensor sensor) {
    gpioWrite(sensor.triggerPin, 0);
    gpioDelay(2);
    gpioWrite(sensor.triggerPin, 1);
    gpioDelay(10);
    gpioWrite(sensor.triggerPin, 0);

    uint32_t timeout = gpioTick() + 30000;
    while (gpioRead(sensor.echoPin) == 0 && gpioTick() < timeout);
    uint32_t startTime = gpioTick();
    while (gpioRead(sensor.echoPin) == 1);
    uint32_t endTime = gpioTick();

    return (endTime - startTime) / 58.0;
}

void getAllDistances(double distances[2]) {
    sonicSensor singleSensor = { 24, 23 };
    gpioDelay(100000);
    distances[0] = GetDistance(singleSensor);
    distances[1] = 0.0;  // 더 이상 사용하지 않음
}