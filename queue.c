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
  insert_at_tail(&(q->data), serialize(state));
}

struct game_state dequeue(struct queue *q)
{
  assert(q != NULL);
  return deserialize(remove_from_head(&(q->data)));
}

static inline void enqueue_hax(struct queue *q, struct queue *qlast,
                               struct game_state start, size_t cache[])
{
  size_t start_s = serialize(start) >> 15;
  if (cache[start_s % 100000] == start_s)
  {
    return;
  }
  cache[start_s % 100000] = start_s;

  enqueue(qlast, start);
  if (q->data.head == NULL)
  {
    q->data = qlast->data;
  }
  else
  {
    qlast->data.head = qlast->data.head->next;
  }
}

int number_of_moves(struct game_state start)
{
  // value of top 49 bits when game solved
  const size_t solved = 81985526993846272 >> 15;

  struct game_state cur;

  struct queue qq = {0};
  struct queue qqlast = {0}; // caches last node in list so insertion is O(1)

  // poorly written but very fast cache
  // breaks on large step counts, but the max is only about 80 anyways
  size_t cache[CACHE_SIZE] = {0};

  enqueue_hax(&qq, &qqlast, start, cache);

  // actually this is just explosive so i guess the queue never empties
  errno = 0;
  while (1)
  {
    cur = dequeue(&qq);
    if (errno != 0)
    {
      break;
    }
    if (qq.data.head == NULL)
    {
      qqlast = qq;
    }

    // this repeated serialization-deserialization code seriously ticks me off
    if ((serialize(cur) >> 15) == solved)
    {
      break;
    }

    if (cur.empty_row != 3)
    {
      start = cur;
      move_up(&start);
      enqueue_hax(&qq, &qqlast, start, cache);
    }

    if (cur.empty_col != 3)
    {
      start = cur;
      move_left(&start);
      enqueue_hax(&qq, &qqlast, start, cache);
    }

    if (cur.empty_row != 0)
    {
      start = cur;
      move_down(&start);
      enqueue_hax(&qq, &qqlast, start, cache);
    }

    if (cur.empty_col != 0)
    {
      start = cur;
      move_right(&start);
      enqueue_hax(&qq, &qqlast, start, cache);
    }
  }

  free_list(qq.data);

  if (errno == EINVAL)
  {
    return -1;
  }

  return cur.num_steps;
}
