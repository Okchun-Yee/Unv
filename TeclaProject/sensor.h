#ifndef SENSOR_H
#define SENSOR_H

typedef struct {
    int triggerPin;
    int echoPin;
} sonicSensor;

double GetDistance(sonicSensor sensor);
void getAllDistances(double distances[2]);

#endif