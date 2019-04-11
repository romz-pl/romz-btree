#pragma once

#include <romz/device/Device.h>
#include <romz/os/File.h>



namespace romz {

class Page;

///
/// Device-implementation for disk-based files. Exception safety is "strong"
/// for most operations, but currently it's possible that the Page is modified
/// if DiskDevice::read_page fails in the middle.
///
/// a File-based device
///
class DiskDevice
{
    constexpr static uint64_t file_size_limit_bytes = 1024 * 1024 * 1024;

public:
    DiskDevice( const std::string& path );
    DiskDevice( const std::string& path, bool read_only );

    void fsync();

    void truncate(uint64_t new_file_size);

    uint64_t file_size();

    void lseek(uint64_t offset) ;

    uint64_t tell();

    void read(uint64_t offset, void *buffer, size_t len) ;

    void write(uint64_t offset, void *buffer, size_t len) ;

    uint64_t alloc(size_t requested_length) ;

    void read_page(Page *page, uint64_t address);

    void alloc_page(Page *page);

    void free_page(Page *page);

    void reclaim_space();

private:
    void truncate_nolock(uint64_t new_file_size);

private:
    // the database file
    File file;

    // excess storage at the end of the file
    std::uint64_t excess_at_end;
};
}
