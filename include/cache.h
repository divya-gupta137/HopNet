#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_CACHE_SIZE 64

// Memory structure to keep track of seen packet Message IDs
typedef struct {
    uint16_t seen_msg_ids[MAX_CACHE_SIZE];
    int count;
} DeduplicationCache;

void cache_init(DeduplicationCache *cache);
bool cache_contains(const DeduplicationCache *cache, uint16_t msg_id);
void cache_add(DeduplicationCache *cache, uint16_t msg_id);

#endif // CACHE_H
