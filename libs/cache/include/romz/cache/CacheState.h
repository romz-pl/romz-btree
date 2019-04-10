#pragma once

#include <romz/page/PageCollection.h>
#include <vector>

namespace romz {

struct CacheState
{
    constexpr static uint64_t cache_size_bytes = 100 * 1024 * 1024;

    typedef PageCollection<Page::kListBucket> CacheLine;

    enum {
        // The number of buckets should be a prime number or similar, as it
        // is used in a MODULO hash scheme
        kBucketSize = 10317,
    };

    CacheState();

    // the capacity (in bytes)
    uint64_t capacity_bytes;

    // the current page size (in bytes)
    uint64_t page_size_bytes;

    // the current number of cached elements that were allocated (and not
    // mapped)
    std::size_t alloc_elements;

    // linked list of ALL cached pages
    PageCollection<Page::kListCache> totallist;

    // The hash table buckets - each is a linked list of Page pointers
    std::vector<CacheLine> buckets;

    // counts the cache hits
    uint64_t cache_hits;

    // counts the cache misses
    uint64_t cache_misses;
};

}
