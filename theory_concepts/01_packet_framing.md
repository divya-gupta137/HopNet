# Concept 01: Packet Framing & Struct Padding in C

## 1. What is Packet Framing?
Packet framing is the process of structuring raw binary data into standardized headers and payloads so network nodes can interpret sender, destination, hop count, and content cleanly.

## 2. Why Fixed-Width Types (<stdint.h>)?
- Standard C types (`int`, `long`) vary in size across operating systems and hardware architectures.
- Protocol headers must be byte-exact across all nodes. We use:
  - `uint8_t`: 8-bit unsigned integer (1 byte, range 0–255)
  - `uint16_t`: 16-bit unsigned integer (2 bytes, range 0–65,535)

## 3. Struct Padding & Memory Alignment
CPUs fetch memory faster when variables align with processor word boundaries.

### Unpacked Header (Compiler inserts padding):
