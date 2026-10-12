#ifndef FAILURE_H
#define FAILURE_H

#include "graph.h"
#include "packet.h"
#include <stdbool.h>

// Simulates a physical radio link failure between two nodes
bool simulate_link_failure(Graph *g, uint8_t u, uint8_t v);

// Simulates link restoration (recovery)
bool simulate_link_restore(Graph *g, uint8_t u, uint8_t v);

// Demonstrates transmission over primary route, injecting failure, and recovering via alternate path
bool route_with_failure_recovery(Graph *g, uint8_t src_id, uint8_t dest_id, uint16_t msg_id, const char *msg);

#endif // FAILURE_H
