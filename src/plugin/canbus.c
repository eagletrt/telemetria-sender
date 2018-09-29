/**********************************************************************
 * canbus.c - acquire CANBus data and fill a can_data_t structure     *
 **********************************************************************/
#include "../libsocketcan.h"
#include "../plugin.h"

int get_data(can_data_t *data) {
  // TO BE IMPLEMENTED
  return EXIT_SUCCESS;
}

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