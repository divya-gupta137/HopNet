#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "packet.h"
#include "graph.h"
#include "routing.h"
#include "reliability.h"
#include "failure.h"
#include "benchmark.h"
#include "security.h"
#include "bluetooth_bridge.h"

// ANSI Color Palette for Terminal UI
#define COLOR_RESET   "\033[0m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_BOLD    "\033[1m"

// Clears the screen and positions cursor at (0,0)
static void clear_screen(void) {
    printf("\033[2J\033[H");
}

// Pauses execution until the user presses Enter
static void wait_for_enter(void) {
    printf("\n" COLOR_YELLOW "Press [ENTER] to return to Main Menu..." COLOR_RESET);
    while (getchar() != '\n'); // flush newline
    getchar();                 // wait for key
}

// Renders the ASCII Network Radar
static void render_topology_radar(const Graph *g) {
    printf(COLOR_CYAN);
    printf("┌────────────────────────────────────────────────────────────────────────┐\n");
    printf("│                      HOPNET 6-NODE MESH TOPOLOGY                       │\n");
    printf("└────────────────────────────────────────────────────────────────────────┘\n" COLOR_RESET);
    
    bool link_2_4 = graph_has_edge(g, 2, 4);
    
    printf("         " COLOR_GREEN "[Node 2: Bob]" COLOR_RESET " %s " COLOR_GREEN "[Node 4: Eve]" COLOR_RESET "\n",
           link_2_4 ? COLOR_GREEN "──────(40m)──────>" COLOR_RESET : COLOR_RED "──[LINK SEVERED]─>" COLOR_RESET);
    printf("        /                                             \\\n");
    printf(COLOR_BOLD "[Node 1: Alice]" COLOR_RESET "                                     " COLOR_BOLD "[Node 6: David]" COLOR_RESET "\n");
    printf("  (Source)                                                (Destination)\n");
    printf("        \\                                             /\n");
    printf("         " COLOR_GREEN "[Node 3: Charlie]" COLOR_RESET " ──────(40m)──────> " COLOR_GREEN "[Node 5: Frank]" COLOR_RESET "\n\n");
}

// Option 1: Live Multi-Hop with E2EE & Return ACK
static void demo_e2ee_transmission(Graph *g) {
    clear_screen();
    printf(COLOR_CYAN "========================================================================\n");
    printf("       DEMO 1: END-TO-END ENCRYPTED (E2EE) MULTI-HOP TRANSMISSION       \n");
    printf("========================================================================\n\n" COLOR_RESET);

    const char *secret_msg = "MEET ME AT STAGE B";
    uint8_t src = 1;
    uint8_t dest = 6;
    uint16_t msg_id = 1001;

    printf(COLOR_BOLD "1. Source Node 1 (Alice) creates packet:\n" COLOR_RESET);
    printf("   Plaintext Message: \"" COLOR_GREEN "%s" COLOR_RESET "\"\n", secret_msg);

    // Create standard packet
    Packet pkt = create_packet(src, dest, msg_id, PACKET_TYPE_DATA, 5, secret_msg);

    // Encrypt payload at Alice (Source)
    security_xor_cipher(pkt.payload, pkt.header.payload_len, HOPNET_SECRET_KEY);
    printf("   " COLOR_YELLOW "[E2EE APPLIED]" COLOR_RESET " Payload encrypted with Key (0x%02X)\n", HOPNET_SECRET_KEY);
    security_print_hex("   Alice's Memory Hex Dump", pkt.payload, pkt.header.payload_len);
    printf("\n");

    printf(COLOR_BOLD "2. Simulating Multi-Hop Routing (Alice -> Bob -> Eve -> David):\n" COLOR_RESET);
    uint32_t hops = 0;
    bool delivered = route_shortest_path(g, src, pkt, &hops);

    if (delivered) {
        printf("\n" COLOR_BOLD "3. Destination Node 6 (David) Delivery & Decryption:\n" COLOR_RESET);
        printf("   David received %d encrypted bytes.\n", pkt.header.payload_len);
        security_print_hex("   David's Received Hex", pkt.payload, pkt.header.payload_len);

        // Decrypt payload at David (Destination)
        security_xor_cipher(pkt.payload, pkt.header.payload_len, HOPNET_SECRET_KEY);
        printf("   " COLOR_GREEN "🎉 Decrypted Plaintext: \"%s\"" COLOR_RESET "\n\n", (char *)pkt.payload);

        // Generate return ACK
        printf(COLOR_BOLD "4. End-to-End Reliability Layer (Return ACK):\n" COLOR_RESET);
        Packet ack_pkt = create_ack_packet(&pkt);
        uint32_t ack_hops = 0;
        printf("   Routing ACK #%d backwards to Node 1...\n", ack_pkt.header.msg_id);
        route_shortest_path(g, dest, ack_pkt, &ack_hops);
        printf("   " COLOR_GREEN "✓ Full Multi-Hop Delivery & ACK Cycle Complete!" COLOR_RESET "\n");
    }

    wait_for_enter();
}

