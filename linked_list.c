#include "linked_list.h"

#include <stdlib.h>
#include <errno.h>
#include <assert.h>

struct list_node *new_node(size_t value)
{
  struct list_node *node = malloc(sizeof(struct list_node));
  node->value = value;
  return node;
}

void insert_at_head(struct linked_list *list, size_t value)
{
  assert(list != NULL);

  struct list_node *new_head = new_node(value);
  new_head->next = list->head;
  list->head = new_head;
}

void insert_at_tail(struct linked_list *list, size_t value)
{
  assert(list != NULL);

  // account for empty list
  if (list->head == NULL)
  {
    insert_at_head(list, value);
    return;
  }

  // track until cur is the last element in the list
  struct list_node *cur = list->head;
  while (cur->next != NULL)
  {
    cur = cur->next;
  }

  // add a new node
  struct list_node *new_tail = new_node(value);
  new_tail->next = NULL;
  cur->next = new_tail;
}

size_t remove_from_head(struct linked_list *list)
{
  assert(list != NULL);

  struct list_node *old_head = list->head;
  size_t ret_val;

  // account for empty list
  if (old_head != NULL)
  {
    list->head = old_head->next;

    ret_val = old_head->value;
    // memory leak lol
    free(old_head);
  }
  else
  {
    errno = EINVAL;
    ret_val = -1;
  }

  return ret_val;
}

size_t remove_from_tail(struct linked_list *list)
{
  assert(list != NULL);

  struct list_node *cur = list->head;
  struct list_node *last = NULL;
  size_t ret_val;

  // account for empty list
  if (cur != NULL)
  {
    // track until cur is the last element in the list
    while (cur->next != NULL)
    {
      last = cur;
      cur = cur->next;
    }

    ret_val = cur->value;
    // account for one element list
    // we can't just set last = cur at the beginning
    // since we also need to set list->head
    if (last != NULL)
    {
      last->next = NULL;
    }
    else
    {
      list->head = NULL;
    }
    free(cur);
  }
  else
  {
    errno = EINVAL;
    ret_val = -1;
  }

  return ret_val;
}

void free_list(struct linked_list list)
{
  struct list_node *cur = list.head;
  struct list_node *next;

  while (cur != NULL)
  {
    next = cur->next;
    free(cur);
    cur = next;
  }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list)
{
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next)
  {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
