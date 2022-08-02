/*
 * SSL Pressure Chamber - Arduino Control Code
 * 
 * © COPYRIGHT Smart Sensors Lab 2022
 * 
 * AUTHORs: Marcin J. Kraśny (m.j.krasny@nuigalway.ie)
 *          Muhhamad Farooq (muhammad.farooq@nuigalway.ie)         
 *          Muhammad Riaz ur Rehman (Riaz.Rehman@nuigalway.ie)
 *            
 *          
 * Smart Sensors Lab
 * The Lambe Institute for Translational Medicine
 * School of Medicine
 * College of Medicine, Nursing and Health Sciences
 * National University of Ireland, Galway
 * 
 *  
 * HARDWARE: Arduino Nano Every (Atmega 4809)
 * 
 * LICENSE          MIT
 * 
 * 
 * 
 * 
 * 
 * 
 * DESCRIPTION 
 * 
 * 
 * SOURCE URL       #TODO
 * 
 */
 
#include "global_variables.h"
#include "user_interface.h"
#include "chamber_func.h"
#include "state_machine.h"


char FW_name[] = "Smart Sensors Lab Pressure Chamber Controller";
char FW_version[] = "ver.1.00.08";


// Hardware connection
uint8_t three_volts_pin     = A0; // input pin for 3V3 reference voltage
uint8_t pressure_gauge_pin  = A3; // input pin for the chamber pressure gauge (PSE543-N01)

uint8_t inlet_valve         = 3;  // valve controling pressure inlet to the chamber
uint8_t outlet_valve        = 2;  // valve controling pressure release from the chamber
uint8_t error_led           = 13; // built-in arduino LED


// User accessible variables
uint8_t pressure_avg_cnt    = 2;  // set sample count for averaging pressure gauge read
uint16_t diastolic          = 80;
uint16_t systolic           = 120;
uint16_t dicrotic_notch     = 90;
uint16_t dicrotic_peak      = 100;

float meas_3v3              = 3.3; // 3V3 pin measured with external multimeter; required for analog in calibration
uint8_t timeout             = 10;  // time in seconds [s]; when no valve change in [timeout] defined time error is reported
uint32_t time_interval_adc  = 2000; // time interval in us (1E6 = 1s); set time between analog input measurements; #TODO: investigate, as shortest possible time 1000us

// Internal variables
uint16_t pressure_val_adc   = 0;  // Pressure [ADC] value coming from the chamber pressure sensor 
uint16_t pressure_val       = 0;  // Pressure [mmHg] value coming from the chamber pressure sensor
uint16_t atmospheric_adc    = 0;  // Atmospheric pressure [ADC] value coming from the chamber pressure sensor (when chamber is equalised); this corresponds to 0 mmHg value

// factors calculated for and from calibration data
float calculated_vsup       = 4.782;
float linear_fit_a          = 0.640;
float linear_fit_b          = 705.6;
// pressure corresponding adc values used for valves control 
uint16_t diastolic_adc      = 757; 
uint16_t systolic_adc       = 782;
uint16_t dicrotic_notch_adc = 763;
uint16_t dicrotic_peak_adc  = 770;


// flags
uint8_t timeout_flag        = 0;
uint8_t start_flag          = 0;
uint8_t cycle_flag          = 0;

// timestamps
uint32_t start_timestmp  = 0;
uint32_t stop_timestmp   = 0;
uint32_t delta_time      = 0;

uint8_t cnt = 0;
uint32_t accumulate_val = 0;
float avg_analogin_val  = 0;


void setup() 
{ 
  pinMode(three_volts_pin, INPUT); 
  pinMode(pressure_gauge_pin, INPUT); 
  
  pinMode(inlet_valve, OUTPUT); 
  pinMode(outlet_valve, OUTPUT);
  pinMode(error_led, OUTPUT);

  analogReference(INTERNAL4V3);
  analogRead(three_volts_pin);

  Serial.begin(115200);
}

void loop() 
{
  start_timestmp = micros();
  
  user_command_check();

  if (start_flag == 1)
  {
    start_flag = 0;
    Serial.println("Starting up...");
    release_pressure();
    delay(5000);
    recalculate_control_params();
    delay(1000);
    pressure_val_adc = read_analogin_avg_adc(pressure_gauge_pin, pressure_avg_cnt);
    
    cycle_flag = 1;
  }

  if (cycle_flag == 1)
  {
    // Serial.println("cycling...");
    //pressure_val_adc = read_analogin_avg_adc(pressure_gauge_pin, pressure_avg_cnt);
    
    
    delta_time = start_timestmp - stop_timestmp;
    
    if (delta_time >= time_interval_adc)
    {
      stop_timestmp = micros();
      
      if (cnt < pressure_avg_cnt )
      {
        accumulate_val += analogRead(pressure_gauge_pin);
        cnt+=1;
      }
      else
      {
        pressure_val_adc = round(accumulate_val / cnt);
        //Serial.println("cnt: " + String(cnt));
        //Serial.println("avg_cnt: " + String(pressure_avg_cnt));
        //Serial.println("pressure: " + String(pressure_val_adc));
        accumulate_val = 0;
        cnt=0;
      }
      
      //Serial.println("pressure: " + String(pressure_val_adc));  
      //Serial.print(delta_time);
      //Serial.print(";");
      Serial.println(pressure_val_adc);
      
      state_machine();
    }
  }
}
