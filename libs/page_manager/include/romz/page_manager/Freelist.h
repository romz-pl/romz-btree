#pragma once

#include <map>

namespace romz {

struct Freelist
{
    // The freelist maps page-id to number of free pages (usually 1)
    typedef std::map<uint64_t, size_t> FreeMap;

    Freelist();

    void clear();

    bool empty() const;

    // Encodes the freelist's state in |data|. Returns a bool which is set to
    // true if there is additional data, or false if the whole state was
    // encoded.
    // Set |cont.first| to false for the first call.
    std::pair<bool, Freelist::FreeMap::const_iterator>
    encode_state(
            std::pair<bool, Freelist::FreeMap::const_iterator> cont,
            uint8_t *data, size_t data_size);

    // Decodes the freelist's state from raw data and adds it to the internal
    // map
    void decode_state(uint8_t *data);

    // Allocates |num_pages| sequential pages from the freelist; returns the
    // page id of the first page, or 0 if not successfull
    uint64_t alloc(size_t num_pages);

    // Stores a page in the freelist
    void put(uint64_t page_id, size_t page_count);

    // Returns true if a page is in the freelist
    bool has(uint64_t page_id) const;

    // Tries to truncate the file by counting how many pages at the file's end
    // are unused. Returns the address of the last unused page, or |file_size|
    // if there are no unused pages at the end.
    uint64_t truncate(uint64_t file_size);

    // The map with free pages
    FreeMap free_pages;

    // number of successful freelist hits
    uint64_t freelist_hits;

    // number of freelist misses
    uint64_t freelist_misses;
};

}
