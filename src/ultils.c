#include "utils.h"

void print_buffer(FILE * stream, const uint8_t *buf, size_t buflen) {
  size_t i, c = 1;
  fprintf(stream, "%04d: ", 0);
  for (i = 0; i < buflen; i++) {
    fprintf(stream, "%02x", buf[i]);
    if ((i + 5) % 4 == 0) { 
      fprintf(stream, " ");
      if ((c++ % 8) == 0) fprintf(stream, "\n%04lu: ", i+1); 
    }
  }
  fprintf(stream, "\n");
}