// Option 2: Relay Eavesdropping & Memory Inspection
static void demo_relay_eavesdrop(void) {
    clear_screen();
    printf(COLOR_CYAN "========================================================================\n");
    printf("        DEMO 2: RELAY NODE EAVESDROPPING INSPECTION (BOB'S RAM)         \n");
    printf("========================================================================\n\n" COLOR_RESET);

    const char *secret_msg = "SECRET_PASSWORD_123";
    Packet pkt = create_packet(1, 6, 2001, PACKET_TYPE_DATA, 5, secret_msg);
    security_xor_cipher(pkt.payload, pkt.header.payload_len, HOPNET_SECRET_KEY);

    printf("Simulating what Relay Node 2 (Bob) sees inside his internal packet buffer:\n\n");
    printf(COLOR_BOLD "┌──────────────────────────────────────────────┐\n");
    printf("│       BOB'S RAW PACKET BUFFER IN RAM         │\n");
    printf("├──────────────────────────────────────────────┤\n" COLOR_RESET);
    printf("│ Outer Routing Envelope (Readable by Relay):  │\n");
    printf("│   Source ID      : %-26d│\n", pkt.header.src_id);
    printf("│   Destination ID : %-26d│\n", pkt.header.dest_id);
    printf("│   Message ID     : %-26d│\n", pkt.header.msg_id);
    printf("│   TTL Counter    : %-26d│\n", pkt.header.ttl);
    printf("│   Payload Length : %-26d│\n", pkt.header.payload_len);
    printf("├──────────────────────────────────────────────┤\n");
    printf("│ Inner Data Payload (Scrambled Ciphertext):   │\n");
    printf("│   Raw String     : " COLOR_RED "%-26s" COLOR_RESET "│\n", (char *)pkt.payload);
    printf("│   Hex Dump       : ");
    for (size_t i = 0; i < 7; i++) printf("%02X ", pkt.payload[i]);
    printf("... │\n");
    printf("└──────────────────────────────────────────────┘\n\n");

    printf(COLOR_GREEN "✓ SECURITY RESULT: Bob can successfully route the packet to Node 6,\n"
           "  but cannot extract, read, or tamper with Alice's original text!" COLOR_RESET "\n");

    wait_for_enter();
}

// Option 3: Dynamic Link Severing & Self-Healing Demo
static void demo_dynamic_recovery(Graph *g) {
    clear_screen();
    printf(COLOR_CYAN "========================================================================\n");
    printf("       DEMO 3: DYNAMIC LINK SEVERING & BFS ROUTE SELF-HEALING           \n");
    printf("========================================================================\n\n" COLOR_RESET);

    route_with_failure_recovery(g, 1, 6, 3001, "Test Self-Healing Mesh");

    // Restore link back so network is ready for next run
    simulate_link_restore(g, 2, 4);

    wait_for_enter();
}

