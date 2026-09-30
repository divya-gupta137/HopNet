# HopNet - Engineering Journal & Viva Challenge Log

This document logs critical engineering decisions, memory bugs, protocol edge cases, and viva defense topics encountered during the development of HopNet.

---

## Log Entry #0: Initializing HopNet Development Roadmap
- **Date:** Sept 29, 2026
- **Context:** Project scope definition for 8-day sprint (Deadline: Oct 7, 2026).
- **Core Decision:** Prioritize a robust, fully understood C simulation core with benchmarking over low-level BLE hardware drivers. Hardware drivers introduce high OS non-determinism, while protocol engineering in pure C yields higher viva defense scores and clean systems demonstration.
- **Viva Defense Point:** Explain the separation between **Protocol Layer** (framing, routing, deduplication, store-and-forward) and **Transport Layer** (in-memory simulator vs physical radio).
