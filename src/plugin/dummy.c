

#include "plugin.h"


int get_data(can_data_t *data) {
  static uint32_t id = 0;
  data->id = id;
  return EXIT_SUCCESS;
}