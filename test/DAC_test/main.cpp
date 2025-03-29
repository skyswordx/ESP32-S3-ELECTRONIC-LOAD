
#include <Arduino.h>
#include "Wire.h"

// 第一条 IIC 总线 Wire 的引脚
#define IIC_SDA 4
#define IIC_SCL 5

#include "INA226.h"
INA226 INA226_device(0x40); // INA226 电流传感器

#include "MCP4725.h"
MCP4725 MCP4725_device(0x60); // MCP4725 DAC 芯片
#define WARNING_VOLTAGE 18 // 触发过压保护的电压

int IIC_init_flag = 1; // IIC 初始化标志位
int MCP4725_init_flag = 1; // MCP4725 初始化标志位
void setup(){
  // 初始化 Wire IIC 总线
  if (!Wire.begin(IIC_SDA, IIC_SCL)) {
    printf("IIC init failed");
    IIC_init_flag = 0; // IIC 初始化失败
  }

  /* 初始化 MCP4725 */
  if (!MCP4725_device.begin()) {
    printf("could not connect MCP4725. Fix and Reboot");
    MCP4725_init_flag = 0; // MCP4725 初始化失败
  }
  MCP4725_device.setMaxVoltage(3.2841); // 设置最大输出电压
  MCP4725_device.setVoltage(2.0); // 设置输出电压为 3.3V

}

float measure_v = 0.0;
float measure_value = 0.0;

void loop(){
  // if (IIC_init_flag == 0) {
  //   printf("[IIC_init_flag] IIC init failed");
  // }else{
  //   printf("[success]");
  // }


  // if (MCP4725_init_flag == 0) {
  //   printf("[MCP4725_init_flag] MCP4725 init failed");
  // }else{
  //   printf("[success]");
  // }

  printf("\n[flag] IIC: %d, MCP: %d",IIC_init_flag, MCP4725_init_flag); 
  // float measure_v = MCP4725_device.getVoltage();
  // float measure_value = MCP4725_device.getValue();
  // printf("MCP4725: %.3f V, %.3f", measure_v, measure_value);
  delay(1000);
}