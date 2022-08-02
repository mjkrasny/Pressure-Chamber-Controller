#ifndef CHAMBER_FUNC_H
#define CHAMBER_FUNC_H
#include <Arduino.h>


uint16_t read_analogin_avg_adc(uint16_t analog_input, uint16_t count_number);
float calculate_supply_v(float meas_3v3);

uint16_t pressure_to_adc(uint16_t pressure);
uint16_t adc_to_pressure(uint16_t adc_val);

void increase_pressure();
void release_pressure();

void recalculate_control_params();


//uint16_t read_3v3_avg(uint8_t avg_cnt);
#endif
