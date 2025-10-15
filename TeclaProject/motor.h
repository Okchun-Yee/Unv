#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

void init_gpio();
void forward();
void turn_left();
void turn_right();
void stop();
void cleanup();

#endif
