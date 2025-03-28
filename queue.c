#include "queue.h"
#include "tile_game.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>

void enqueue(struct queue *q, struct game_state state)
{
    assert(q != NULL);
    insert_at_head(&(q->data), serialize(state));
}

struct game_state dequeue(struct queue *q)
{
    assert(q != NULL);
    return deserialize(remove_from_tail(&(q->data)));
}

// Anything to prevent a time-out... well, while still being forced to use BFS
// head is a dummy, first real element is head->next
static void lln_push(struct list_node *head, size_t val) {
  struct list_node *first = malloc(sizeof(struct list_node));
  first->value = val;
  first->next = head->next;
  head->next = first;
}

static size_t lln_pop(struct list_node *head) {
  assert(head != NULL);

  size_t ret_val;
  struct list_node *last;

  if (head->next == NULL) {
    errno = EINVAL;
    return 0;
  }

  do {
    last = head;
    head = head->next;
  } while (head->next != NULL);

  last->next = NULL;

  ret_val = head->value;
  free(head);

  return ret_val;
}

static void lln_free(struct list_node *head) {
  struct list_node *last;
  while (head != NULL) {
    last = head;
    head = head->next;
    free(last);
  }
}

int number_of_moves(struct game_state start)
{
  // value of top 49 bits when game solved
  const size_t solved = 81985526993846272;

  size_t cur_s;
  struct game_state cur;

  struct list_node *head = calloc(1, sizeof(struct list_node));
  lln_push(head, serialize(start));

  // actually this is just explosive so i guess the queue never empties
  errno = 0;
  while (1) {
    cur_s = lln_pop(head);
    if (errno != 0) {
      break;
    }

    cur = deserialize(cur_s);
    cur.num_steps &= (1 << 13) - 1;
    if ((cur_s & ~((1 << 15) - 1)) == solved) {
      break;
    }

    // hack: use 1 << 14th and 1 << 13th bits to store prev direction
    // up = 10, down = 11, left = 00, right = 01
    if (((cur_s >> 13) & 0b11) != 0b11) {
      start = cur;
      start.num_steps = cur.num_steps | (0b10 << 13);
      if (start.empty_row != 3) {
        move_up(&start);
        lln_push(head, serialize(start));
      }
    }

    if (((cur_s >> 13) & 0b11) != 0b10) {
      start = cur;
      start.num_steps = cur.num_steps | (0b11 << 13);
      if (start.empty_row != 0) {
        move_down(&start);
        lln_push(head, serialize(start));
      }
    }

    if (((cur_s >> 13) & 0b11) != 0b01) {
      start = cur;
      start.num_steps = cur.num_steps | (0b00 << 13);
      if (start.empty_col != 3) {
        move_left(&start);
        lln_push(head, serialize(start));
      }
    }

    if (((cur_s >> 13) & 0b11) != 0b00) {
      start = cur;
      start.num_steps = cur.num_steps | (0b01 << 13);
      if (start.empty_col != 0) {
        move_right(&start);
        lln_push(head, serialize(start));
      }
    }
  }

  lln_free(head);

  if (errno == EINVAL) {
    return -1;
  }

  return cur.num_steps;
}
