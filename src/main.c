// #include <stdio.h>
// #include "packet.h"

// int main(void) {
//     printf("===========================================\n");
//     printf("   HopNet Packet Framing & Serialization   \n");
//     printf("===========================================\n\n");

//     // 1. Create a packet from Alice (Node 1) to David (Node 4)
//     Packet orig_pkt = create_packet(1, 4, 1001, PACKET_TYPE_DATA, 5, "Meet me at the food court!");

//     printf("[TX] Original Envelope Created:\n");
//     printf("     Src ID: %d | Dest ID: %d | Msg ID: %d | Type: %d | TTL: %d | Payload Len: %d\n",
//            orig_pkt.header.src_id, orig_pkt.header.dest_id, orig_pkt.header.msg_id,
//            orig_pkt.header.type, orig_pkt.header.ttl, orig_pkt.header.payload_len);
//     printf("     Payload: \"%s\"\n\n", (char *)orig_pkt.payload);

//     // 2. Serialize into raw byte array
//     uint8_t raw_wire_buffer[512];
//     int bytes_serialized = serialize_packet(&orig_pkt, raw_wire_buffer, sizeof(raw_wire_buffer));

//     if (bytes_serialized < 0) {
//         printf("Error: Serialization failed!\n");
//         return 1;
//     }
//     printf("[WIRE] Serialized to %d contiguous wire bytes.\n\n", bytes_serialized);

//     // 3. Deserialize back into a new Rx Packet struct
//     Packet rx_pkt;
//     if (deserialize_packet(raw_wire_buffer, bytes_serialized, &rx_pkt) == 0) {
//         printf("[RX] Deserialized Envelope at Destination:\n");
//         printf("     Src ID: %d | Dest ID: %d | Msg ID: %d | Type: %d | TTL: %d | Payload Len: %d\n",
//                rx_pkt.header.src_id, rx_pkt.header.dest_id, rx_pkt.header.msg_id,
//                rx_pkt.header.type, rx_pkt.header.ttl, rx_pkt.header.payload_len);
//         printf("     Payload: \"%s\"\n\n", (char *)rx_pkt.payload);
//         printf("✅ SUCCESS: Packet created, serialized, and deserialized perfectly!\n");
//     } else {
//         printf("❌ ERROR: Packet deserialization failed!\n");
//         return 1;
//     }

//     return 0;
// }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// #include <stdio.h>
// #include "packet.h"
// #include "graph.h"

// int main(void) {
//     printf("===========================================\n");
//     printf("     HopNet Phase 1: Graph Topology        \n");
//     printf("===========================================\n");

//     Graph festival_map;
//     graph_init(&festival_map);

//     // Add 4 Nodes: Alice (1), Bob (2), Charlie (3), David (4)
//     graph_add_node(&festival_map, 1, 0, 0);   // Alice
//     graph_add_node(&festival_map, 2, 40, 0);  // Bob
//     graph_add_node(&festival_map, 3, 80, 0);  // Charlie
//     graph_add_node(&festival_map, 4, 120, 0); // David

//     // Connect 50m Radio Links: Alice <-> Bob <-> Charlie <-> David
//     graph_add_edge(&festival_map, 1, 2); // Alice <-> Bob
//     graph_add_edge(&festival_map, 2, 3); // Bob <-> Charlie
//     graph_add_edge(&festival_map, 3, 4); // Charlie <-> David

//     // Print the network map
//     graph_print(&festival_map);

//     // Check direct connectivity
//     printf("Direct Link Checks:\n");
//     printf("  Can Alice (1) reach Bob (2)?     %s\n", graph_has_edge(&festival_map, 1, 2) ? "YES" : "NO");
//     printf("  Can Alice (1) reach David (4)?   %s (Requires Multi-Hop Relay!)\n", graph_has_edge(&festival_map, 1, 4) ? "YES" : "NO");

//     return 0;
// }




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// #include <stdio.h>
// #include "packet.h"
// #include "graph.h"
// #include "forwarding.h"

// int main(void) {
//     printf("===========================================\n");
//     printf("   HopNet Phase 2: Multi-Hop Forwarding    \n");
//     printf("===========================================\n\n");

//     // 1. Setup Festival Topology: 1 (Alice) -> 2 (Bob) -> 3 (Charlie) -> 4 (David)
//     Graph g;
//     graph_init(&g);
//     graph_add_node(&g, 1, 0, 0);   // Alice
//     graph_add_node(&g, 2, 40, 0);  // Bob
//     graph_add_node(&g, 3, 80, 0);  // Charlie
//     graph_add_node(&g, 4, 120, 0); // David

//     graph_add_edge(&g, 1, 2);
//     graph_add_edge(&g, 2, 3);
//     graph_add_edge(&g, 3, 4);

//     graph_print(&g);

//     // TEST 1: Successful Multi-Hop Delivery (Alice -> Bob -> Charlie -> David) with TTL = 5
//     printf("--- TEST 1: Sending Packet from Alice (1) to David (4) [TTL = 5] ---\n");
//     Packet pkt1 = create_packet(1, 4, 1001, PACKET_TYPE_DATA, 5, "Meet me at the food court!");
//     process_packet(&g, 1, pkt1);

//     // TEST 2: TTL Expiration Drop Test (Alice -> Bob -> Charlie, but TTL = 2, so it dies before David!)
//     printf("--- TEST 2: Low TTL Loop-Prevention Drop Test [TTL = 2] ---\n");
//     Packet pkt2 = create_packet(1, 4, 1002, PACKET_TYPE_DATA, 2, "This message will expire early!");
//     process_packet(&g, 1, pkt2);

//     return 0;
// }



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




#include <stdio.h>
#include "packet.h"
#include "graph.h"
#include "forwarding.h"

int main(void) {
    printf("===========================================\n");
    printf("  HopNet Phase 2: Duplicate Suppression    \n");
    printf("===========================================\n\n");

    Graph g;
    graph_init(&g);
    graph_add_node(&g, 1, 0, 0);   // Alice
    graph_add_node(&g, 2, 40, 0);  // Bob
    graph_add_node(&g, 3, 80, 0);  // Charlie
    graph_add_node(&g, 4, 120, 0); // David

    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 3, 4);

    graph_print(&g);

    // TEST 1: First send of Packet #2001 (Should succeed: Node 1 -> 2 -> 3 -> 4)
    printf("--- TEST 1: Sending Packet #2001 (First Time) ---\n");
    Packet pkt1 = create_packet(1, 4, 2001, PACKET_TYPE_DATA, 5, "Hello David!");
    process_packet(&g, 1, pkt1);

    // TEST 2: Re-sending Packet #2001 to Node 1 (Should trigger DUPLICATE DROP!)
    printf("--- TEST 2: Re-sending Packet #2001 to Node 1 (Duplicate Test) ---\n");
    process_packet(&g, 1, pkt1);

    return 0;
}
