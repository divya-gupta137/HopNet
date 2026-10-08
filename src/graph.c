#include "graph.h"
#include <stdio.h>
#include <string.h>
#include "cache.h"

void graph_init(Graph *g) {
    if (g == NULL) return;
    g->num_nodes = 0;
    memset(g->nodes, 0, sizeof(g->nodes));
    memset(g->adj, 0, sizeof(g->adj));
}

int graph_add_node(Graph *g, uint8_t id, int x, int y) {
    if (g == NULL || id >= MAX_NODES) return -1;
    
    g->nodes[id].id = id;
    g->nodes[id].is_active = true;
    g->nodes[id].x = x;
    g->nodes[id].y = y;
    g->nodes[id].packets_sent = 0;
    g->nodes[id].packets_relayed = 0;
    g->nodes[id].packets_received = 0;

    cache_init(&(g->nodes[id].cache)); // Initialize node's deduplication cache!

    if (id >= g->num_nodes) {
        g->num_nodes = id + 1;
    }
    return 0;
}

int graph_add_edge(Graph *g, uint8_t src_id, uint8_t dest_id) {
    if (g == NULL || src_id >= MAX_NODES || dest_id >= MAX_NODES) return -1;
    
    // Undirected radio link: if Alice reaches Bob, Bob reaches Alice
    g->adj[src_id][dest_id] = 1;
    g->adj[dest_id][src_id] = 1;
    return 0;
}

int graph_remove_edge(Graph *g, uint8_t src_id, uint8_t dest_id) {
    if (g == NULL || src_id >= MAX_NODES || dest_id >= MAX_NODES) return -1;
    
    g->adj[src_id][dest_id] = 0;
    g->adj[dest_id][src_id] = 0;
    return 0;
}

bool graph_has_edge(const Graph *g, uint8_t src_id, uint8_t dest_id) {
    if (g == NULL || src_id >= MAX_NODES || dest_id >= MAX_NODES) return false;
    
    return (g->adj[src_id][dest_id] == 1 && 
            g->nodes[src_id].is_active && 
            g->nodes[dest_id].is_active);
}

void graph_print(const Graph *g) {
    if (g == NULL) return;
    printf("\n=== HopNet Festival Map Topology ===\n");
    for (int i = 1; i < g->num_nodes; i++) {
        if (!g->nodes[i].is_active) continue;
        printf("Node %d (%d,%d) -> Links to: ", i, g->nodes[i].x, g->nodes[i].y);
        int connections = 0;
        for (int j = 1; j < g->num_nodes; j++) {
            if (i != j && g->adj[i][j] == 1 && g->nodes[j].is_active) {
                printf("Node %d  ", j);
                connections++;
            }
        }
        if (connections == 0) printf("[No active links]");
        printf("\n");
    }
    printf("===================================\n\n");
}
