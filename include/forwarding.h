#ifndef FORWARDING_H
#define FORWARDING_H

#include "packet.h"
#include "graph.h"
#include <stdbool.h>

// Simulates node packet processing and multi-hop forwarding
bool process_packet(Graph *g, uint8_t current_node_id, Packet pkt);

#endif // FORWARDING_H
