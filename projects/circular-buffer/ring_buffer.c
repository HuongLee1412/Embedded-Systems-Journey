#include "ring_buffer.h"
#include <stddef.h>
void ring_buffer_init(RingBuffer *rb){
    if(rb == NULL) return;
    rb->head = 0;
    rb->tail = 0;
}

uint8_t ring_buffer_count(const RingBuffer *rb){
    if(rb == NULL){
        return 0;
    }
    return (rb->tail - rb->head + BUFFER_SIZE) % BUFFER_SIZE;
}

int ring_buffer_push(RingBuffer *rb, uint8_t data){
    if(rb == NULL){
        return 0;
    }
    if((rb->tail + 1) % BUFFER_SIZE == rb->head){
        return 0;
    }
    rb->buffer[rb->tail] = data;
    rb->tail = (rb->tail + 1) % BUFFER_SIZE;
    return 1;
}

int ring_buffer_pop(RingBuffer *rb, uint8_t *data){
    if(rb == NULL || data == NULL){
        return 0;
    }
    if((rb->tail) == (rb->head)){
        return 0;
    }
    *data = rb->buffer[rb->head];
    rb->head = (rb->head + 1) % BUFFER_SIZE;
    return 1;
}