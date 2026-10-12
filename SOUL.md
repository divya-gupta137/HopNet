# HopNet - Project Soul & Architecture Tracker

## Project Overview
HopNet is a C-based multi-hop peer-to-peer mesh networking and routing simulator. It simulates ad-hoc node communication, multi-hop packet relaying, loop prevention, route discovery, and performance benchmarking.

## The Master Analogy (The Desert Music Festival)
- **Scenario:** A crowdsourced communication network where Alice wants to send a message to David 500 meters away without cellular internet or direct radio range.
- **Multi-Hop Relay:** Alice -> Bob -> Charlie -> David.
- **Key Protocol Mechanics:**
  1. **Binary Packet Envelopes:** Header metadata (Sender, Receiver, Msg ID, TTL) + Payload.
  2. **TTL (Loop Prevention):** Expiration counter decremented at each hop to prevent infinite looping.
  3. **Message ID Deduplication:** Nodes log seen Message IDs in a cache to drop duplicate packets instantly.
  4. **Flooding vs Shortest Path Routing:** Compare naive shouting to everyone (Flooding) vs target next-hop routing (Dijkstra/Shortest-Path).
  5. **Store-and-Forward Ring Buffer:** Temporary pocket storage when a neighbor node temporarily disappears.
  6. **Reliability Layer:** End-to-end ACKs with automatic retransmission retries.
  7. **Dynamic Route Recovery:** Automatic BFS alternate-path fallback when primary links drop.
  8. **Benchmarking Layer:** Measuring Packet Delivery Ratio (PDR), Latency, and Routing Overhead across traffic loads.

## Current Status
- **Phase:** Project Complete (All 8 Phases Delivered & Validated)
- **Current Target Date:** Oct 12, 2026
- **Status:** All 8 Phases 100% completed. Codebase verified with Valgrind (0 bytes leaked, 0 errors). E2EE security, pure C interactive TUI, dynamic route recovery, store-and-forward buffers, automated POSIX benchmarking, and Linux Bluetooth RFCOMM socket bridge fully functional.

## Milestone Progress
- [x] Phase 0: Master Story Analogy, Architecture Design & Repo Blueprint (Completed)
- [x] Phase 1: Repo Setup, Binary Packet Framing & Topology Graph (Day 1 - Completed)
- [x] Phase 2: Multi-Hop Forwarding, TTL Loop Prevention & Deduplication Cache (Day 2 - Completed)
- [x] Phase 3: Routing Protocol Engine (Baseline Flooding vs Shortest Path Routing) (Day 3 - Completed)
- [x] Phase 4: Circular Ring Buffer, End-to-End ACKs & Dynamic Route Recovery (Day 4 - Completed)
- [x] Phase 5: Automated Benchmarking Engine (PDR, Latency, Overhead calculation) (Day 5 - Completed)
- [x] Phase 6: Unified Showcase CLI, Interactive Terminal TUI & E2EE Security Layer (Day 6 - Completed)
- [x] Phase 7: Real Bluetooth RFCOMM Bridge Layer (Day 7 - Completed)
- [x] Phase 8: Valgrind Memory Leak Audit & Viva Defense Dry Run (Day 8 - Completed)

## Architecture Decisions
1. **Simulation-First:** Transport is decoupled from protocol logic. Simulator uses in-memory node structures. Real Bluetooth is deferred to Phase 7.
2. **Fixed-Width Binary Framing:** Uses `<stdint.h>` (`uint8_t`, `uint16_t`) with `#pragma pack(push, 1)` for exact 8-byte network wire layout.
3. **Graph Topology:** 2D Adjacency Matrix (`adj[MAX_NODES][MAX_NODES]`) for bi-directional 50m radio links.
4. **Deduplication Cache:** Per-node embedded `DeduplicationCache` struct for node-isolated duplicate message ID drops.
5. **ACK Sequence ID Separation:** Dedicated ACK packet message IDs (`data_msg_id + 10000`) prevent collision with DATA packets in deduplication cache.
6. **Dynamic BFS Fallback:** BFS dynamically queries active matrix edges to discover alternate routes during link failures.
7. **Empirical Benchmarking (No Fake Stats):** Real timestamps (`CLOCK_MONOTONIC`) and transmission accumulators dynamically compute exact PDR and overhead reduction ratios.
