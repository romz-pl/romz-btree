#include <romz/cache/CacheState.h>
#include <cassert>

namespace romz {

CacheState::CacheState()
    : capacity_bytes(cache_size_bytes)
    , page_size_bytes(Page::page_size_bytes)
    , alloc_elements(0)
    , buckets(kBucketSize)
    , cache_hits(0)
    , cache_misses(0)
{
    assert(capacity_bytes > 0);
}

}
