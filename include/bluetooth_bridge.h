#ifndef BLUETOOTH_BRIDGE_H
#define BLUETOOTH_BRIDGE_H

#include <stdbool.h>
#include <stdint.h>
#include "packet.h"

// Standard Linux Bluetooth Kernel ABI Constants (eliminates external library dependencies)
#ifndef AF_BLUETOOTH
#define AF_BLUETOOTH 31
#endif

#ifndef BTPROTO_RFCOMM
#define BTPROTO_RFCOMM 3
#endif

// 6-byte Bluetooth MAC address representation (bdaddr_t)
typedef struct {
    uint8_t b[6];
} HopNetBdAddr;

// Linux Bluetooth RFCOMM Socket Address Structure (sockaddr_rc)
typedef struct {
    uint16_t rc_family;     // AF_BLUETOOTH
    HopNetBdAddr rc_bdaddr; // Remote/Local 6-byte MAC address
    uint8_t rc_channel;     // RFCOMM Channel number (1 - 30)
} HopNetSockAddrRc;

// Checks whether physical Linux Bluetooth subsystem is active
bool bt_is_hardware_available(void);

// Formats a 6-byte MAC address into human-readable string (e.g. "CC:78:AB:12:34:56")
void bt_format_mac(const HopNetBdAddr *addr, char *out_str);

// Runs the live interactive RFCOMM Socket serialization and transmission demo
void bt_run_live_rfcomm_demo(void);

#endif // BLUETOOTH_BRIDGE_H
