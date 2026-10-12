# HopNet - Engineering Journal & Viva Challenge Log

This document logs critical engineering decisions, memory bugs, protocol edge cases, and viva defense topics encountered during the development of HopNet.

---

## Log Entry #0: Initializing HopNet Development Roadmap
- **Date:** Sept 29, 2026
- **Context:** Project scope definition for 8-day sprint.
- **Core Decision:** Prioritize a robust, fully understood C simulation core with benchmarking over low-level BLE hardware drivers. Hardware drivers introduce high OS non-determinism, while protocol engineering in pure C yields higher viva defense scores and clean systems demonstration.
- **Viva Defense Point:** Explain the separation between **Protocol Layer** (framing, routing, deduplication, store-and-forward) and **Transport Layer** (in-memory simulator vs physical radio).

---

## Log Entry #1: Struct Padding & Binary Packet Alignment
- **Date:** Oct 8, 2026 (Day 1)
- **Context:** Designing `PacketHeader` struct in `include/packet.h`.
- **Problem:** Compilers automatically pad structs to 2, 4, or 8-byte boundaries, which distorts binary network wire layout.
- **Solution:** Enforced `#pragma pack(push, 1)` to mandate byte-exact 8-byte header packing with 0 padding bytes.
- **Viva Question:** *"Why do network protocol headers require `#pragma pack(push, 1)`?"*
- **Answer:** *"To eliminate compiler-inserted padding bytes, ensuring the binary memory layout matches the exact wire protocol format."*

---

## Log Entry #2: Next-Hop Bouncing Loop & Deduplication Cache
- **Date:** Oct 8, 2026 (Day 2)
- **Context:** Multi-hop relaying between Node 2 and Node 3.
- **Problem:** Naive next-hop loop selection caused Node 3 to bounce packet #1001 back to Node 2 until TTL expired.
- **Solution:** 
  1. Implemented `DeduplicationCache` struct embedded inside each `Node` struct (`Node.cache`).
  2. Checked `cache_contains` before processing packet. Duplicates trigger `[DUPLICATE DROP]`.
  3. Added forward-only node indexing `next_hop = current_node_id + 1` for single-path simulation.
- **Viva Question:** *"How do nodes track duplicate packets independently in memory?"*
- **Answer:** *"Each `Node` struct embeds an instance of `DeduplicationCache`, providing per-node state isolation for seen packet Message IDs."*

---

## Log Entry #3: ACK Packet ID Collision in Deduplication Cache
- **Date:** Oct 10, 2026 (Day 4)
- **Context:** Generating return ACK envelopes in `src/reliability.c`.
- **Problem:** When destination Node 4 received DATA packet #4002 and generated an ACK with the same `msg_id = 4002`, Node 4's cache dropped its own ACK packet because it had already seen `4002`!
- **Solution:** Assigned unique sequence offsets to ACK packets (`ack_pkt.msg_id = data_pkt.msg_id + 10000`), allowing ACK packets to traverse intermediate nodes cleanly without colliding with DATA packet IDs.
- **Viva Question:** *"Why can't ACK packets share the identical sequence ID of the DATA packet in a mesh network cache?"*
- **Answer:** *"Because relay nodes cache the DATA packet's message ID to prevent duplicate flooding. Re-using the same raw ID causes intermediate nodes to reject the ACK as a duplicate."*

---

## Log Entry #4: Dynamic Route Recovery via On-Demand BFS
- **Date:** Oct 11, 2026 (Day 4)
- **Context:** Primary link failure simulation in `src/failure.c`.
- **Problem:** Static routes break when participating nodes move or fail.
- **Solution:** Leveraged runtime BFS graph queries. When `graph_remove_edge(g, u, v)` severs a primary edge, BFS traverses surviving adjacency matrix links to dynamically select an alternate branch.
- **Viva Question:** *"How does HopNet recover from intermediate link failure without a centralized routing server?"*
- **Answer:** *"Source and relay nodes detect next-hop link drops and re-evaluate BFS against surviving active neighbors, dynamically rerouting packets through secondary mesh paths."*

