#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"

// because this is a demo, assume that the degree is fixed
#define NODE_DEGREE 16

typedef struct { uint32_t adjacent[NODE_DEGREE]; } node_t;

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

size_t lastvisited(node_t *biggraph, size_t number_of_nodes, size_t start_node,
                   size_t *visited) {
  *visited = 0;
  size_t arraysize = (number_of_nodes * 64 + sizeof(uint64_t) * 8 - 1) /
                     (sizeof(uint64_t) * 8);
  uint64_t *bitset = (uint64_t *)malloc(sizeof(uint64_t) * arraysize);
  assert(bitset != NULL);
  assert((number_of_nodes >> 6) < arraysize);
  memset(bitset, 0, sizeof(uint64_t) * arraysize);
  bitset[start_node >> 6] |= ((uint64_t)1) << (start_node % 64);
  size_t queue_capacity = 262144; // memory is cheap
  size_t *queue = (size_t *)malloc(queue_capacity * sizeof(size_t));
  size_t *buffer = (size_t *)malloc(queue_capacity * sizeof(size_t));
  assert(queue != NULL);
  assert(buffer != NULL);
  size_t queue_size = 1;
  size_t lastvisited = start_node;
  queue[0] = start_node;
  while (queue_size > 0) {
    size_t buffer_size = queue_size;
    // we could avoid the memcpy with more complicated code.
    assert(queue_size <= queue_capacity);
    memcpy(buffer, queue, queue_size * sizeof(size_t));
    queue_size = 0;
    *visited += buffer_size * NODE_DEGREE;
    for (size_t i = 0; i < buffer_size; i++) {
      node_t *queued_node = &biggraph[buffer[i]];
      if (unlikely(queue_size + NODE_DEGREE > queue_capacity)) {
        queue_capacity *= 2;
        queue = (size_t *)realloc(queue, queue_capacity * sizeof(size_t));
        buffer = (size_t *)realloc(buffer, queue_capacity * sizeof(size_t));
        assert(queue != NULL);
        assert(buffer != NULL);
      }
      for (size_t j = 0; j < NODE_DEGREE; j++) {
        uint32_t potential_new_node = queued_node->adjacent[j];
        bool visited = bitset[potential_new_node >> 6] &&
                       (((uint64_t)1) << (potential_new_node % 64));
        if (!visited) {
          lastvisited = potential_new_node;
          queue[queue_size++] = potential_new_node;
          bitset[potential_new_node >> 6] |= ((uint64_t)1)
                                             << (potential_new_node % 64);
        }
      }
    }
  }
  free(buffer);
  free(queue);
  free(bitset);
  return lastvisited;
}

int64_t distance(node_t *biggraph, size_t number_of_nodes, size_t start_node,
                 size_t final_node, bool prefetching) {
  int64_t distance = 0;
  if (start_node == final_node) {
    return distance;
  }

  size_t arraysize = (number_of_nodes * 64 + sizeof(uint64_t) * 8 - 1) /
                     (sizeof(uint64_t) * 8);
  uint64_t *bitset = (uint64_t *)malloc(sizeof(uint64_t) * arraysize);

  assert(bitset != NULL);
  assert((number_of_nodes >> 6) < arraysize);

  memset(bitset, 0, sizeof(uint64_t) * arraysize);
  bitset[start_node >> 6] |= ((uint64_t)1) << (start_node % 64);

  size_t queue_capacity = 262144; // memory is cheap
  size_t *queue = (size_t *)malloc(queue_capacity * sizeof(size_t));
  size_t *buffer = (size_t *)malloc(queue_capacity * sizeof(size_t));
  assert(queue != NULL);
  assert(buffer != NULL);
  size_t queue_size = 1;
  queue[0] = start_node;
  while (queue_size > 0) {
    distance++;
    size_t buffer_size = queue_size;
    // we could avoid the memcpy with more complicated code.
    assert(queue_size <= queue_capacity);
    memcpy(buffer, queue, queue_size * sizeof(size_t));
    queue_size = 0;
    for (size_t i = 0; i < buffer_size; i++) {
      node_t *queued_node = &biggraph[buffer[i]];
      if (prefetching)
        __builtin_prefetch(biggraph[buffer[i + 4]].adjacent, 1, 3);
      if (unlikely(queue_size + NODE_DEGREE > queue_capacity)) {
        queue_capacity *= 2;
        queue = (size_t *)realloc(queue, queue_capacity * sizeof(size_t));
        buffer = (size_t *)realloc(buffer, queue_capacity * sizeof(size_t));
        assert(queue != NULL);
        assert(buffer != NULL);
      }
      for (size_t j = 0; j < NODE_DEGREE; j++) {
        uint32_t potential_new_node = queued_node->adjacent[j];
        if (unlikely(potential_new_node == final_node)) { // success!!!
          free(buffer);
          free(queue);
          free(bitset);
          return distance;
        }
        bool visited = bitset[potential_new_node >> 6] &&
                       (((uint64_t)1) << (potential_new_node % 64));
        if (!visited) {
          queue[queue_size++] = potential_new_node;
          bitset[potential_new_node >> 6] |= ((uint64_t)1)
                                             << (potential_new_node % 64);
        }
      }
    }
  }
  free(buffer);
  free(queue);
  free(bitset);
  return -distance;
}

void demo(size_t number_of_nodes) {
  printf("number_of_nodes = %zu.\n", number_of_nodes);

  node_t *biggraph = (node_t *)malloc(sizeof(node_t) * number_of_nodes);
  printf("Building random graph with %zu nodes.\n", number_of_nodes);

  for (size_t i = 0; i < number_of_nodes; i++) {
    for (size_t j = 0; j < NODE_DEGREE; j++) {
      biggraph[i].adjacent[j] = rand() % number_of_nodes;
      // this might allow duplicates (unlikely), but it is fine
    }
  }
  // ok, the graph has been populated
  size_t start_node = rand() % number_of_nodes;
  printf(
      "making the graph connected... starting of with %zu potential nodes.\n",
      number_of_nodes);
  size_t volume = 0;
  size_t final_node =
      lastvisited(biggraph, number_of_nodes, start_node, &volume);
  // we seek the distance
  int64_t expected =
      distance(biggraph, number_of_nodes, start_node, final_node, false);
  printf("distance %lld \n", expected);
  int repeat = 5;
  bool USE_PREFETCH = true;
  bool DO_NOT_USE_PREFETCH = false;
  BEST_TIME(distance(biggraph, number_of_nodes, start_node, final_node,
                     DO_NOT_USE_PREFETCH),
            expected, , repeat, volume, true);
  BEST_TIME(
      distance(biggraph, number_of_nodes, start_node, final_node, USE_PREFETCH),
      expected, , repeat, volume, true);

  free(biggraph);
}

int main() {
  demo(10000000);
  return EXIT_SUCCESS;
}
