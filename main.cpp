/* Includes */
#include "mbed.h"
// #include "HTS221Sensor.h"
// #include "LPS22HBSensor.h"
// #include "LSM6DSLSensor.h"

// #include "lis3mdl_class.h"
// #include "VL53L0X.h"
#include "SensorQueue.hpp"



// /* Interface definition */
// static DevI2C devI2c(PB_11,PB_10);
// /* Motion sensors */
// static LSM6DSLSensor acc_gyro(&devI2c,LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW,PD_11); // low address



/* Simple main function */
int main() {

  printf("test start\r\n");
  SensorQueue_Test();
  printf("test finished\r\n");
  exit(0);
  // uint8_t id;
  // float value1, value2;
  // int32_t axes[3];
  
  // /* Init all sensors with default params */
  // acc_gyro.init(NULL);

  // /* Enable all sensors */
  // acc_gyro.enable_x();
  // acc_gyro.enable_g();
  
  // printf("\033[2J\033[20A");
  // printf ("\r\n--- Starting new run ---\r\n\r\n");


  // printf("LIS3MDL magnetometer              = 0x%X\r\n", id);
  // acc_gyro.read_id(&id);
  // printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);
  
  // printf("\n\r--- Reading sensor values ---\n\r"); ;
 
  // while(1) {
  //   printf("\r\n");

  //   acc_gyro.get_x_axes(axes);
  //   printf("LSM6DSL [acc/mg]:        %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);
  //   acc_gyro.get_g_axes(axes);
  //   printf("LSM6DSL [gyro/mdps]:     %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);

  //   printf("\033[8A");

  //   wait(0.5);
  // }
}
