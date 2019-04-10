#pragma once

namespace romz {

//
// A helper class wrapping key-related constants into a common namespace.
// This class does not contain any logic.
//
struct BtreeKey {
    // persisted btree key flags; also used in combination with ups_key_t._flags
    enum {
        // key is extended with overflow area
        kExtendedKey          = 0x01,

        // key is compressed; the original size is stored in the payload
        kCompressed           = 0x08
    };


    // flags used with the ups_key_t::_flags (note the underscore - this
    // field is for INTERNAL USE!)
    //
    // Note: these flags should NOT overlap with the persisted flags above!
    //
    // As these flags NEVER will be persisted, they should be located outside
    // the range of a uint16_t, i.e. outside the mask 0x0000ffff.
    enum {
        // Actual key is lower than the requested key
        kLower               = 0x00010000,

        // Actual key is greater than the requested key
        kGreater             = 0x00020000,

        // Actual key is an "approximate match"
        kApproximate         = (kLower | kGreater)
    };

};

}
