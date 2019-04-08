#pragma once

namespace romz {

enum class key_types {
    /** A binary blob without type; sorted by memcmp */
    UPS_TYPE_BINARY = 0,

    /** A binary blob without type; sorted by callback function */
    UPS_TYPE_CUSTOM = 1,

    /** An unsigned 8-bit integer */
    UPS_TYPE_UINT8 = 3,

    /** An unsigned 16-bit integer */
    UPS_TYPE_UINT16 = 5,

    /** An unsigned 32-bit integer */
    UPS_TYPE_UINT32 = 7,

    /** An unsigned 64-bit integer */
    UPS_TYPE_UINT64 = 9,

    /** An 32-bit float */
    UPS_TYPE_REAL32 = 11,

    /** An 64-bit double */
    UPS_TYPE_REAL64  = 12
};

}
