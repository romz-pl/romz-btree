#pragma once

#include <cstdint>

namespace romz {

/*
 * This header is only available if the (non-persistent) flag
 * kNpersNoHeader is not set! Blob pages do not have this header.
 */
struct  PPageHeader {
    // flags of this page - currently only used for the Page::kType* codes
    std::uint32_t flags;

    // crc32
    std::uint32_t crc32;

    // the lsn of the last operation
    std::uint64_t lsn;

    // the persistent data blob
    std::uint8_t payload[1];

};

}
