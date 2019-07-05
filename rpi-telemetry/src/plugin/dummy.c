
/**********************************************************************
 * dummy.c - generate data as if coming from a dummy CANbus interface *
 **********************************************************************/
#include "../plugin.h"
#include "../utils.h"
#include "../version.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

static double rnd(double min, double max) {
  return (max - min) * (RAND_MAX - rand()) / (double)RAND_MAX + min;
}

int get_data(int* data_gathered, int data_lenght,can_data_t *data) {
  struct timespec time;
  static uint32_t id = 0;
  int i = 0;
  uint32_t timestamp;
  if( clock_gettime(CLOCK_MONOTONIC, &time) == -1 ) {
    perror("clock gettime");
    return EXIT_FAILURE;
  }

  timestamp = time.tv_sec * 1000 + time.tv_nsec / 1E6;

  data->id = id++;
  data->timestamp = timestamp;
  
  for (int i = 0; i < 20; ++i) {
    data->resolver[i] = 0.0 + rnd(0, 50);
    data->front_wheels_encoder[i] = 0.0 + rnd(0, 100);
    data->imu[i] = (imu_tensor *) malloc (sizeof(imu_tensor) * 20);
      data->imu[i]->x = 0.0 + rnd(-0.5, 0.5);
      data->imu[i]->y = 0.0 + rnd(-0.5, 0.5);
      data->imu[i]->z = 0.0 + rnd(-0.5, 0.5);
    data->throttle[i] = 0 + rnd(0, 100);
    data->brake[i%2] = 0 + rnd(0, 100);
    data->steering_wheel_encoder[i] = 0.0 + rnd(0, 50);

    data->gps[i] = (gps_tensor *) malloc (sizeof(gps_tensor) * 20);
      data->gps[i]->latitude = 0.0 + rnd(-0.5, 0.5);
      data->gps[i]->longitude = 0.0 + rnd(-0.5, 0.5);
      data->gps[i]->speed = 0.0 + rnd(-0.5, 0.5);
  }

  data->bms_hv[0] = (bms_hv *) malloc (sizeof(bms_hv));
    data->bms_hv[0]->temp = 0.0 + rnd(-0.5, 0.5);
    data->bms_hv[0]->volt = 0.0 + rnd(-0.5, 0.5);

  data->inv[0] = (inverter *) malloc (sizeof(inverter));
    data->inv[0]->temp = 0.0 + rnd(-0.5, 0.5);
    data->inv[0]->volt = 0.0 + rnd(-0.5, 0.5);
    data->inv[0]->curr = 0.0 + rnd(-0.5, 0.5);

  data->bms_lv[0] = (bms_lv *) malloc (sizeof(bms_lv));
    data->bms_lv[0]->temp = 0.0 + rnd(-0.5, 0.5);

  return EXIT_SUCCESS;
}


#if 0
int main(int argc, char const *argv[]) {
  bson_t *doc;
  can_data_t can_data = {0};
  printf("Plugin %s version %s\n", argv[0], GIT_COMMIT_HASH);
  get_data(&can_data);
  can_data_to_bson(&can_data, &doc, argv[0]);
  printf("Document:\n%s\n", bson_as_json(doc, NULL));
  printf("Raw data (%d bytes):\n", doc->len);
  print_buffer(stdout, bson_get_data(doc), doc->len);
  return 0;
}
#endif
