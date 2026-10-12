#include "failure.h"
#include "routing.h"
#include "reliability.h"
#include <stdio.h>

bool simulate_link_failure(Graph *g, uint8_t u, uint8_t v) {
    if (g == NULL) return false;
    printf("[FAILURE] ⚡ Cutting radio link between Node %d <---> Node %d!\n", u, v);
    return (graph_remove_edge(g, u, v) == 0);
}

bool simulate_link_restore(Graph *g, uint8_t u, uint8_t v) {
    if (g == NULL) return false;
    printf("[RECOVERY] 🔄 Restoring radio link between Node %d <---> Node %d!\n", u, v);
    return (graph_add_edge(g, u, v) == 0);
}

bool route_with_failure_recovery(Graph *g, uint8_t src_id, uint8_t dest_id, uint16_t msg_id, const char *msg) {
    if (g == NULL) return false;

    printf("\n--- STEP 1: Transmitting over Primary Route ---\n");
    int next_hop = routing_get_next_hop(g, src_id, dest_id);
    if (next_hop != -1) {
        printf("[INFO] Primary next-hop from Node %d is Node %d\n", src_id, next_hop);
    }

    // Deliver first packet over primary path
    send_packet_with_ack(g, src_id, dest_id, msg_id, msg);

    // Now inject link failure on the primary link!
    printf("\n--- STEP 2: Simulating Link Failure on Primary Path ---\n");
    simulate_link_failure(g, src_id, next_hop);

    printf("\n--- STEP 3: Transmitting Packet #%d with Dynamic Route Recovery ---\n", msg_id + 1);
    
    // Dynamic Route Discovery: Check if an alternate path exists
    int alternate_hop = routing_get_next_hop(g, src_id, dest_id);
    if (alternate_hop == -1) {
        printf("[FAILURE] ❌ No alternate route found! Network partitioned.\n");
        return false;
    }

    printf("[RECOVERY] 🎯 Dynamic BFS discovered Alternate Path via Node %d!\n", alternate_hop);
    bool recovered = send_packet_with_ack(g, src_id, dest_id, msg_id + 1, "Recovered via Alternate Path!");

    if (recovered) {
        printf("[SUCCESS] 🎉 Dynamic Route Recovery Succeeded! Packet delivered despite primary link failure.\n\n");
    }

    return recovered;
}
