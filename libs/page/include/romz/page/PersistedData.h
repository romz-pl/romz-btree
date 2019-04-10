#pragma once

#include <romz/page/PPageData_fwd.h>
#include <cstdint>

class Spinlock
{
public:
    bool try_lock() { return true; }
    void unlock() { }


};

namespace romz {

// A wrapper around the persisted page data
struct PersistedData
{
    PersistedData();

    PersistedData(const PersistedData &other);

    ~PersistedData();

    // The spinlock is locked if the page is in use or written to disk
    Spinlock mutex;

    // address of this page - the absolute offset in the file
    uint64_t address;

    // the size of this page
    uint32_t size;

    // is this page dirty and needs to be flushed to disk?
    bool is_dirty;

    // Page buffer was allocated with malloc() (if not then it was mapped
    // with mmap)
    bool is_allocated;

    // True if page has no persistent header
    bool is_without_header;

    // the persistent data of this page
    PPageData *raw_data;
};

}
