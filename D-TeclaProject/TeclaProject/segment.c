#include <pigpio.h>
#include <string.h>
#include "segment.h"

int segmentPins[] = { 21, 26, 19, 13, 12, 14, 15 };
int digit8[7];
#define DECODER_A 16
#define DECODER_B 20
#define DECODER_C 21

void initSegment() {
    for (int i = 0; i < 7; i++) gpioSetMode(segmentPins[i], PI_OUTPUT);
    gpioSetMode(DECODER_A, PI_OUTPUT);
    gpioSetMode(DECODER_B, PI_OUTPUT);
    gpioSetMode(DECODER_C, PI_OUTPUT);
}

void selectDigit(int index) {
    gpioWrite(DECODER_A, index & 0x01);
    gpioWrite(DECODER_B, (index >> 1) & 0x01);
    gpioWrite(DECODER_C, (index >> 2) & 0x01);
}

void displayDigitAt(int digit[], int index) {
    selectDigit(index);
    for (int i = 0; i < 7; i++) gpioWrite(segmentPins[i], digit[i]);
}

void usePinCheck(int num) {
    if (num >= 10) num = 9;
    const int temp[10][7] = {
        {0,1,1,1,1,1,1},{0,0,1,0,1,0,0},{1,1,0,1,1,0,1},
        {1,1,0,1,0,1,1},{1,0,1,1,0,1,0},{1,1,1,0,0,1,1},
        {1,1,1,0,1,1,1},{0,1,1,1,0,1,0},{1,1,1,1,1,1,1},{1,1,1,1,0,1,1}
    };
    memcpy(digit8, temp[num], sizeof(digit8));
}
