#include <stdio.h>
#include "packet.h"

int main(void) {
    printf("===========================================\n");
    printf("   HopNet Packet Framing & Serialization   \n");
    printf("===========================================\n\n");

    // 1. Create a packet from Alice (Node 1) to David (Node 4)
    Packet orig_pkt = create_packet(1, 4, 1001, PACKET_TYPE_DATA, 5, "Meet me at the food court!");

    printf("[TX] Original Envelope Created:\n");
    printf("     Src ID: %d | Dest ID: %d | Msg ID: %d | Type: %d | TTL: %d | Payload Len: %d\n",
           orig_pkt.header.src_id, orig_pkt.header.dest_id, orig_pkt.header.msg_id,
           orig_pkt.header.type, orig_pkt.header.ttl, orig_pkt.header.payload_len);
    printf("     Payload: \"%s\"\n\n", (char *)orig_pkt.payload);

    // 2. Serialize into raw byte array
    uint8_t raw_wire_buffer[512];
    int bytes_serialized = serialize_packet(&orig_pkt, raw_wire_buffer, sizeof(raw_wire_buffer));

    if (bytes_serialized < 0) {
        printf("Error: Serialization failed!\n");
        return 1;
    }
    printf("[WIRE] Serialized to %d contiguous wire bytes.\n\n", bytes_serialized);

    // 3. Deserialize back into a new Rx Packet struct
    Packet rx_pkt;
    if (deserialize_packet(raw_wire_buffer, bytes_serialized, &rx_pkt) == 0) {
        printf("[RX] Deserialized Envelope at Destination:\n");
        printf("     Src ID: %d | Dest ID: %d | Msg ID: %d | Type: %d | TTL: %d | Payload Len: %d\n",
               rx_pkt.header.src_id, rx_pkt.header.dest_id, rx_pkt.header.msg_id,
               rx_pkt.header.type, rx_pkt.header.ttl, rx_pkt.header.payload_len);
        printf("     Payload: \"%s\"\n\n", (char *)rx_pkt.payload);
        printf("✅ SUCCESS: Packet created, serialized, and deserialized perfectly!\n");
    } else {
        printf("❌ ERROR: Packet deserialization failed!\n");
        return 1;
    }

    return 0;
}
