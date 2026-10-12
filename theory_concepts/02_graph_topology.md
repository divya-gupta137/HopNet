# Concept 02: Network Topology Graph & Adjacency Matrix in C

## 1. What is a Network Topology Graph?
A network topology graph models participating nodes (devices) and the physical links (edges) connecting them.

## 2. Adjacency Matrix Representation
We use a 2D array `uint8_t adj[MAX_NODES][MAX_NODES]` where:
- `adj[i][j] = 1`: Direct radio link exists between Node `i` and Node `j`.
- `adj[i][j] = 0`: Nodes `i` and `j` are out of direct radio range.

Because Bluetooth P2P links are bi-directional (undirected):
`adj[i][j] = 1; adj[j][i] = 1;`

## 3. Likely Viva Questions
1. **Q: Why use an Adjacency Matrix over an Adjacency List for small mesh networks?**
   - **A:** Adjacency Matrices offer $O(1)$ constant-time lookup to check if a link `adj[i][j]` exists, with zero pointer fragmentation.
2. **Q: How does HopNet check if Node A can reach Node B directly vs multi-hop?**
   - **A:** `graph_has_edge(g, A, B)` checks if `adj[A][B] == 1` and both nodes are active. If false, multi-hop relaying is triggered.
