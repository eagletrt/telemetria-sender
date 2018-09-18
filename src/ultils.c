#include "utils.h"

void print_buffer(const uint8_t *buf, size_t buflen) {
  size_t i;
  for (i = 0; i < buflen; i++) {
    printf("%02x", buf[i]);
    if ((i + 5) % 4 == 0) printf(" ");
  }
  printf("\n");
}