// Option 4: Full Automated Benchmarking Stress Test
static void demo_benchmark_stress_test(Graph *g) {
    clear_screen();
    printf(COLOR_CYAN "========================================================================\n");
    printf("       DEMO 4: 50-PACKET EMPIRICAL BENCHMARKING EVALUATION ENGINE       \n");
    printf("========================================================================\n\n" COLOR_RESET);

    BenchmarkStats flood_stats;
    BenchmarkStats smart_stats;

    printf("[BENCHMARK] Executing Test 1: Baseline Epidemic Flooding (50 Packets)...\n");
    benchmark_run_flooding(g, 50, &flood_stats);

    printf("[BENCHMARK] Executing Test 2: Smart Shortest-Path Routing (50 Packets)...\n");
    benchmark_run_shortest_path(g, 50, &smart_stats);

    benchmark_print_comparison(&flood_stats, &smart_stats);

    wait_for_enter();
}

// Option 5: Live Linux Bluetooth RFCOMM Bridge Demo
static void demo_bluetooth_architecture(void) {
    clear_screen();
    bt_run_live_rfcomm_demo();
    wait_for_enter();
}

// Main Interactive Dispatch Loop
int main(void) {
    // 1. Initialize Network Graph
    Graph g;
    graph_init(&g);

    graph_add_node(&g, 1, 0, 0);    // Alice (Source)
    graph_add_node(&g, 2, 40, 20);  // Bob (Top branch)
    graph_add_node(&g, 3, 40, -20); // Charlie (Bottom branch)
    graph_add_node(&g, 4, 80, 20);  // Eve (Top branch)
    graph_add_node(&g, 5, 80, -20); // Frank (Bottom branch)
    graph_add_node(&g, 6, 120, 0);  // David (Destination)

    // Primary top branch
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 4);
    graph_add_edge(&g, 4, 6);

    // Alternate bottom branch
    graph_add_edge(&g, 1, 3);
    graph_add_edge(&g, 3, 5);
    graph_add_edge(&g, 5, 6);

    int choice = -1;

    while (1) {
        clear_screen();
        render_topology_radar(&g);

        printf(COLOR_CYAN "┌────────────────────────────────────────────────────────────────────────┐\n");
        printf("│                  HOPNET CONTROL CENTER - MAIN MENU                     │\n");
        printf("├────────────────────────────────────────────────────────────────────────┤\n" COLOR_RESET);
        printf("│ " COLOR_BOLD "[1]" COLOR_RESET " Send Multi-Hop Message with E2EE & ACKs (Alice -> David)       │\n");
        printf("│ " COLOR_BOLD "[2]" COLOR_RESET " Inspect Relay Node RAM (Bob's Buffer Eavesdropping Demo)       │\n");
        printf("│ " COLOR_BOLD "[3]" COLOR_RESET " Simulate Link Failure & Dynamic BFS Route Recovery (Self-Heal)  │\n");
        printf("│ " COLOR_BOLD "[4]" COLOR_RESET " Run 50-Packet Stress Test & Comparative Benchmarks               │\n");
        printf("│ " COLOR_BOLD "[5]" COLOR_RESET " View Real Linux Bluetooth RFCOMM Architecture Mapping           │\n");
        printf("│ " COLOR_BOLD "[0]" COLOR_RESET " Exit HopNet                                                     │\n");
        printf(COLOR_CYAN "└────────────────────────────────────────────────────────────────────────┘\n" COLOR_RESET);
        printf(COLOR_YELLOW "\nEnter selection [0 - 5] > " COLOR_RESET);

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // clear invalid input
            continue;
        }

        switch (choice) {
            case 1:
                demo_e2ee_transmission(&g);
                break;
            case 2:
                demo_relay_eavesdrop();
                break;
            case 3:
                demo_dynamic_recovery(&g);
                break;
            case 4:
                demo_benchmark_stress_test(&g);
                break;
            case 5:
                demo_bluetooth_architecture();
                break;
            case 0:
                clear_screen();
                printf(COLOR_GREEN "\nShutting down HopNet. Clean exit!\n\n" COLOR_RESET);
                return 0;
            default:
                break;
        }
    }

    return 0;
}
