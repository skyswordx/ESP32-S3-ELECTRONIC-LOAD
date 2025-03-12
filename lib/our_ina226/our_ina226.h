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

class ina226_handle_t
{
    public:
        INA226 ina_device(0x40);

        ina226_data_t ina226_data;
        
        float bus_voltage;
        float shunt_voltage_mV;
        float current_mA;
        float power_mW;

        

        ina226_handle_t();
        void update_all_data();
        
};



#endif // OUR_INA226