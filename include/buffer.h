#ifndef BUFFER_H
#define BUFFER_H

#include "packet.h"
#include <stdbool.h>

#define MAX_RING_BUFFER_SIZE 16

// Thread-safe Store-and-Forward Circular Ring Buffer
typedef struct {
    Packet packets[MAX_RING_BUFFER_SIZE];
    int head; // Index where next packet will be written
    int tail; // Index where next packet will be read
    int count;// Number of packets currently queued
} PacketBuffer;

void buffer_init(PacketBuffer *buf);
bool buffer_is_full(const PacketBuffer *buf);
bool buffer_is_empty(const PacketBuffer *buf);
bool buffer_enqueue(PacketBuffer *buf, Packet pkt);
bool buffer_dequeue(PacketBuffer *buf, Packet *out_pkt);

#endif // BUFFER_H
