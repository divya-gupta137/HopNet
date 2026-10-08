#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include <stdbool.h>
#include "cache.h"

#define MAX_NODES 32

// Structure representing an individual festival participant (Node)
typedef struct {
    uint8_t id;               // Node ID (1, 2, 3, etc.)
    bool is_active;           // Is node online/active? (true/false)
    int x;                    // Simulated X coordinate
    int y;                    // Simulated Y coordinate
    uint32_t packets_sent;    // Statistics: Sent
    uint32_t packets_relayed; // Statistics: Forwarded
    uint32_t packets_received;// Statistics: Received
    DeduplicationCache cache; // <-- Each node's deduplication cache!
} Node;

#endif // NODE_H
