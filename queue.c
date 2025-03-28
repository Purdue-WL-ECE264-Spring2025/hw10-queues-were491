#include "queue.h"
#include "tile_game.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>

// for some reason powers of two are really slow at this
// and trying to resist collisions doesn't help either
#define CACHE_SIZE 100000

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
struct dll_node
{
  struct dll_node *next;
  struct dll_node *prev;
  size_t value;
};

struct dll_l
{
  struct dll_node *first;
  struct dll_node *last;
};

static void dll_push(struct dll_l *lst, size_t val, size_t *cache)
{
  if (cache[(val >> 15) % CACHE_SIZE] == (val >> 15))
  {
    return;
  }
  cache[(val >> 15) % CACHE_SIZE] = val >> 15;

  struct dll_node *first = malloc(sizeof(*first));
  first->value = val;

  first->next = lst->first;
  if (lst->first != NULL)
  {
    lst->first->prev = first;
  }

  lst->first = first;
  if (lst->last == NULL)
  {
    lst->last = first;
  }
}

static size_t dll_pop(struct dll_l *lst)
{
  size_t ret_val;
  struct dll_node *last;

  if (lst->last == NULL)
  {
    errno = EINVAL;
    return 0;
  }

  ret_val = lst->last->value;

  if (lst->first == lst->last)
  {
    free(lst->last);
    lst->first = NULL;
    lst->last = NULL;
  }
  else
  {
    last = lst->last->prev;
    free(lst->last);
    lst->last = last;
    if (last != NULL)
    {
      last->next = NULL;
    }
  }

  return ret_val;
}

static void dll_free(struct dll_l *lst)
{
  struct dll_node *tmp;
  while (lst->first != NULL)
  {
    tmp = lst->first;
    lst->first = lst->first->next;
    free(tmp);
  }
  free(lst);
}

int number_of_moves(struct game_state start)
{
  // value of top 49 bits when game solved
  const size_t solved = 81985526993846272 >> 15;

  size_t cur_s;
  struct game_state cur;

  size_t *cache = calloc(CACHE_SIZE, sizeof(*cache));
  struct dll_l *lst = calloc(1, sizeof(*lst));

  dll_push(lst, serialize(start), cache);

  // actually this is just explosive so i guess the queue never empties
  errno = 0;
  while (1)
  {
    cur_s = dll_pop(lst);
    if (errno != 0)
    {
      break;
    }

    cur = deserialize(cur_s);
    if ((cur_s >> 15) == solved)
    {
      break;
    }

    if (cur.empty_row != 3)
    {
      start = cur;
      move_up(&start);
      dll_push(lst, serialize(start), cache);
    }

    if (cur.empty_col != 3)
    {
      start = cur;
      move_left(&start);
      dll_push(lst, serialize(start), cache);
    }

    if (cur.empty_row != 0)
    {
      start = cur;
      move_down(&start);
      dll_push(lst, serialize(start), cache);
    }

    if (cur.empty_col != 0)
    {
      start = cur;
      move_right(&start);
      dll_push(lst, serialize(start), cache);
    }
  }

  free(cache);
  dll_free(lst);

  if (errno == EINVAL)
  {
    return -1;
  }

  return cur.num_steps;
}
