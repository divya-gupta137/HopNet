#ifndef SECURITY_H
#define SECURITY_H

#include <stdint.h>
#include <stddef.h>

// Shared Secret Key between Alice (Node 1) and David (Node 6)
#define HOPNET_SECRET_KEY 0x5A

// Performs in-place End-to-End Encryption (E2EE) / Decryption using an XOR stream cipher
void security_xor_cipher(uint8_t *payload, size_t len, uint8_t key);

// Utility to display raw byte memory as hexadecimal (e.g., 5A F1 2C ...)
void security_print_hex(const char *label, const uint8_t *data, size_t len);

#endif // SECURITY_H
