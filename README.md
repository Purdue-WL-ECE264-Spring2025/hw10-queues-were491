[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/DHdTd1rH)
# Breadth-First Search

## Learning Goals

In this assignment, you will implement a queue backed by a linked-list, and use the queue to implement breadth-first search.

## Breadth-First Search

In lecture, you have learned about the depth-first search algorithm, which prioritizes exploring the children of recently-explored nodes. We might express this algorithm in pseudocode as:

```
node dfs(graph g, node start, node search) {
    stack s = new_stack();
    while (!empty(s)) {
        node cur = pop(&s);
        if (equals(cur, search)) {
            return cur;
        } else {
            for (node child in children(cur)) {
                push(&s, child);
            }
        }
    }
}
```

By pushing visited nodes to a Last-In, First-Out data structure like a stack, we ensure that the next node we explore is a child of the most recent node we visited.

By contrast, a _Breadth-First Search_ prioritizes exploring all the children of the current node _before_ exploring their children, and so on--in other words, it visits nodes in order of how close they are to the starting node. This makes it useful when, for instance, trying to calculate a shortest path: if the target node is reached, its guaranteed to be reached along the shortest possible path.

We can use a FIFO structure like a _queue_ to implement BFS:

```
node bfs(graph g, node start, node search) {
    queue q = new_queue();
    while (!empty(s)) {
        node cur = dequeue(&q);
        if (equals(cur, search)) {
            return cur;
        } else {
            for (node child in children(cur)) {
                enqueue(&q, child);
            }
        }
    }
}
```

## Implementing a Queue

A queue represents a particular policy for enqueuing and dequeuing elements, but the storage still needs to be backed by a physical data structure such as an array or a linked list.

### Singly Linked Lists

A linked list is a convenient data structure to hold a collection of elements when:

1. You don't know _a priori_ how many elements you'll need to hold
2. You want to be able to insert and remove elements in arbitrary locations, not just from the end.

Since the queue might grow arbitrarily large, and since the first-in, first-out policy means we need to be able to insert/remove from _both the beginning and end_, a linked list is a natural choice.

### Enqueue and Dequeue

While `enqueue` and `dequeue` need to touch opposite ends of the linked list, there are two valid choices:

- `enqueue` inserts at the _head_ and `dequeue` removes from the _tail_.
- `enqueue` inserts at the _tail_ and `dequeue` removes from the _head_.

For fun, we make you implement _all four functions_:

- `insert_at_head`
- `insert_at_tail`
- `remove_from_head`
- `remove_from_tail`

but feel free to use whichever pair of functions you want when implementing your actual queue!

## Tiles

In this assignment, you will use a breadth-first search to determine the _shortest number of moves_ required to solve an instance of the _Tiles_ game.

The game takes place on a $4\times 4$ grid with 15 tiles numbered 1 through 15, and one space left empty. The aim of the game is to slide tiles around until they end up in the configuration shown below:

```
+---+---+---+---+
| 1 | 2 | 3 | 4 |
+---+---+---+---+
| 5 | 6 | 7 | 8 |
+---+---+---+---+
| 9 | 10| 11| 12|
+---+---+---+---+
| 13| 14| 15|   |
+---+---+---+---+
```

For example, "solving" the following board takes two moves: one to slide the `11` tile up, and one to slide the `15` tile left.

```
+---+---+---+---+
| 1 | 2 | 3 | 4 |
+---+---+---+---+
| 5 | 6 | 7 | 8 |
+---+---+---+---+
| 9 | 10|   | 12|
+---+---+---+---+
| 13| 14| 11| 15|
+---+---+---+---+
```

### Tracking the game state

We have provided a structure (`struct game_state`) to track the current state of a Tiles game. The structure contains the following data:

- A $4\times 4$ array of `uint8_t`s representing the configuration of tiles, where the array is indexed by _row_ then _column_, and an entry of $0$ represents an empty space
- A pair of indices for the coordinates of the empty space (for convenience, although this can be determined from the array above)
- An integer representing the number of moves that have taken place so far

Because your linked list (and therefore your queue) can only hold integers, we also provide a function to `serialize` a `struct game_state` into an integer, and one to `deserialize` a `struct game_state` _from_ an integer.

> **Note:** The `serialize` and `deserialize` functions are guaranteed to be correct when called on valid inputs (e.g., structs that have exactly one instance of each tile from 1-15, and in which the empty coordinates match the state of the board), but their behavior is otherwise undefined.

## Instructions

Your implementation is across two files:

- In `linked_list.c`:
  - Implement functions to insert/remove values from the head/tail of a linked list
  - Implement a function to free a linked list
- In `queue.c`:
  - Use your linked list and the `serialize`/`deserialize` functions to implement `enqueue` and `dequeue`
  - Use the queue to implement a BFS that determines the shortest `number_of_moves` required to solve a Tiles game

You only need to submit `linked_list.c` and `queue.c`. DO NOT edit the functions in `tile_game.c`
