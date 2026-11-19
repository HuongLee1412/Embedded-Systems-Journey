#include <stdint.h>
#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#define BUFFER_SIZE 8
typedef struct{
    uint8_t buffer[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
} RingBuffer;
void ring_buffer_init(RingBuffer *rb);
uint8_t ring_buffer_count(const RingBuffer *rb);
int ring_buffer_push(RingBuffer *rb, uint8_t data);
int ring_buffer_pop(RingBuffer *rb, uint8_t *data);
#endif