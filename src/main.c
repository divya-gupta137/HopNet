#include <stdio.h>
#include "packet.h"
#include "graph.h"
#include "routing.h"
#include "reliability.h"

int main(void) {
    printf("===========================================\n");
    printf("  HopNet Phase 4: Reliability & ACK Layer  \n");
    printf("===========================================\n\n");

    Graph g;
    graph_init(&g);

    graph_add_node(&g, 1, 0, 0);   // Alice
    graph_add_node(&g, 2, 40, 0);  // Bob
    graph_add_node(&g, 3, 80, 0);  // Charlie
    graph_add_node(&g, 4, 120, 0); // David

    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 3, 4);

    graph_print(&g);

    // TEST: End-to-End DATA Transmission + ACK Confirmation
    printf("--- TEST: Transmitting Data Packet #4002 with End-to-End ACK Confirmation ---\n");
    send_packet_with_ack(&g, 1, 4, 4002, "Reliable Message with ACK!");

    return 0;
}
