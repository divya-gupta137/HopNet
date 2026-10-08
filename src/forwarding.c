#include "forwarding.h"
#include <stdio.h>
#include "cache.h"

bool process_packet(Graph *g, uint8_t current_node_id, Packet pkt) {
    if (g == NULL) return false;

    Node *curr_node = &(g->nodes[current_node_id]);

    printf("[NODE %d] Received packet #%d (Src: %d -> Dest: %d, TTL: %d)\n",
           current_node_id, pkt.header.msg_id, pkt.header.src_id, pkt.header.dest_id, pkt.header.ttl);

    // 1. Check for Duplicate Packet (Has this node already seen this Message ID?)
    if (cache_contains(&(curr_node->cache), pkt.header.msg_id)) {
        printf("  ❌ [DUPLICATE DROP] Node %d has already processed packet #%d! Dropping duplicate.\n\n",
               current_node_id, pkt.header.msg_id);
        return false;
    }

    // Mark this packet ID as seen by current node
    cache_add(&(curr_node->cache), pkt.header.msg_id);

    // 2. Check if packet reached final destination
    if (pkt.header.dest_id == current_node_id) {
        curr_node->packets_received++;
        printf("  🎉 [DELIVERED] Packet #%d successfully delivered to destination Node %d!\n",
               pkt.header.msg_id, current_node_id);
        printf("  📩 Payload Content: \"%s\"\n\n", (char *)pkt.payload);
        return true;
    }

    // 3. Decrement TTL (Time-To-Live) for loop prevention
    if (pkt.header.ttl <= 1) {
        printf("  ❌ [DROP] Packet #%d dropped at Node %d due to TTL Expiration (TTL=0)!\n\n",
               pkt.header.msg_id, current_node_id);
        return false;
    }

    pkt.header.ttl--; // Decrement hop counter
    curr_node->packets_relayed++;

    // 4. Smart Forwarding: Prioritize destination if directly connected
    if (graph_has_edge(g, current_node_id, pkt.header.dest_id)) {
        printf("  ➡️  [DIRECT FORWARD] Node %d forwarding packet #%d directly to Destination Node %d (New TTL: %d)\n",
               current_node_id, pkt.header.msg_id, pkt.header.dest_id, pkt.header.ttl);
        return process_packet(g, pkt.header.dest_id, pkt);
    }

    // 5. Otherwise, forward to next valid neighbor along path
    for (int next_hop = current_node_id + 1; next_hop < g->num_nodes; next_hop++) {
        if (graph_has_edge(g, current_node_id, next_hop)) {
            printf("  ➡️  [FORWARD] Node %d forwarding packet #%d to next-hop Node %d (New TTL: %d)\n",
                   current_node_id, pkt.header.msg_id, next_hop, pkt.header.ttl);
            return process_packet(g, next_hop, pkt);
        }
    }

    printf("  ⚠️ [DEAD-END] Node %d has no available forward neighbors for packet #%d!\n\n",
           current_node_id, pkt.header.msg_id);
    return false;
}
