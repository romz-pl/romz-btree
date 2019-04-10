#include <romz/page_manager/Pickle.h>

namespace romz {
///
/// encodes a uint64 number and stores it in |p|; returns the number of bytes used
///
std::size_t Pickle::encode_u64(uint8_t *p, uint64_t n)
{
    if (n <= 0xf) {
        *p = static_cast< uint8_t >( n );
        return 1;
    }
    if (n <= 0xff) {
        *(p + 1) = (n & 0xf0) >> 4;
        *(p + 0) = n & 0xf;
        return 2;
    }
    if (n <= 0xfff) {
        *(p + 2) = (n & 0xf00) >> 8;
        *(p + 1) = (n & 0xf0) >> 4;
        *(p + 0) = n & 0xf;
        return 3;
    }
    if (n <= 0xffff) {
        *(p + 3) = (n & 0xf000) >> 12;
        *(p + 2) = (n & 0xf00) >> 8;
        *(p + 1) = (n & 0xf0) >> 4;
        *(p + 0) = n & 0xf;
        return 4;
    }
    if (n <= 0xfffff) {
        *(p + 4) = (n & 0xf0000) >> 16;
        *(p + 3) = (n & 0xf000) >> 12;
        *(p + 2) = (n & 0xf00) >> 8;
        *(p + 1) = (n & 0xf0) >> 4;
        *(p + 0) = n & 0xf;
        return 5;
    }
    if (n <= 0xffffff) {
        *(p + 5) = (n & 0xf00000) >> 24;
        *(p + 4) = (n & 0xf0000) >> 16;
        *(p + 3) = (n & 0xf000) >> 12;
        *(p + 2) = (n & 0xf00) >> 8;
        *(p + 1) = (n & 0xf0) >> 4;
        *(p + 0) = n & 0xf;
        return 6;
    }
    if (n <= 0xfffffff) {
        *(p + 6) = (n & 0xf000000) >> 32;
        *(p + 5) = (n & 0xf00000) >> 24;
        *(p + 4) = (n & 0xf0000) >> 16;
        *(p + 3) = (n & 0xf000) >> 12;
        *(p + 2) = (n & 0xf00) >> 8;
        *(p + 1) = (n & 0xf0) >> 4;
        *(p + 0) = n & 0xf;
        return 7;
    }
    *(p + 7) = (n & 0xf0000000) >> 36;
    *(p + 6) = (n & 0xf000000) >> 32;
    *(p + 5) = (n & 0xf00000) >> 24;
    *(p + 4) = (n & 0xf0000) >> 16;
    *(p + 3) = (n & 0xf000) >> 12;
    *(p + 2) = (n & 0xf00) >> 8;
    *(p + 1) = (n & 0xf0) >> 4;
    *(p + 0) = n & 0xf;
    return 8;
}

///
/// decodes and returns a pickled number of |len| bytes
///
uint64_t Pickle::decode_u64(std::size_t len, uint8_t *p)
{
    uint64_t ret = 0;

    for (std::size_t i = 0; i < len - 1; i++) {
        ret += *(p + (len - i - 1));
        ret <<= 4;
    }

    // last assignment is without *= 10
    return ret + *p;
}


}
