#ifndef RELIABILITY_H
#define RELIABILITY_H

#include "packet.h"
#include "graph.h"
#include <stdbool.h>

#define MAX_RETRANSMIT_ATTEMPTS 3

// Creates an ACK (Acknowledgement) packet in response to a received DATA packet
Packet create_ack_packet(const Packet *data_pkt);

// Transmits DATA packet with ACK expectation and automatic retransmission retries
bool send_packet_with_ack(Graph *g, uint8_t src_id, uint8_t dest_id, uint16_t msg_id, const char *msg);

#endif // RELIABILITY_H
