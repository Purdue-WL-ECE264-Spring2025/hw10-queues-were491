#include "queue.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  // struct linked_list test;
  // insert_at_head(&test, 1);
  // insert_at_head(&test, 2);
  // insert_at_head(&test, 3);
  // insert_at_tail(&test, 4);
  // insert_at_tail(&test, 5);
  // insert_at_tail(&test, 6);
  // printf("%ld\n", remove_from_head(&test));
  // printf("%ld\n", remove_from_tail(&test));
  // insert_at_head(&test, 62);
  // printf("%ld\n", remove_from_head(&test));
  // dump_list(stdout, test);
  // free_list(test);
  // return 0;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [test file]\n", argv[0]);
    exit(1);
  }

  FILE *fp = fopen(argv[1], "r");
  if (!fp) {
    fprintf(stderr, "Failed to open input file %s\n", argv[1]);
    exit(2);
  }

  int expected;
  fscanf(fp, "%d", &expected);

  struct game_state start;
  start.num_steps = 0;
  for (uint8_t i = 0; i < 4; i++) {
    for (uint8_t j = 0; j < 4; j++) {
      int value;
      fscanf(fp, "%d", &value);
      start.tiles[i][j] = value;
      if (value == 0) {
        start.empty_row = i;
        start.empty_col = j;
      }
    }
  }
  fclose(fp);

  int output = number_of_moves(start);
  if (output != expected) {
    fprintf(stderr, "Expected %d moves, got %d\n", expected, output);
    exit(3);
  }

  return 0;
}
