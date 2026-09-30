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
  6. **Benchmarking Layer:** Measuring Packet Delivery Ratio (PDR), Latency, and Routing Overhead.

## Current Status
- **Phase:** 0 - Project Setup & Architecture Finalization
- **Current Target Date:** Sept 29, 2026
- **Deadline:** Oct 7, 2026 (8-Day Intensive Development)
- **Status:** Initialized architectural plan, master story model, and core configuration files.

## Milestone Progress
- [x] Phase 0: Master Story Analogy, Architecture Design & Repo Blueprint (Sept 29)
- [ ] Phase 1: Repo Setup, Core Structures, Topology Graph & Memory Management (Day 1 - Sept 30)
- [ ] Phase 2: Binary Packet Framing, Serialization & Single-Hop Transport (Day 2 - Oct 1)
- [ ] Phase 3: Multi-Hop Forwarding, TTL Loop Prevention & Deduplication Cache (Day 3 - Oct 2)
- [ ] Phase 4: Routing Protocol Engine (Baseline Flooding vs Shortest Path / Distance Vector) (Day 4 - Oct 3)
- [ ] Phase 5: Circular Store-and-Forward Buffers & Link/Node Failure Simulation (Day 5 - Oct 4)
- [ ] Phase 6: Reliability Layer (ACKs, Retransmission & Route Recovery) (Day 6 - Oct 5)
- [ ] Phase 7: Benchmarking Engine (PDR, Latency, Overhead) & Experiment CLI (Day 7 - Oct 6)
- [ ] Phase 8: Viva Preparation, Code Hardening & GitHub Presentation (Day 8 - Oct 7)

## Architecture Decisions
1. **Simulation-First:** Transport is decoupled from protocol logic. Simulator uses POSIX sockets/in-memory queues. Real Bluetooth is deferred to STRETCH/FUTURE.
2. **Explicit C Systems Focus:** Pointers, structs, manual memory allocation (`malloc/free`), linked lists, circular buffers, graph representations, binary serialization, POSIX threads/mutexes.
3. **Strict Manual Implementation:** AI acts solely as mentor/tutor. User manually edits all code files except `SOUL.md` and `CHALLENGES.md`.

## Next Immediate Logical Step
Create project folder structure (`include/`, `src/`, `theory_concepts/`), initial `.gitignore`, `Makefile`, and `LICENSE`. Then begin Phase 1 data structure design (`node.h`, `packet.h`, `graph.h`).
