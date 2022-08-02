#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include <Arduino.h>


enum states_list {RISE_1,
              FALL_1,
              RISE_2,
              FALL_2
             };

extern enum states_list state;
             

void state_machine();


#endif
