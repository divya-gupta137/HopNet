#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "graph.h"
#include <stdint.h>

// Metrics recorded during simulation benchmark runs
typedef struct {
    uint32_t packets_sent;
    uint32_t packets_delivered;
    uint32_t packets_dropped;
    uint32_t total_transmissions; // Total broadcasts/relays crowd-wide
    double   total_time_ms;       // Total simulation execution time in milliseconds
    float    avg_hops;            // Average hops per delivered packet
    float    pdr;                 // Packet Delivery Ratio (%)
    float    overhead_ratio;      // Total Transmissions / Delivered Packets
} BenchmarkStats;

void benchmark_init(BenchmarkStats *stats);
void benchmark_run_flooding(Graph *g, int packet_count, BenchmarkStats *stats);
void benchmark_run_shortest_path(Graph *g, int packet_count, BenchmarkStats *stats);
void benchmark_print_comparison(const BenchmarkStats *flood, const BenchmarkStats *smart);

#endif // BENCHMARK_H
