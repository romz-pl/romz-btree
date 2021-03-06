#include <romz/config/EnvConfig.h>
#include <limits>

// the default cache size is 2 MB
#define UPS_DEFAULT_CACHE_SIZE    (2 * 1024 * 1024)

// the default page size is 16 kb
#define UPS_DEFAULT_PAGE_SIZE (16 * 1024)


/** Value for @ref UPS_PARAM_POSIX_FADVISE */
#define UPS_POSIX_FADVICE_NORMAL 0

namespace romz {

EnvConfig::EnvConfig()
    : read_only( false )
    , disable_mmap( false )
    , in_memory( false )
    , file_mode(0644)
    , max_databases(0)
    , page_size_bytes(UPS_DEFAULT_PAGE_SIZE)
    , cache_size_bytes(UPS_DEFAULT_CACHE_SIZE)
    , file_size_limit_bytes(std::numeric_limits<size_t>::max())
    , remote_timeout_sec(0)
    , journal_compressor(0)
    , is_encryption_enabled(false)
    , journal_switch_threshold(0)
    , posix_advice(UPS_POSIX_FADVICE_NORMAL)
{
}

}
