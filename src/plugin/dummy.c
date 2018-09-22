
/**********************************************************************
 * dummy.c - generate data as if coming from a dummy CANbus interface *
 **********************************************************************/
#include "plugin.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

static double rnd(double min, double max) {
  return (max - min) * (RAND_MAX - rand()) / (double)RAND_MAX + min;
}

int get_data(can_data_t *data) {
  struct timespec time;
  static uint32_t id = 0;
  int i = 0;
  uint32_t timestamp;
  if( clock_gettime(CLOCK_MONOTONIC, &time) == -1 ) {
    perror("clock gettime");
    return EXIT_FAILURE;
  }
  srand(time.tv_sec);
  timestamp = time.tv_sec * 1000 + time.tv_nsec / 1E6;

  data->id = id++;
  data->timestamp = timestamp;
  
  data->location.latitude = 10 + rnd(-100, 100);
  data->location.longitude = 10 + rnd(-100, 100);
  data->location.elevation = 193 + rnd(-10, 10);

  data->speed = 0.0 + rnd(0, 50);
  data->odometry = 0.0 + rnd(0, 100);
  data->steering_angle = 0.0 + rnd(-20, 20);
  data->throttle = 0.0 + rnd(0, 1);
  data->brake = 0.0 + rnd(0, 1);
  data->acceleration.x = 0.0 + rnd(-0.5, 0.5);
  data->acceleration.y = 0.0 + rnd(-0.5, 0.5);
  data->acceleration.z = -1.0 + rnd(-0.1, 0.1);
  data->gyro.x = 0.0 + rnd(-0.5, 0.5);
  data->gyro.y = 0.0 + rnd(-0.5, 0.5);
  data->gyro.z = 0.0 + rnd(-0.1, 0.1);
  data->magneto.x = 0.0 + rnd(-3, 3);
  data->magneto.y = 0.0 + rnd(-3, 3);
  data->magneto.z = 0.0 + rnd(-3, 3);

  data->accumulator.voltage = 400 + rnd(-100, 0);
  data->accumulator.current[0] = 1.0 + rnd(-0.1, 0.1);
  data->accumulator.current[1] = 1.1 + rnd(-0.1, 0.1);
  data->accumulator.current[2] = 0.9 + rnd(-0.1, 0.1);
  for (i = 0; i < ACCUMULATOR_MODULES; i++) {
    data->accumulator.temperature[i] = 37 + rnd(0, 10);
  }
  return EXIT_SUCCESS;
}