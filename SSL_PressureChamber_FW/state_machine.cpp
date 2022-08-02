#include "state_machine.h"

#include "global_variables.h"
#include "chamber_func.h"

#include <Arduino.h>


enum states_list state = RISE_1;
             


void state_machine()
{
  switch (state) 
      {
        case RISE_1:    // state 0 : RISE 1
          increase_pressure();
          
          if(pressure_val_adc <= systolic_adc)
            state = RISE_1;  // go to state = 0;
          else
            state = FALL_1;  //  go to state = 1;
          break;
          
        case FALL_1:    // state 1 : FALL 1
          release_pressure();
          
          if(pressure_val_adc <= dicrotic_notch_adc)
            state = RISE_2; // go to state 2
          else
            state = FALL_1; 
          break;
        
        case RISE_2:    // state 2: RISE 2
          increase_pressure();
          
          if(pressure_val_adc >= dicrotic_peak_adc)
            state = FALL_2; // go to state 3
          else
            state = RISE_2;
          break;
        
        case FALL_2:    // state 3: FALL 2
          release_pressure();
          
          if(pressure_val_adc <= diastolic_adc)
            state = RISE_1; // return to the beginning, state 0
          else
            state = FALL_2;
          break;
       }
}
