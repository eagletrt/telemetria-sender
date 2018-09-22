#include <stdlib.h>
#include <stdint.h>

#define ACCUMULATOR_MODULES 12

typedef struct {
  uint32_t id;
  uint32_t timestamp;
  struct {
    uint32_t latitude, longitude;
    double elevation;
  } location;
  double speed;
  double odometry;
  double steering_angle;
  double brake;
  double throttle;
  struct {
    double x, y, z;
  } acceleration;
  struct {
    double x, y, z;
  } gyro;
  struct {
    double x, y, z;
  } magneto;
  struct {
    double voltage;
    double current[3];
    double temperature[ACCUMULATOR_MODULES];
  } accumulator;
} can_data_t;