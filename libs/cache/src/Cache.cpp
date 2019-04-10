#include <romz/cache/Cache.h>

namespace romz {

namespace Impl {
// Calculates the hash of a page address
static std::size_t calc_hash(uint64_t value)
{
    return static_cast< std::size_t >( value % CacheState::kBucketSize );
}
} // namespace Impl

// The default constructor
Cache::Cache() = default;

// Retrieves a page from the cache, also removes the page from the cache
// and re-inserts it at the front. Returns null if the page was not cached.
Page* Cache::get(uint64_t address)
{
    size_t hash = Impl::calc_hash(address);

    Page *page = state.buckets[hash].get(address);
    if (!page) {
        state.cache_misses++;
        return nullptr;
    }

    // Now re-insert the page at the head of the "totallist", and
    // thus move far away from the tail. The pages at the tail are highest
    // candidates to be deleted when the cache is purged.
    state.totallist.del(page);
    state.totallist.put(page);
    state.cache_hits++;
    return page;
}

// Stores a page in the cache
void Cache::put(Page *page)
{
    size_t hash = Impl::calc_hash(page->address());

    /* First remove the page from the cache, if it's already cached
 *
 * Then re-insert the page at the head of the list. The tail will
 * point to the least recently used page.
 */
    state.totallist.del(page);
    state.totallist.put(page);
    if (page->is_allocated())
        state.alloc_elements++;

    state.buckets[hash].put(page);
}

// Removes a page from the cache
void Cache::del(Page *page)
{
    assert(page->address() != 0);

    /* remove it from the list of all cached pages */
    if (state.totallist.del(page) && page->is_allocated())
        state.alloc_elements--;

    /* remove the page from the cache buckets */
    size_t hash = Impl::calc_hash(page->address());
    state.buckets[hash].del(page);
}

// Purges the cache. Implements a LRU eviction algorithm. Dirty pages are
// forwarded to the |processor()| for flushing.
// The |ignore_page| is passed by the caller; this page will not be purged
// under any circumstance. This is used by the PageManager to make sure
// that the "last blob page" is not evicted by the cache.
void Cache::purge_candidates(std::vector<uint64_t> &candidates,
                      std::vector<Page *> &garbage,
                      Page *ignore_page) {
    int limit = (int)(current_elements()
                      - (state.capacity_bytes / state.page_size_bytes));

    Page *page = state.totallist.tail();
    for (int i = 0; i < limit && page != nullptr; i++) {
        if (page->mutex().try_lock()) {
            if (page->cursor_list.size() == 0
                    && page != ignore_page
                    && page->type() != Page::kTypeBroot) {
                if (page->is_dirty())
                    candidates.push_back(page->address());
                else
                    garbage.push_back(page);
            }
            page->mutex().unlock();
        }

        page = page->previous(Page::kListCache);
    }
}

// Returns true if the capacity limits are exceeded
bool Cache::is_cache_full() const
{
    return state.totallist.size() * state.page_size_bytes
            > state.capacity_bytes;
}

// Returns the capacity (in bytes)
uint64_t Cache::capacity() const
{
    return state.capacity_bytes;
}

// Returns the number of currently cached elements
std::size_t Cache::current_elements() const
{
    return state.totallist.size();
}

///
/// Returns the number of currently cached elements (excluding those that are mmapped)
///
std::size_t Cache::allocated_elements() const
{
    return state.alloc_elements;
}

}
