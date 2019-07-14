/**********************************************************************
 * canbus.c - acquire CANBus data and fill a can_data_t structure     *
 **********************************************************************/
#include "../linux/libsocketcan.h"
#include "../plugin.h"
#include "../can_custom_lib.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int get_data(int* data_gathered, int data_lenght, can_data_t *data) {
  struct timespec time;
  static uint32_t id = 0;

  uint32_t timestamp;

  if (clock_gettime(CLOCK_MONOTONIC, &time) == -1 ) {
    perror("clock gettime");
    return EXIT_FAILURE;
  }

  timestamp = time.tv_sec * 1000 + time.tv_nsec / 1E6;

  data->id = id++;
  data->timestamp = timestamp;

  //initialize values
  for (int i = 0; i < 20; ++i) {
    data->resolver[i] = 0;
    data->throttle[i] = 0; 
    data->brake[i/2] = 0;
    data->front_wheels_encoder[i] = 0.0;
    //data->gps[i] = (gps_tensor *) malloc (sizeof(gps_tensor));
      data->gps[i].latitude = 0.0;
      data->gps[i].longitude = 0.0; 
      data->gps[i].speed = 0.0; 
      data->gps[i].altitude = 0.0; 
      data->gps[i].lat_o = '-';
      data->gps[i].lat_o = '-';
    //data->imu_gyro[i] = (imu_tensor *) malloc (sizeof(imu_tensor));
      data->imu_gyro[i].x = 0.0;
      data->imu_gyro[i].y = 0.0;
      data->imu_gyro[i].z = 0.0;
    //data->imu_axel[i] = (imu_tensor *) malloc (sizeof(imu_tensor));
      data->imu_axel[i].x = 0.0;
      data->imu_axel[i].y = 0.0;
      data->imu_axel[i].z = 0.0;
    data->steering_wheel_encoder[i] = 0.0;
  }
  data->marker = 0;

  //data->bms_hv[0] = (bms_hv *) malloc (sizeof(bms_hv));
    data->bms_hv[0].temp = 0.0;
    data->bms_hv[0].volt = 0.0;

  //data->inv[0] = (inverter *) malloc (sizeof(inverter));
    data->inv[0].temp = 0.0;
    data->inv[0].volt = 0.0;
    data->inv[0].curr = 0.0;

  //data->bms_lv[0] = (bms_lv *) malloc (sizeof(bms_lv));
    data->bms_lv[0].temp = 0.0;

  int countFWE = 0; //
  int countIMG = 0; //
  int countIMA = 0; //
  int countTHR = 0; //
  int countBRK = 0; //
  int countSWE = 0; //
  int countGPS = 0; //
  //int countBHV = 0; //
  //int countBLV = 0; //
  for (int i = 0; i < data_lenght; i = i+3) {

    int id_gathered = data_gathered[i];
    int data1 = data_gathered[i+1];
    int data2 = data_gathered[i+2];
    //check if gps data are complete
    int received7 = 0;
    int received8 = 0;
    
    //printf("%d %d %d\n", i, i+1, i+2);
    int firstByte = ((data1 >> 24) & 255);
    switch(id_gathered){
      case(0xAA):
        if (firstByte == 0x01) {
        //volt                                  
          data->bms_hv[0].volt = data1 & 16777215; //0xFFFFFF
        } else if (firstByte == 0x0A) {
        //temp
          data->bms_hv[0].temp = (data1>>8) & 65535; //0xFFFF
        }
      break;
      case(0xB0):
        //pedals
        if (firstByte == 0x01 && countTHR < 20) {
        	//throttle
        	data->throttle[countTHR++] = ((data1 >> 16) & 255);
        } else if (firstByte == 0x02 && countBRK < 10) {
        	//brake
        	data->brake[countBRK++] = ((data1 >> 16) & 255);
        }
      break;
      case (0xC0):
        //swe, imu gyro and axel
        if (firstByte == 0x04) {
          //imu gyro
          data->imu_gyro[countIMG].x = ((data1>>16)&255)*256+((data1>>8)&255);
          data->imu_gyro[countIMG].y = (data1&255)*256+((data1>>24)&255);
          data->imu_gyro[countIMG++].z = ((data2>>16)&255)*256+((data2>>8)&255);
        } else if (firstByte == 0x05) {
          //imu axel
          data->imu_axel[countIMA].x = ((data1>>16)&255)*256+((data1>>8)&255);
          data->imu_axel[countIMA].y = (data1&255)*256+((data1>>24)&255);
          data->imu_axel[countIMA++].z = ((data2>>16)&255)*256+((data2>>8)&255);
        } else if (firstByte == 0x02) {
          //swe
          data->steering_wheel_encoder[countSWE++] = ((data1>>16) & 255);
        }
      break;
      case (0xD0):
      //gps and fwe
      if (firstByte == 0x07) {
        //gps1
        data->gps[countGPS].latitude = (((data1>>16)&255)*256+((data1>>8)&255))*100000+((data1&255)*256+((data2>>24)&255));
        data->gps[countGPS].lat_o = (data2>>16) & 255;
        data->gps[countGPS].speed = (((data2>>8) & 255)*256) + (data2 & 255);
        if (received8 == 1) {
          received8 = 0;
          countGPS++;
        } else {
          received7 = 1;
        }
      } else if (firstByte == 0x08) {
        //gps2
        data->gps[countGPS].longitude = (((data1>>16)&255)*256+((data1>>8)&255))*100000+((data1&255)*256+((data2>>24)&255));
        data->gps[countGPS].lon_o = (data2>>16) & 255;
        data->gps[countGPS].altitude = (((data2>>8) & 255)*256) + (data2 & 255);
        if (received7 == 1) {
          received7 = 0;
          countGPS++;
        } else {
          received8 = 1;
        }
      } else if (firstByte == 0x06) {
        //fwe
        data->front_wheels_encoder[countFWE++] = ((data1>>16)&255)*256+((data1>>8)&255);
      }
      break;
      case (0xFF):
        if (firstByte == 0x01) {
          data->bms_lv[0].temp = ((data1 & 255)<<8) + ((data2>>24)&255);
        }
      break;
      case (0xAB):
        data->marker = 1;
      break;
    }
  }  
  return EXIT_SUCCESS;
}
