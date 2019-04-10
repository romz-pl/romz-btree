#pragma once

#include <romz/cache/CacheState.h>

namespace romz {



struct Cache
{
    template<typename Purger>
    struct PurgeIfSelector {
        PurgeIfSelector(Cache *cache, Purger &purger)
            : cache_(cache), purger_(purger) {
        }

        bool operator()(Page *page) {
            if (purger_(page))
                cache_->del(page);
            // don't remove page from list; it was already removed above
            return false;
        }

        Cache *cache_;
        Purger &purger_;
    };


    Cache();

    Page *get(uint64_t address);
    void put(Page *page);

    void del(Page *page);

    void purge_candidates(std::vector<uint64_t> &candidates,
                          std::vector<Page *> &garbage,
                          Page *ignore_page);

    // Visits all pages in the "totallist". If |cb| returns true then the
    // page is removed and deleted. This is used by the Environment
    // to flush (and delete) pages.
    template<typename Purger>
    void purge_if(Purger &purger) {
        PurgeIfSelector<Purger> selector(this, purger);
        state.totallist.extract(selector);
    }

    bool is_cache_full() const;


    uint64_t capacity() const;

    std::size_t current_elements() const;

    std::size_t allocated_elements() const;


    CacheState state;
};

}
