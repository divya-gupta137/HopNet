#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <stddef.h>

#define MAX_PAYLOAD_SIZE 256
#define BROADCAST_NODE_ID 0xFF

// Types of envelopes circulating in our festival crowd
typedef enum {
    PACKET_TYPE_DATA  = 1,  // Regular text message
    PACKET_TYPE_ACK   = 2,  // Delivery receipt acknowledgement
    PACKET_TYPE_HELLO = 3   // Neighbor presence broadcast
} PacketType;

// Standardized Binary Header (8 bytes total)
// #pragma pack(push, 1) tells C compiler: DO NOT insert padded alignment bytes!
#pragma pack(push, 1)
typedef struct {
    uint8_t  src_id;       // Sender Node ID (1 byte: 0-255)
    uint8_t  dest_id;      // Destination Node ID (1 byte: 0-255, 255=Broadcast)
    uint16_t msg_id;       // Unique Message ID (2 bytes)
    uint8_t  type;         // PacketType (1 byte)
    uint8_t  ttl;          // Time-To-Live hop counter (1 byte)
    uint16_t payload_len;  // Payload byte length (2 bytes)
} PacketHeader;
#pragma pack(pop)

// Complete In-Memory Packet Envelope
typedef struct {
    PacketHeader header;
    uint8_t payload[MAX_PAYLOAD_SIZE];
} Packet;

// Function Declarations (Prototypes)
Packet create_packet(uint8_t src, uint8_t dest, uint16_t msg_id, PacketType type, uint8_t ttl, const char *message);
int serialize_packet(const Packet *pkt, uint8_t *buffer, size_t buffer_size);
int deserialize_packet(const uint8_t *buffer, size_t buffer_size, Packet *pkt);

#endif // PACKET_H
