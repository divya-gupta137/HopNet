# Concept 05: Empirical Systems Benchmarking & Network Metrics in C

## 1. Why Empirical Benchmarking?
In systems engineering and viva defenses, claims such as *"Our protocol is 75% faster"* must be backed by **real measurements from automated experiments**, not hardcoded assumptions.

## 2. Core Network Metrics Measured
1. **Packet Delivery Ratio (PDR):**
   $$\text{PDR} = \left(\frac{\text{Delivered Packets}}{\text{Sent Packets}}\right) \times 100\%$$
2. **Routing Overhead Ratio:**
   $$\text{Overhead} = \frac{\text{Total Relays / Broadcasts Crowd-Wide}}{\text{Delivered Packets}}$$
3. **Average Hop Count:**
   The average number of hops taken per delivered packet.
4. **Latency Measurement via `CLOCK_MONOTONIC`:**
   Using POSIX `clock_gettime(CLOCK_MONOTONIC, &ts)` guarantees monotonic, forward-only nanosecond timestamp precision, unaffected by OS NTP time jumps.

## 3. Likely Viva Questions
1. **Q: Why is Flooding overhead significantly higher than Shortest Path routing?**
   - **A:** Flooding broadcasts $O(N^2)$ duplicate packets across all neighboring branches simultaneously, whereas Shortest Path routing unicasts packets along a single linear path.
2. **Q: What is the difference between `CLOCK_REALTIME` and `CLOCK_MONOTONIC`?**
   - **A:** `CLOCK_REALTIME` reflects wall-clock calendar time (and can jump if the user or NTP adjusts time), whereas `CLOCK_MONOTONIC` measures continuous elapsed processor time since boot, ideal for benchmarking.