---

## Log Entry #5: Empirical Systems Benchmarking (No Fabricated Stats)
- **Date:** Oct 11, 2026 (Day 5)
- **Context:** Building `src/benchmark.c` to measure real network metrics across 50-packet load.
- **Problem:** Academic guidelines prohibit assuming performance improvements like "reduced traffic by 75%" without empirical experimental evidence.
- **Solution:** Built POSIX high-resolution timer (`CLOCK_MONOTONIC`) and transmission accumulators to dynamically calculate real measured Packet Delivery Ratio (PDR), overhead ratios, and transmission counts across identical 50-packet loads.
- **Viva Question:** *"Why use CLOCK_MONOTONIC instead of time() or gettimeofday() for network latency benchmarking?"*
- **Answer:** *"CLOCK_MONOTONIC is guaranteed to never jump backwards due to system NTP clock adjustments, providing nanosecond-precision elapsed duration measurement."*

---

## Log Entry #6: End-to-End Encryption (E2EE) vs Hop-by-Hop Relaying
- **Date:** Oct 12, 2026 (Day 6)
- **Context:** Implementing payload confidentiality in `src/security.c` and terminal interface in `src/main.c`.
- **Problem:** In a peer-to-peer mesh, intermediate relay nodes (Bob, Charlie) must inspect packet headers to make routing decisions. Without security, relays can read or tamper with private user payloads in their memory buffers.
- **Solution:** 
  1. Decoupled the outer 8-byte routing envelope (`PacketHeader`) from the inner data payload (`pkt.payload`).
  2. Implemented an in-place symmetric XOR stream cipher (`security_xor_cipher`) with a byte-varying offset `(key + (i % 7))`.
  3. Intermediate nodes inspect `dest_id` and forward without having the secret key; destination node decrypts in-place with zero heap allocation.
- **Viva Question:** *"How does HopNet maintain message confidentiality when routing packets through untrusted intermediate nodes?"*
- **Answer:** *"HopNet implements End-to-End Encryption (E2EE). Intermediate nodes only inspect the 8-byte plaintext routing header to determine the next hop. The payload buffer remains encrypted ciphertext until it reaches the destination node, preventing eavesdropping and tampering."*

---

## Log Entry #7: Bluetooth RFCOMM Socket Bridge & OSI Layer Decoupling
- **Date:** Oct 12, 2026 (Day 7)
- **Context:** Implementing `include/bluetooth_bridge.h` and `src/bluetooth_bridge.c` to interface with physical Linux BlueZ radio hardware.
- **Problem:** Direct hardware drivers introduce heavy OS non-determinism, platform-specific header dependencies, and prevent deterministic unit testing on virtual environments like WSL.
- **Solution:** 
  1. Defined self-contained Linux Bluetooth socket ABI definitions (`AF_BLUETOOTH = 31`, `BTPROTO_RFCOMM = 3`, `struct sockaddr_rc`), eliminating external third-party library prerequisites.
  2. Implemented a dual-mode POSIX transport loop: uses standard POSIX `socket()`, `bind()`, `listen()`, `write()`, and `read()` to stream serialized binary `Packet` envelopes.
  3. Integrated simulated radio channel emulation for testing RFCOMM loopback without requiring physical dongles in virtualized environments.
- **Viva Question:** *"How does HopNet transition from in-memory simulation to physical Bluetooth transmission?"*
- **Answer:** *"HopNet decouples the Network Routing Layer from the Physical Transport Layer. In simulation mode, packets are copied to node memory queues; in real Bluetooth mode, the exact same binary packet is passed through `serialize_packet()` to a Linux RFCOMM Bluetooth socket (`BTPROTO_RFCOMM`) via standard POSIX `write()`. The protocol algorithms remain 100% identical."*
