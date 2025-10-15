// motor_control.c

#include <stdio.h>
#include <pigpio.h>
#include <pthread.h>
#include <unistd.h>
#include "motor.h"

#define FORWARD_STEPS  512

int left_pins[4] = { 17, 27, 22, 25};   // 왼쪽 모터 (예시)
int right_pins[4] = {20, 16, 5, 6 };   // 오른쪽 모터

int seq[8][4] = {
    {1, 0, 0, 1}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0},
    {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}
};

int reverse_seq[8][4] = {
    {0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 1, 1, 0},
    {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 1}
};

void init_gpio() {
    gpioInitialise();
    for (int i = 0; i < 4; i++) {
        gpioSetMode(left_pins[i], PI_OUTPUT);
        gpioSetMode(right_pins[i], PI_OUTPUT);
    }
}

void cleanup() {
    for (int i = 0; i < 4; i++) {
        gpioWrite(left_pins[i], 0);
        gpioWrite(right_pins[i], 0);
    }
    gpioTerminate();
}

void* stepMotor(void* arg) {
    int* pins = (int*)arg;
    for (int i = 0; i < FORWARD_STEPS; i++) {  // 반바퀴
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 4; k++) {
                gpioWrite(pins[k], seq[j][k]);
            }
            gpioDelay(1000);
        }
    }
    for (int i = 0; i < 4; i++) gpioWrite(pins[i], 0);
    return NULL;
}

void* stepMotorReverse(void* arg) {
    int* pins = (int*)arg;
    for (int i = 0; i < FORWARD_STEPS; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 4; k++) {
                gpioWrite(pins[k], reverse_seq[j][k]);
            }
            gpioDelay(1000);
        }
    }
    for (int i = 0; i < 4; i++) gpioWrite(pins[i], 0);
    return NULL;
}

void forward() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, stepMotorReverse, left_pins);           // 그대로 회전
    pthread_create(&t2, NULL, stepMotor, right_pins);                 // 반대 회전
    pthread_join(t1, NULL); pthread_join(t2, NULL);
}

void turn_left() {
    printf("Turn Left!\n");
    pthread_t t1, t2;
    pthread_create(&t1, NULL, stepMotorReverse, right_pins);
    pthread_create(&t2, NULL, stepMotorReverse, left_pins);
    pthread_join(t1, NULL); pthread_join(t2, NULL);
}

void turn_right() {
    printf("Turn Right!\n");
    pthread_t t1,t2;
    pthread_create(&t1, NULL, stepMotor, left_pins);
    pthread_create(&t2, NULL, stepMotor, right_pins);
    pthread_join(t1, NULL); pthread_join(t2, NULL);
}

void stop() {
    for (int i = 0; i < 4; i++) {
        gpioWrite(left_pins[i], 0);
        gpioWrite(right_pins[i], 0);
    }
}
