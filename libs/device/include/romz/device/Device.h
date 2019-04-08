#pragma once

#include <romz/config/EnvConfig.h>
#include <romz/page/Page_fwd.h>

namespace romz {


///
/// Device management; a device encapsulates the physical device, either a
/// file or memory chunks (for in-memory-databases)
///
///
struct Device
{
    // Constructor
    Device(const EnvConfig &config);

    // virtual destructor
    virtual ~Device();

    // Returns the current page size
    std::size_t page_size() const;

    // Create a new device - called in ups_env_create
    virtual void create() = 0;

    // Opens an existing device - called in ups_env_open
    virtual void open() = 0;

    // Returns true if the device is open
    virtual bool is_open() = 0;

    // Closes the device - called in ups_env_close
    virtual void close() = 0;

    // Flushes the device - called in ups_env_flush
    virtual void flush() = 0;

    // Truncate/resize the device
    virtual void truncate(std::uint64_t new_size) = 0;

    // Returns the current file/storage size
    virtual std::uint64_t file_size() = 0;

    // Seek position in a file
    virtual void seek(std::uint64_t offset, int whence) = 0;

    // Tell the position in a file
    virtual std::uint64_t tell() = 0;

    // Reads from the device; this function does not use mmap
    virtual void read(std::uint64_t offset, void *buffer, std::size_t len) = 0;

    // Writes to the device; this function does not use mmap
    virtual void write(std::uint64_t offset, void *buffer, std::size_t len) = 0;

    // Allocate storage from this device; this function
    // will *NOT* use mmap. returns the offset of the allocated storage.
    virtual std::uint64_t alloc(std::size_t len) = 0;

    // Reads a page from the device; this function CAN use mmap
    virtual void read_page(Page *page, std::uint64_t address) = 0;

    // Allocate storage for a page from this device; this function
    // can use mmap if available
    virtual void alloc_page(Page *page) = 0;

    // Frees a page on the device.
    // The caller is responsible for flushing the page; the @ref free_page
    // function will assert that the page is not dirty.
    virtual void free_page(Page *page) = 0;

    // Returns true if the specified range is in mapped memory
    virtual bool is_mapped(std::uint64_t file_offset, std::size_t size) const = 0;

    // Removes unused space at the end of the file
    virtual void reclaim_space() = 0;

    // the Environment configuration settings
    const EnvConfig &config;
};

}
