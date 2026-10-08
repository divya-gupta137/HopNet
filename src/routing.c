#include "routing.h"
#include <stdio.h>
#include <string.h>

// BFS Algorithm to find the unweighted shortest path next-hop
int routing_get_next_hop(const Graph *g, uint8_t src_id, uint8_t dest_id) {
    if (g == NULL || src_id >= MAX_NODES || dest_id >= MAX_NODES) return -1;
    if (src_id == dest_id) return src_id;

    bool visited[MAX_NODES];
    int parent[MAX_NODES];
    int queue[MAX_NODES];
    int front = 0, rear = 0;

    memset(visited, 0, sizeof(visited));
    for (int i = 0; i < MAX_NODES; i++) parent[i] = -1;

    // Start BFS from src_id
    visited[src_id] = true;
    queue[rear++] = src_id;

    bool found = false;
    while (front < rear) {
        int u = queue[front++];

        if (u == dest_id) {
            found = true;
            break;
        }

        for (int v = 1; v < g->num_nodes; v++) {
            if (graph_has_edge(g, u, v) && !visited[v]) {
                visited[v] = true;
                parent[v] = u;
                queue[rear++] = v;
            }
        }
    }

    if (!found) return -1; // No physical path exists in graph!

    // Backtrack from dest_id to find the node right after src_id
    int curr = dest_id;
    while (parent[curr] != -1 && parent[curr] != src_id) {
        curr = parent[curr];
    }

    return curr;
}

// Baseline Epidemic Flooding (Shout to all neighbors)
bool route_flooding(Graph *g, uint8_t current_node_id, Packet pkt, uint32_t *total_broadcasts) {
    if (g == NULL) return false;

    Node *curr_node = &(g->nodes[current_node_id]);

    // 1. Deduplication Cache Check
    if (cache_contains(&(curr_node->cache), pkt.header.msg_id)) {
        return false; // Drop duplicate shout silently
    }
    cache_add(&(curr_node->cache), pkt.header.msg_id);

    // 2. Check Delivery
    if (pkt.header.dest_id == current_node_id) {
        printf("  🎉 [FLOOD DELIVERED] Packet #%d delivered to Node %d!\n", pkt.header.msg_id, current_node_id);
        return true;
    }

    // 3. TTL Expiration Check
    if (pkt.header.ttl <= 1) return false;
    pkt.header.ttl--;

    bool delivered = false;

    // Broadcast packet to ALL active neighbors simultaneously!
    for (int neighbor = 1; neighbor < g->num_nodes; neighbor++) {
        if (neighbor != current_node_id && graph_has_edge(g, current_node_id, neighbor)) {
            if (total_broadcasts) (*total_broadcasts)++;
            bool res = route_flooding(g, neighbor, pkt, total_broadcasts);
            if (res) delivered = true;
        }
    }

    return delivered;
}

// Smart Shortest Path Routing (BFS Next-Hop Relay)
bool route_shortest_path(Graph *g, uint8_t current_node_id, Packet pkt, uint32_t *total_hops) {
    if (g == NULL) return false;

    Node *curr_node = &(g->nodes[current_node_id]);

    if (cache_contains(&(curr_node->cache), pkt.header.msg_id)) {
        return false;
    }
    cache_add(&(curr_node->cache), pkt.header.msg_id);

    if (pkt.header.dest_id == current_node_id) {
        printf("  🎉 [SMART DELIVERED] Packet #%d delivered to Node %d!\n", pkt.header.msg_id, current_node_id);
        return true;
    }

    if (pkt.header.ttl <= 1) return false;
    pkt.header.ttl--;

    // Use BFS to get targeted next hop
    int next_hop = routing_get_next_hop(g, current_node_id, pkt.header.dest_id);
    if (next_hop == -1) {
        printf("  ⚠️ [NO ROUTE] Node %d has no route to Node %d!\n", current_node_id, pkt.header.dest_id);
        return false;
    }

    printf("  ➡️  [SMART ROUTE] Node %d forwarding packet #%d to next-hop Node %d\n",
           current_node_id, pkt.header.msg_id, next_hop);

    if (total_hops) (*total_hops)++;
    return route_shortest_path(g, next_hop, pkt, total_hops);
}
