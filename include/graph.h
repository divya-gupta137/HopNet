#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include <stddef.h>

// Adjacency Matrix Graph representation of the festival network
typedef struct {
    int num_nodes;
    Node nodes[MAX_NODES];
    uint8_t adj[MAX_NODES][MAX_NODES]; // adj[i][j] = 1 if radio link exists
} Graph;

void graph_init(Graph *g);
int graph_add_node(Graph *g, uint8_t id, int x, int y);
int graph_add_edge(Graph *g, uint8_t src_id, uint8_t dest_id);
int graph_remove_edge(Graph *g, uint8_t src_id, uint8_t dest_id);
bool graph_has_edge(const Graph *g, uint8_t src_id, uint8_t dest_id);
void graph_print(const Graph *g);

#endif // GRAPH_H
