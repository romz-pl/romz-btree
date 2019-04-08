#include <romz/config/DbConfig.h>
#include <limits>

namespace romz {

DbConfig::DbConfig( std::uint16_t db_name_)
    : db_name(db_name_)
    , flags(0)
    , key_type( key_types::UPS_TYPE_BINARY )
    , key_size( std::numeric_limits< std::uint32_t >::max() )
    , record_type( key_types::UPS_TYPE_BINARY )
    , record_size( std::numeric_limits< std::uint32_t >::max() )
    , key_compressor(0)
    , record_compressor(0)
{
}
}
