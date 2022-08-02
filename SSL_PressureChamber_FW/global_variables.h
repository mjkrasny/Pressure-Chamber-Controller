#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H
#include <Arduino.h>

extern char FW_name[];
extern char FW_version[];

extern uint8_t pressure_gauge_pin;
extern uint8_t three_volts_pin;

extern uint8_t inlet_valve;  
extern uint8_t outlet_valve;  
extern uint8_t error_led; 

extern uint8_t  pressure_avg_cnt;
extern uint16_t pressure_val_adc;
extern uint16_t pressure_val;
extern uint16_t atmospheric_adc;

extern uint16_t diastolic;
extern uint16_t systolic;
extern uint16_t dicrotic_notch;
extern uint16_t dicrotic_peak;

extern uint16_t diastolic_adc;       
extern uint16_t systolic_adc;        
extern uint16_t dicrotic_notch_adc;
extern uint16_t dicrotic_peak_adc;  

extern float    meas_3v3;
extern float    calculated_vsup;
extern uint8_t  timeout;
extern uint32_t time_interval_adc;

extern float linear_fit_a;
extern float linear_fit_b;

extern uint8_t start_flag;
extern uint8_t cycle_flag;


#endif
