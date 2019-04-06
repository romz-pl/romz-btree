#pragma once

#include <romz/page/PPageHeader.h>

namespace romz {

/*
 * A union combining the page header and a pointer to the raw page data.
 *
 * This structure definition is present outside of @ref Page scope
 * to allow compile-time OFFSETOF macros to correctly judge the size,
 * depending on platform and compiler settings.
 */
union PPageData
{
    // the persistent header
    PPageHeader header;

    // a char pointer to the allocated storage on disk
    std::uint8_t payload[1];

};

}
