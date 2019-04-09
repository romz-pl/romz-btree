#pragma once

#include <cstdint>
#include <string>

namespace romz {

///
/// The configuration settings of an Environment.
///
struct EnvConfig
{
    // Constructor initializes with default values
    EnvConfig();

    ///
    /// Opens the file for reading only.
    ///
    bool read_only;

    ///
    /// Do not use memory mapped files for I/O.
    /// By default, upscaledb checks if it can use mmap, since mmap is faster than read/write.
    /// For performance reasons, this flag should not be used.
    ///
    bool disable_mmap;

    ///
    /// Creates an In-Memory Environment.
    /// No file will be created, and the Database contents are lost after the Environment is closed.
    /// The filename parameter can be NULL.
    /// Do NOT specify a cache_size other than 0.
    ///
    bool in_memory;


    // the file mode
    int file_mode;

    // the number of databases
    int max_databases;

    // the page size (in bytes)
    std::uint32_t page_size_bytes;

    // the cache size (in bytes)
    std::uint64_t cache_size_bytes;

    // the file size limit (in bytes)
    std::size_t file_size_limit_bytes;

    // the remote timeout (in seconds)
    std::size_t remote_timeout_sec;

    // the path (or remote location)
    std::string filename;

    // the path of the logfile
    std::string log_filename;

    // the algorithm for journal compression
    int journal_compressor;

    // true if AES encryption is enabled
    bool is_encryption_enabled;

    // the AES encryption key
    std::uint8_t encryption_key[16];

    // threshold for switching journal files
    std::size_t journal_switch_threshold;

    // parameter for posix_fadvise()
    int posix_advice;
};


}
