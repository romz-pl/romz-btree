#pragma once

#include <string>
#include <cstdint>
#include <romz/config/key_types.h>

namespace romz {

///
/// The configuration settings of a Database.
///
struct DbConfig
{
    // Constructor initializes with default values
    DbConfig( std::uint16_t db_name_ = 0);

    // the database name
    std::uint16_t db_name;

    // the database flags
    std::uint32_t flags;

    // the key type
    key_types key_type;

    // the key size (if specified)
    std::uint32_t key_size;

    // the record type
    key_types record_type;

    // the record size (if specified)
    std::uint32_t record_size;

    // the algorithm for key compression
    int key_compressor;

    // the algorithm for record compression
    int record_compressor;

    // the name of the custom compare callback function
    std::string compare_name;
};


}
