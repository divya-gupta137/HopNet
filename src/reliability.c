#include "reliability.h"
#include "routing.h"
#include <stdio.h>

Packet create_ack_packet(const Packet *data_pkt) {
    // ACK packet flips src and dest from original DATA packet
    return create_packet(
        data_pkt->header.dest_id, // New Src = Original Dest
        data_pkt->header.src_id,  // New Dest = Original Src
        data_pkt->header.msg_id,  // Same Message ID
        PACKET_TYPE_ACK,          // Type ACK
        5,                        // Initial TTL = 5
        "ACK_OK"                  // Payload
    );
}

bool send_packet_with_ack(Graph *g, uint8_t src_id, uint8_t dest_id, uint16_t msg_id, const char *msg) {
    if (g == NULL) return false;

    Packet data_pkt = create_packet(src_id, dest_id, msg_id, PACKET_TYPE_DATA, 5, msg);

    for (int attempt = 1; attempt <= MAX_RETRANSMIT_ATTEMPTS; attempt++) {
        printf("[RELIABILITY] Attempt %d/%d: Transmitting Data Packet #%d from Node %d to Node %d...\n",
               attempt, MAX_RETRANSMIT_ATTEMPTS, msg_id, src_id, dest_id);

        uint32_t hops = 0;
        bool delivered = route_shortest_path(g, src_id, data_pkt, &hops);

        if (delivered) {
            printf("[RELIABILITY] Node %d received Data Packet #%d. Generating ACK...\n", dest_id, msg_id);
            Packet ack_pkt = create_ack_packet(&data_pkt);

            // Re-initialize caches so ACK packet can traverse back cleanly
            for (int i = 1; i < g->num_nodes; i++) {
                cache_init(&(g->nodes[i].cache));
            }

            uint32_t ack_hops = 0;
            printf("[RELIABILITY] Routing ACK Packet #%d back from Node %d to Node %d...\n", msg_id, dest_id, src_id);
            bool ack_delivered = route_shortest_path(g, dest_id, ack_pkt, &ack_hops);

            if (ack_delivered) {
                printf("[RELIABILITY] 🎉 ACK Confirmed! Packet #%d delivered and acknowledged in attempt %d.\n\n",
                       msg_id, attempt);
                return true;
            }
        }

        printf("[RELIABILITY] ⚠️ ACK Timeout / Transmission failed on attempt %d. Retransmitting...\n\n", attempt);
        
        // Re-initialize caches for retry attempt
        for (int i = 1; i < g->num_nodes; i++) {
            cache_init(&(g->nodes[i].cache));
        }
    }

    printf("[RELIABILITY] ❌ FAILED: Packet #%d failed after %d retransmission attempts.\n\n",
           msg_id, MAX_RETRANSMIT_ATTEMPTS);
    return false;
}
