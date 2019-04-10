#pragma once

#include <cstdint>

namespace romz {

///
/// Class for pickling/unpickling data to a buffer
///
struct Pickle
{
    static std::size_t encode_u64(uint8_t *p, uint64_t n);

    static uint64_t decode_u64(std::size_t len, uint8_t *p);
};

}
