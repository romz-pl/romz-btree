#pragma once

#include <cstdint>

namespace romz {

//
// A generic exception for storing a status code
//
struct Exception
{
    Exception( std::uint32_t st);

    std::uint32_t code;
};

}
