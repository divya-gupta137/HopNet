# Concept 03: TTL & Deduplication Cache in C

## 1. Loop Prevention via TTL (Time-To-Live)
Every packet contains a `ttl` field in its header:
- At creation: `ttl = 5` (max allowed hops).
- At each relay node: `ttl` is decremented by 1 (`pkt.header.ttl--`).
- If `ttl <= 1`: The packet is dropped immediately (`[DROP]`). This prevents infinite routing loops in mesh networks.

## 2. Duplicate Packet Suppression (Deduplication Cache)
To prevent nodes from processing or re-forwarding duplicate copies of the same packet:
- Each `Node` struct contains an embedded `DeduplicationCache cache;`.
- Before processing packet `#msg_id`, the node runs `cache_contains(&(node->cache), msg_id)`.
- If true $\rightarrow$ Drop packet (`[DUPLICATE DROP]`).
- If false $\rightarrow$ Log `msg_id` in `cache` and process packet.

## 3. Likely Viva Questions
1. **Q: What happens if TTL is omitted in a mesh network?**
   - **A:** Packets circulate indefinitely in cyclic paths, consuming bandwidth and causing network congestion storm.
2. **Q: How does `DeduplicationCache` handle memory when full?**
   - **A:** Implements FIFO (First-In, First-Out) sliding replacement, discarding the oldest seen message ID when `count == MAX_CACHE_SIZE`.
