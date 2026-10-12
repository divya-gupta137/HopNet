#include "security.h"
#include <stdio.h>

// In-place byte-varying symmetric stream cipher
void security_xor_cipher(uint8_t *payload, size_t len, uint8_t key) {
    if (payload == NULL || len == 0) return;

    for (size_t i = 0; i < len; i++) {
        // Apply XOR with key + positional variation
        payload[i] ^= (uint8_t)(key + (i % 7));
    }
}

// Visualizes raw byte buffer in memory
void security_print_hex(const char *label, const uint8_t *data, size_t len) {
    if (label != NULL) {
        printf("%s: ", label);
    }
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}
