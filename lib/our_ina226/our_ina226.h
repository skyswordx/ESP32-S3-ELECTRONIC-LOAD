#ifndef OUR_INA226
#define OUR_INA226

#include <Arduino.h>
#include <Wire.h>
#include "INA226.h"

typedef struct {
  float measured_current; // 电流值
  float measured_voltage; // 电压值
  float measured_power; // 功率值
}ina226_data_t;


#endif // OUR_INA226