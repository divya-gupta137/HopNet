#include "packet.h"
#include <string.h>
#include <stdio.h>

Packet create_packet(uint8_t src, uint8_t dest, uint16_t msg_id, PacketType type, uint8_t ttl, const char *message) {
    Packet pkt;
    memset(&pkt, 0, sizeof(Packet));

    pkt.header.src_id = src;
    pkt.header.dest_id = dest;
    pkt.header.msg_id = msg_id;
    pkt.header.type = (uint8_t)type;
    pkt.header.ttl = ttl;

    if (message != NULL) {
        size_t len = strlen(message);
        if (len > MAX_PAYLOAD_SIZE - 1) {
            len = MAX_PAYLOAD_SIZE - 1; // Prevent payload buffer overflow
        }
        memcpy(pkt.payload, message, len);
        pkt.payload[len] = '\0'; // Ensure null-terminated string payload
        pkt.header.payload_len = (uint16_t)len;
    } else {
        pkt.header.payload_len = 0;
    }

    return pkt;
}

int serialize_packet(const Packet *pkt, uint8_t *buffer, size_t buffer_size) {
    if (pkt == NULL || buffer == NULL) {
        return -1;
    }

    size_t total_size = sizeof(PacketHeader) + pkt->header.payload_len;
    if (buffer_size < total_size) {
        return -1; // Destination buffer too small!
    }

    // 1. Copy exact header bytes (8 bytes) into buffer start
    memcpy(buffer, &(pkt->header), sizeof(PacketHeader));

    // 2. Copy payload bytes immediately following the header
    if (pkt->header.payload_len > 0) {
        memcpy(buffer + sizeof(PacketHeader), pkt->payload, pkt->header.payload_len);
    }

    return (int)total_size;
}

int deserialize_packet(const uint8_t *buffer, size_t buffer_size, Packet *pkt) {
    if (buffer == NULL || pkt == NULL) {
        return -1;
    }

    // 1. Ensure buffer has at least enough bytes for the header
    if (buffer_size < sizeof(PacketHeader)) {
        return -1;
    }

    memset(pkt, 0, sizeof(Packet));

    // 2. Copy header bytes from raw byte buffer into Packet struct
    memcpy(&(pkt->header), buffer, sizeof(PacketHeader));

    // 3. Safety validation on payload length
    if (pkt->header.payload_len > MAX_PAYLOAD_SIZE) {
        return -1; // Corrupted packet header!
    }

    if (buffer_size < sizeof(PacketHeader) + pkt->header.payload_len) {
        return -1; // Truncated/incomplete packet payload!
    }

    // 4. Copy payload bytes into packet payload buffer
    if (pkt->header.payload_len > 0) {
        memcpy(pkt->payload, buffer + sizeof(PacketHeader), pkt->header.payload_len);
    }

    return 0;
}
