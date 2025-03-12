#include "our_ina226.h"

ina226_handle_t::ina226_handle_t(){
    
    Wire.begin();
    if (!this->ina_device.begin() ){
        printf("could not connect. Fix and Reboot\n");
    }

    this->ina_device.setMaxCurrentShunt(1, 0.002);

}


void ina226_handle_t::update_all_data(){
    this->bus_voltage = this->ina_device.getBusVoltage();
    this->shunt_voltage_mV = this->ina_device.getShuntVoltage_mV();
    this->current_mA = this->ina_device.getCurrent_mA();
    this->power_mW = this->ina_device.getPower_mW();
}