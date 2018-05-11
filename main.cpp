/**
 ******************************************************************************
 * @file    main.cpp
 * @author  CLab
 * @version V1.0.0
 * @date    5-September-2017
 * @brief   Simple Example application for using X_NUCLEO_IKS01A2  
 *          MEMS Inertial & Environmental Sensor Nucleo expansion and 
 *          B-L475E-IOT01A2 boards.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
*/ 

/* Includes */
#include "mbed.h"
#include "HTS221Sensor.h"
#include "LPS22HBSensor.h"
#include "LSM6DSLSensor.h"

#include "lis3mdl_class.h"
#include "VL53L0X.h"



/* Retrieve the composing elements of the expansion board */

/* Interface definition */
static DevI2C devI2c(PB_11,PB_10);

/* Environmental sensors */
static LPS22HBSensor press_temp(&devI2c);
static HTS221Sensor hum_temp(&devI2c);

/* Motion sensors */
static LSM6DSLSensor acc_gyro(&devI2c,LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW,PD_11); // low address
static LIS3MDL magnetometer(&devI2c);

/* Range sensor - B-L475E-IOT01A2 only */
static DigitalOut shutdown_pin(PC_6);
static VL53L0X range(&devI2c, &shutdown_pin, PC_7);


/* Simple main function */
int main() {
  uint8_t id;
  float value1, value2;
//  char buffer1[32], buffer2[32];
  int32_t axes[3];
  
  /* Init all sensors with default params */
  hum_temp.init(NULL);
  press_temp.init(NULL);
  magnetometer.init(NULL);
  acc_gyro.init(NULL);
  range.init_sensor(VL53L0X_DEFAULT_ADDRESS);

  /* Enable all sensors */
  hum_temp.enable();
  press_temp.enable();
  acc_gyro.enable_x();
  acc_gyro.enable_g();
  
  printf("\033[2J\033[20A");
  printf ("\r\n--- Starting new run ---\r\n\r\n");

  hum_temp.read_id(&id);
  printf("HTS221  humidity & temperature    = 0x%X\r\n", id);
  press_temp.read_id(&id);
  printf("LPS22HB pressure & temperature    = 0x%X\r\n", id);
  magnetometer.read_id(&id);
  printf("LIS3MDL magnetometer              = 0x%X\r\n", id);
  acc_gyro.read_id(&id);
  printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);
  
  printf("\n\r--- Reading sensor values ---\n\r"); ;
 
  while(1) {
    printf("\r\n");

    hum_temp.get_temperature(&value1);
    hum_temp.get_humidity(&value2);
    printf("HTS221:  [temp] %.2f C, [hum]   %.2f%%\r\n", value1, value2);   
    value1=value2=0;    
    press_temp.get_temperature(&value1);
    press_temp.get_pressure(&value2);
    printf("LPS22HB: [temp] %.2f C, [press] %.2f mbar\r\n", value1, value2);

    printf("---\r\n");

    magnetometer.get_m_axes(axes);
    printf("LIS3MDL [mag/mgauss]:    %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);
    acc_gyro.get_x_axes(axes);
    printf("LSM6DSL [acc/mg]:        %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);
    acc_gyro.get_g_axes(axes);
    printf("LSM6DSL [gyro/mdps]:     %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);

    uint32_t distance;
    int status = range.get_distance(&distance);
    if (status == VL53L0X_ERROR_NONE) {
        printf("VL53L0X [mm]:            %6ld\r\n", distance);
    } else {
        printf("VL53L0X [mm]:                --\r\n");
    }
  
    printf("\033[8A");

    wait(0.5);
  }
}
