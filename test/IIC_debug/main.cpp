 // --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    https://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
// 
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//
 
#include <Arduino.h>
#include <Wire.h>


// 第一条 IIC 总线 Wire 的引脚
#define IIC_SDA 4
#define IIC_SCL 5

void setup()
{

    bool flag;

    
    // 需要指定 SDA 和 SCL 引脚
    flag = Wire.begin(IIC_SDA, IIC_SCL);
    printf("I2C Scanner");

    if (flag == false)
    {
        printf("Wire.begin() failed");
    }
    else
    {
        printf("Wire.begin() success");
    }

    
}
 
 
void loop()
{
  byte error, address;
  int nDevices;
 
  printf("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      printf("I2C device found at address 0x");
      if (address<16) 
        printf("0");
      printf("%x",address);
      printf("  !");
 
      nDevices++;
    }
    else if (error==4) 
    {
      printf("Unknown error at address 0x");
      if (address<16) 
        printf("0");
      printf("%x",address);
    }    
  }
  if (nDevices == 0)
    printf("No I2C devices found\n");
  else
    printf("done\n");
 
  delay(5000);           // wait 5 seconds for next scan
}
// //
// //    FILE: INA226_calibrate.ino
// //  AUTHOR: Prashant Kumar
// // PURPOSE: accurate calibration of shunt resistance, current zero offset, bus voltage scaling and full user control
// //     url: https://github.com/pk17r/INA226


// #include "INA226.h"


// INA226 INA(0x40);


// void setup()
// {
//   Serial.begin(115200);
//   delay(100);
//   printf(__FILE__);
//   printf("INA226_LIB_VERSION: ");
//   printf(INA226_LIB_VERSION);

//   Wire.begin();
//   if (!INA.begin() )
//   {
//     printf("could not connect. Fix and Reboot");
//   }

//   /* STEPS TO CALIBRATE INA226
//    * 1. Set shunt equal to shunt resistance in ohms. This is the shunt resistance between IN+ and IN- pins of INA226 in your setup.
//    * 2. Set current_LSB_mA (Current Least Significant Bit mA) equal to your desired least count resolution for IOUT in milli amps. Expected value to be in multiples of 0.050 o 0.010. Recommended values: 0.050, 0.100, 0.250, 0.500, 1, 2, 2.5 (in milli Ampere units).
//    * 3. Set current_zero_offset_mA = 0, bus_V_scaling_e4 = 10000.
//    * 4. Build firmware and flash microcontroller.
//    * 5. Attach a power supply with voltage 5-10V to INA226 on VBUS/IN+ and GND pins, without any load.
//    * 6. Start Serial Monitor and note Current values. Update current_zero_offset_mA = current_zero_offset_mA + average of 10 Current values in milli Amperes.
//    * NOTE: Following adjustments shouldn't change values by more than 15-20%.
//    * 7. Now measure Bus Voltage using a reliable Digital MultiMeter (DMM). Update bus_V_scaling_e4 = bus_V_scaling_e4 / (Displayed Bus Voltage on Serial Monitor) * (DMM Measured Bus Voltage). Can only be whole numbers.
//    * 8. Now set DMM in current measurement mode. Use a resistor that will generate around 50-100mA IOUT measurement between IN- and GND pins with DMM in series with load. Note current measured on DMM.
//    * 9. Update shunt = shunt * (Displayed IOUT on Serial Monitor) / (DMM Measured IOUT).
//    * 10. Build firmware and flash microcontroller. Your INA 226 is now calibrated. It should have less than 1% error in Current and Voltage measurements over a wide range like [5mA, 1A] and [5V, 20V].
//    */

//   /* USER SET VALUES */

