#ifndef ROUTING_H
#define ROUTING_H

#include "packet.h"
#include "graph.h"
#include <stdbool.h>

// Routing Strategies available in HopNet
typedef enum {
    ROUTING_MODE_FLOODING       = 1, // Baseline Shouting (Broadcast to all neighbors)
    ROUTING_MODE_SHORTEST_PATH  = 2  // Smart Shortest Path Routing (BFS / Dijkstra)
} RoutingMode;

// Finds the optimal next-hop Node ID from src_id to dest_id using BFS
int routing_get_next_hop(const Graph *g, uint8_t src_id, uint8_t dest_id);

// Simulates baseline Epidemic Flooding (returns true if delivered, tracks total duplicate broadcasts)
bool route_flooding(Graph *g, uint8_t current_node_id, Packet pkt, uint32_t *total_broadcasts);

// Simulates smart Shortest-Path Routing (returns true if delivered, tracks total hops taken)
bool route_shortest_path(Graph *g, uint8_t current_node_id, Packet pkt, uint32_t *total_hops);

#endif // ROUTING_H
