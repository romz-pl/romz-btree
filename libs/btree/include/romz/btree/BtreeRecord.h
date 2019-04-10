#pragma once

namespace romz {

//
// A helper class wrapping record-related constants into a common namespace.
// This class does not contain any logic.
//
struct BtreeRecord {
    enum {
        // record size < 8; length is encoded at byte[7] of key->ptr
        kBlobSizeTiny         = 0x01,

        // record size == 8; record is stored in key->ptr
        kBlobSizeSmall        = 0x02,

        // record size == 0; key->ptr == 0
        kBlobSizeEmpty        = 0x04,

        // key has duplicates in an overflow area; this is the msb of 1 byte;
        // the lower bits are the counter for the inline duplicate list
        kExtendedDuplicates   = 0x80
    };
};

}