//   float shunt = 0.020;                      /* shunt (Shunt Resistance in Ohms). Lower shunt gives higher accuracy but lower current measurement range. Recommended value 0.020 Ohm. Min 0.001 Ohm */
//   float current_LSB_mA = 0.05;              /* current_LSB_mA (Current Least Significant Bit in milli Amperes). Recommended values: 0.050, 0.100, 0.250, 0.500, 1, 2, 2.5 (in milli Ampere units) */
//   float current_zero_offset_mA = 0;         /* current_zero_offset_mA (Current Zero Offset in milli Amperes, default = 0) */
//   uint16_t bus_V_scaling_e4 = 10000;        /* bus_V_scaling_e4 (Bus Voltage Scaling Factor, default = 10000) */

//   if(INA.configure(shunt, current_LSB_mA, current_zero_offset_mA, bus_V_scaling_e4))
//     printf("\n***** Configuration Error! Chosen values outside range *****\n");
//   else
//     printf("\n***** INA 226 CONFIGURE *****");
//   printf("Shunt:\t");
//   printf(shunt, 4);
//   printf(" Ohm");
//   printf("current_LSB_mA:\t");
//   printf(current_LSB_mA * 1e+3, 1);
//   printf(" uA / bit");
//   printf("\nMax Measurable Current:\t");
//   printf(INA.getMaxCurrent(), 3);
//   printf(" A");


//   /* CALIBRATION */

//   float bv = 0, cu = 0;
//   for (int i = 0; i < 10; i++) {
//     bv += INA.getBusVoltage();
//     cu += INA.getCurrent_mA();
//     delay(150);
//   }
//   bv /= 10;
//   cu /= 10;
//   printf("\nAverage Bus and Current values for use in Shunt Resistance, Bus Voltage and Current Zero Offset calibration:");
//   bv = 0;
//   for (int i = 0; i < 10; i++) {
//     bv += INA.getBusVoltage();
//     delay(100);
//   }
//   bv /= 10;
//   printf("\nAverage of 10 Bus Voltage values = ");
//   printf(bv, 3);
//   printf("V");
//   cu = 0;
//   for (int i = 0; i < 10; i++) {
//     cu += INA.getCurrent_mA();
//     delay(100);
//   }
//   cu /= 10;
//   printf("Average of 10 Current values = ");
//   printf(cu, 3);
//   printf("mA");

//   printf("\nCALIBRATION VALUES TO USE:\t(DMM = Digital MultiMeter)");
//   printf("Step 5. Attach a power supply with voltage 5-10V to INA226 on VBUS/IN+ and GND pins, without any load.");
//   printf("\tcurrent_zero_offset_mA = ");
//   printf(current_zero_offset_mA + cu, 3);
//   printf("mA");
//   if(cu > 5)
//     printf("********** NOTE: No resistive load needs to be present during current_zero_offset_mA calibration. **********");
//   printf("\tbus_V_scaling_e4 = ");
//   printf(bus_V_scaling_e4);
//   printf(" / ");
//   printf(bv, 3);
//   printf(" * (DMM Measured Bus Voltage)");
//   printf("Step 8. Set DMM in current measurement mode. Use a resistor that will generate around 50-100mA IOUT measurement between IN- and GND pins with DMM in series with load. Note current measured on DMM.");
//   printf("\tshunt = ");
//   printf(shunt);
//   printf(" * ");
//   printf(cu, 3);
//   printf(" / (DMM Measured IOUT)");
//   if(cu < 40)
//     printf("********** NOTE: IOUT needs to be more than 50mA for better shunt resistance calibration. **********");
//   delay(1000);

//   /* MEASUREMENTS */

//   printf("\nBUS(V) SHUNT(mV) CURRENT(mA) POWER(mW)");
//   for (int i = 0; i < 5; i++)
//   {
//     bv = INA.getBusVoltage();
//     float sv = INA.getShuntVoltage_mV();
//     cu = INA.getCurrent_mA();
//     float po = (bv - sv / 1000) * cu;
//     printf(bv, 3);
//     printf("\t");
//     printf(sv, 3);
//     printf("\t\t");
//     printf(cu, 1);
//     printf("\t");
//     printf(po, 1);
//     printf();
//     delay(1000);
//   }
// }

// void loop()
// {

// }

// //  -- END OF FILE --

