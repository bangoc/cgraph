#include <stdio.h>
#include <stdlib.h>

#include "ctc/queue.h"

void print_i(void *value) {
  printf(" %d", *((int*)value));
}

int main() {
  Queue *q = NULL;
  queue_new(&q);
  printf("Enqueue\n");
  for (int value = 1; value <= 10; ++value) {
    int *tmp = (int*)malloc(sizeof(int));
    *tmp = value;
    printf(" %d", value);
    queue_enqueue(q, tmp);
  }
  printf("\n");
  printf("Queue size = %lu\n", queue_size(q));
  printf("Iterate by iterator\n");
  QueueIter iter;
  queue_iter_init(&iter, q);
  void *value;
  while (queue_iter_next(&iter, &value) != CC_ITER_END) {
    printf(" %d", *((int*)value));
  }
  printf("\n");

  printf("Foreach\n");
  queue_foreach(q, print_i);
  printf("\n");

  printf("Poll (get and remove front)\n");
  while (queue_poll(q, &value) != CC_ERR_OUT_OF_RANGE) {
    printf(" %d", *((int*)value));    
  }
  printf("\n");
  printf("Size after poll = %lu:\n", queue_size(q));
  return 0;
}