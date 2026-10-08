#include "buffer.h"
#include <string.h>

void buffer_init(PacketBuffer *buf) {
    if (buf == NULL) return;
    buf->head = 0;
    buf->tail = 0;
    buf->count = 0;
    memset(buf->packets, 0, sizeof(buf->packets));
}

bool buffer_is_full(const PacketBuffer *buf) {
    if (buf == NULL) return true;
    return buf->count >= MAX_RING_BUFFER_SIZE;
}

bool buffer_is_empty(const PacketBuffer *buf) {
    if (buf == NULL) return true;
    return buf->count == 0;
}

bool buffer_enqueue(PacketBuffer *buf, Packet pkt) {
    if (buf == NULL || buffer_is_full(buf)) {
        return false; // Buffer overflow! Cannot accept new packet.
    }

    buf->packets[buf->head] = pkt;
    buf->head = (buf->head + 1) % MAX_RING_BUFFER_SIZE; // Circular wraparound
    buf->count++;
    return true;
}

bool buffer_dequeue(PacketBuffer *buf, Packet *out_pkt) {
    if (buf == NULL || buffer_is_empty(buf) || out_pkt == NULL) {
        return false; // Buffer empty!
    }

    *out_pkt = buf->packets[buf->tail];
    buf->tail = (buf->tail + 1) % MAX_RING_BUFFER_SIZE; // Circular wraparound
    buf->count--;
    return true;
}
