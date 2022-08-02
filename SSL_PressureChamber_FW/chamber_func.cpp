#include "global_variables.h"
#include "chamber_func.h"
#include <Arduino.h>

float calculate_supply_v(float meas_3v3)
{
  uint16_t avg_cnt       = 100;
  uint16_t avg_3v3_adc  = read_analogin_avg_adc(three_volts_pin,avg_cnt);

  Serial.println("avg_3v3_adc: " + String(avg_3v3_adc));
  calculated_vsup = (1023*meas_3v3)/ avg_3v3_adc;

  Serial.println("calculated supply voltage: " + String(calculated_vsup));
}


uint16_t pressure_to_adc(uint16_t pressure)
{
  // Use linear fit from calibration to calculate ADC values corresponding to pressure given in mmHg
  // To get linear fit equation calibration against external reader is required.
  // linear fit follows function: y[ADC] = a*x + b, 
  // where
  // x - pressure in mmHg
  // a - linear_fit_a parameter
  // b - linear_fit_b parameter
  uint16_t adc_val = 0;
  
  adc_val = round((linear_fit_a * pressure) + linear_fit_b);
  Serial.println(adc_val);

  return adc_val;
}


uint16_t adc_to_pressure(uint16_t adc_val)
{
  // Uses linear fit from calibration to calculate pressure [mmHg] values corresponding to pressure given in ADC
  // To get linear fit equation calibration against external reader is required.
  // linear fit follows function: y[ADC] = a*x + b, 
  // where
  // x - pressure in mmHg
  // a - linear_fit_a parameter
  // b - linear_fit_b parameter
  //
  // Thus x = (y-b)/a
  uint16_t pressure_val = 0;
  
  pressure_val = round((adc_val - linear_fit_b) / linear_fit_a);
  Serial.println(pressure_val);

  return pressure_val;
}


uint16_t read_analogin_avg_adc(uint16_t analog_input, uint16_t count_number)
{
  uint32_t accumulate_val = 0;
  float avg_analogin_val  = 0;
  
  for (uint16_t i = 0; i < count_number; i++)
  {
    accumulate_val += analogRead(analog_input);
    //Serial.println("accumulated value: " + String(accumulate_val));
    delay (1); // time in ms
  }

  avg_analogin_val = round(accumulate_val / count_number);
  
  return avg_analogin_val;
}


void recalculate_control_params()
{
  diastolic_adc = pressure_to_adc(diastolic);
  systolic_adc = pressure_to_adc(systolic);
  dicrotic_notch_adc = pressure_to_adc(dicrotic_notch);
  dicrotic_peak_adc = pressure_to_adc(dicrotic_peak);
}




//############################################################################################
// Pressure control params
void increase_pressure()
{
  digitalWrite(inlet_valve, HIGH);
  digitalWrite(outlet_valve, LOW);
}

void release_pressure()
{
  digitalWrite(inlet_valve, LOW);
  digitalWrite(outlet_valve, HIGH);
}







//uint16_t read_3v3_avg(uint16_t avg_cnt) // #TODO: obsolete, replace
//{
//  uint32_t accumulate_val = 0;
//  float avg_3v3_adc       = 0;
//  
//  for (uint16_t i = 0; i < avg_cnt; i++)
//  {
//    accumulate_val += analogRead(three_volts_pin);
//    Serial.println("accumulated value: " + String(accumulate_val));
//    delay (1);
//  }
//
//  avg_3v3_adc = round(accumulate_val / avg_cnt);
//  
//  return avg_3v3_adc;
//}
