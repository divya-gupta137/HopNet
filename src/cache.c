#include "cache.h"
#include <string.h>

void cache_init(DeduplicationCache *cache) {
    if (cache == NULL) return;
    cache->count = 0;
    memset(cache->seen_msg_ids, 0, sizeof(cache->seen_msg_ids));
}

bool cache_contains(const DeduplicationCache *cache, uint16_t msg_id) {
    if (cache == NULL) return false;
    for (int i = 0; i < cache->count; i++) {
        if (cache->seen_msg_ids[i] == msg_id) {
            return true; // Already seen!
        }
    }
    return false; // New packet
}

void cache_add(DeduplicationCache *cache, uint16_t msg_id) {
    if (cache == NULL) return;
    
    if (cache_contains(cache, msg_id)) return;

    // Simple FIFO replacement if cache is full
    if (cache->count < MAX_CACHE_SIZE) {
        cache->seen_msg_ids[cache->count++] = msg_id;
    } else {
        // Shift old entries left (discard oldest seen ID)
        for (int i = 0; i < MAX_CACHE_SIZE - 1; i++) {
            cache->seen_msg_ids[i] = cache->seen_msg_ids[i + 1];
        }
        cache->seen_msg_ids[MAX_CACHE_SIZE - 1] = msg_id;
    }
}
