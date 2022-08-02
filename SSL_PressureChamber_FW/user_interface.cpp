#include "global_variables.h"
#include "user_interface.h"
#include "chamber_func.h"




String command; // command upcoming from user

void user_command_check()
{
  // this is responsible for user command recognition
  if(Serial.available())
  {
    char c = Serial.read();
    if(c== '\n')
    {
      parseCommand(command);
      command = "";
    }
    else
    {
      command += c;
    }
  }
}



void parseCommand(String com)
{ 
  /*
   * Command is: funcMode,comValue; "," as separator
   * Example:
   *          DIASTOLIC,80 - sets diastoli pressure to 80 mmHg
   */
  
  String funcMode; // Function mode
  String comValue; // Value

  funcMode = com.substring(0, com.indexOf(",") ); // split string when "," sign found
  comValue = com.substring(com.indexOf(",") +1 ); // from comma till end of string

  //############################################################################################
  // Start & Stop pressure cycles
  if(funcMode == "STOP")
  {
    cycle_flag = 0;
    release_pressure();
    Serial.println("STOP");
  }
  else if(funcMode == "START")
  {
    start_flag = 1;
    Serial.println("START");
  }
  else if(funcMode == "EQUALIZE")
  {
    release_pressure();
    Serial.println("EQUALIZE");
  }

  //############################################################################################
  // Pressure pulse parameters
  else if(funcMode == "DIASTOLIC")
  {
    diastolic = comValue.toInt();
    Serial.println("DIASTOLIC set to " + comValue);
    diastolic_adc = pressure_to_adc(diastolic);
    Serial.println("DIASTOLIC ADC: " + String(diastolic_adc));
  }
  else if(funcMode == "SYSTOLIC")
  {
    systolic = comValue.toInt();
    Serial.println("SYSTOLIC set to " + comValue);
    systolic_adc = pressure_to_adc(systolic);
    Serial.println("SYSTOLIC ADC: " + String(systolic_adc));
  }
  else if(funcMode == "DICROTIC NOTCH")
  {
    dicrotic_notch = comValue.toInt();
    Serial.println("DICROTIC NOTCH set to " + comValue);
    dicrotic_notch_adc = pressure_to_adc(dicrotic_notch);
    Serial.println("DICROTIC NOTCH ADC: " + String(dicrotic_notch_adc));
  }
  else if(funcMode == "DICROTIC PEAK")
  {
    dicrotic_peak = comValue.toInt();
    Serial.println("DICROTIC PEAK set to " + comValue);
    dicrotic_peak_adc = pressure_to_adc(dicrotic_peak);
    Serial.println("DICROTIC PEAK ADC: " + String(dicrotic_peak_adc));
  }
  // Show current values of blood pressure parameters
  else if(funcMode == "BP PARAMS")
  {
    Serial.println("DIASTOLIC: "        + String(diastolic));
    Serial.println("SYSTOLIC: "         + String(systolic));
    Serial.println("DICROTIC NOTCH: "   + String(dicrotic_notch));
    Serial.println("DICROTIC PEAK: "    + String(dicrotic_peak));
  }
  
  //############################################################################################
  // Calibration params
  else if(funcMode == "3V3 MEAS")
  {
    meas_3v3 = comValue.toFloat();
    Serial.println("3V3 MEASURED = " + String(meas_3v3));

    calculate_supply_v(meas_3v3);
    Serial.println("V supply calculated = " + String(calculated_vsup));
  }

  else if(funcMode == "PRESSURE AVG CNT")
  {
    pressure_avg_cnt = comValue.toInt();
    Serial.println("PRESSURE AVG CNT set to " + String(pressure_avg_cnt));
  }

  else if(funcMode == "LIN FIT A")
  {
    linear_fit_a = comValue.toInt();
    Serial.println("LIN FIT A set to " + String(linear_fit_a));
  }
  else if(funcMode == "LIN FIT B")
  {
    linear_fit_b = comValue.toInt();
    Serial.println("LIN FIT B set to " + String(linear_fit_b));
  }
  else if(funcMode == "PRESSURE")
  {
    uint16_t cnt = 0;
    cnt = comValue.toInt();

    if (cnt == 0) // when no parameter given or param=0, use pressure_avg_cnt
    {
      pressure_val_adc = read_analogin_avg_adc(pressure_gauge_pin, pressure_avg_cnt);
      pressure_val = adc_to_pressure(pressure_val_adc);
    }
    else
    { 
      pressure_val_adc = read_analogin_avg_adc(pressure_gauge_pin, cnt);
      pressure_val = adc_to_pressure(pressure_val_adc);
    }
    Serial.println("PRESSURE [ADC]" + String(pressure_val_adc));
    Serial.println("PRESSURE [mmHg]" + String(pressure_val));
  }
  else if(funcMode == "ADC INTERVAL")
  {
    time_interval_adc = comValue.toInt();
    Serial.println("ADC TIME INTERVAL set to [us] " + String(time_interval_adc));
  }
  

  // Show current value of all parameters
  else if(funcMode == "PARAMS")
  {
    Serial.println("meas_3v3: "           + String(meas_3v3));
    Serial.println("calculated_vsup: "    + String(calculated_vsup));
    Serial.println("pressure_avg_cnt: "   + String(pressure_avg_cnt));
    Serial.println("timeout: "            + String(timeout));
    Serial.println("time_interval_adc: "  + String(time_interval_adc));
    Serial.println("atmospheric_adc: "    + String(atmospheric_adc));
  }

  //############################################################################################
  // Manual control params
  else if(funcMode == "INCREASE") // introduce yourself
  {
    increase_pressure();
    Serial.println("Incrising pressure");
  }
  else if(funcMode == "RELEASE") // introduce yourself
  {
    release_pressure();
    Serial.println("Releasing pressure");
  }

  
  //############################################################################################
  else if(funcMode == "IDF") // introduce yourself
  {
    Serial.print(FW_name);
    Serial.print(' ');
    Serial.println(FW_version);
  }
  
  //############################################################################################
  // Troubleshooting and errors
  else if(funcMode == "TIMEOUT")
  {
    timeout = comValue.toInt();
    Serial.println("TIMEOUT set to " + String(timeout));
  }
  else if(funcMode == "ERROR") // return or registered errors
  {
    Serial.println("@TODO: ERROR");
  }

  
  else // if command was not recognised
  {
    Serial.println("Wrong command or command not recognised!");
  }
}
