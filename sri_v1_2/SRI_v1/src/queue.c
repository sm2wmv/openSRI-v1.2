#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define QUEUE_DEBUG

#include "queue.h"

#ifdef QUEUE_DEBUG
  #include "misc.h"
#endif

unsigned char queue_enqueue(queue* q, void* data) {
  #ifdef QUEUE_DEBUG
    PRINTF("Queue size: %i\n\r",q->queue_size);
    PRINTF("Count: %i\n\r",q->count);
  #endif
  
  if (q->count >= q->queue_size) {
    #ifdef QUEUE_DEBUG
      PRINTF("Warning: queue overflow enqueue\n\r");
    #endif
    return(1);
  }
  else {
    q->last = (q->last+1) % q->queue_size;
    memcpy(q->content+q->element_size*q->last, data, q->element_size);
    q->count++;
    
    return(0);
  }
}

unsigned char queue_dequeue(queue* q, void* res) {
  if (q->count <= 0) {
    #ifdef QUEUE_DEBUG
      PRINTF("Warning: empty queue dequeue.\n");
    #endif
    return(1);
  }
  else {
    memcpy((char *)res, q->content+q->first*q->element_size, q->element_size);
    q->first = (q->first+1) % q->queue_size;
    q->count--;
  }

  return(0);
}

unsigned char queue_get_first(queue *q, void* res) {
  if (q->count <= 0) {
    #ifdef QUEUE_DEBUG
      PRINTF("Warning: empty queue get first\n\r");
    #endif
    return(1);
  }
  else {
    memcpy((char *)res, q->content+q->first*q->element_size, q->element_size);
    
    return(0);
  }
}

unsigned char queue_is_empty(queue *q) {
  return(q->count == 0);
}

unsigned int queue_get_count(queue *q) {
  return(q->count);
}

unsigned char queue_drop(queue *q) {  
  if (q->count > 0) {
    q->first = (q->first+1) % q->queue_size;
    q->count--;
    
    return(1);
  }
  else {
    #ifdef QUEUE_DEBUG
      PRINTF("Warning: The queue was already empty - queue_drop()\n\r");
    #endif
      
    return(0);
  }
}

void queue_drop_all(queue *q) {
  q->first = 0;
  q->last = q->queue_size-1;
  q->count = 0;
}
