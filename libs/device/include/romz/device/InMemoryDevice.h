#pragma once

#include <romz/device/Device.h>

namespace romz {

///
/// an In-Memory device
///


struct InMemoryDevice : public Device
{
    // constructor
    InMemoryDevice(const EnvConfig &config);

    void create() override;

    void open() override;

    bool is_open() override;

    void close() override;

    void flush() override;

    void truncate(uint64_t newsize) override;

    uint64_t file_size() override;

    void seek(uint64_t offset, int whence) override;

    uint64_t tell() override;

    void read(uint64_t offset, void *buffer, size_t len) override;

    void write(uint64_t offset, void *buffer, size_t len) override;

    void read_page(Page *page, uint64_t address) override;

    uint64_t alloc(size_t size) override;

    void alloc_page(Page *page) override;

    void free_page(Page *page) override;

    bool is_mapped(uint64_t file_offset, size_t size) const override;

    void reclaim_space() override;

    void release(void *ptr, size_t size);

    // flag whether this device was "opened" or is uninitialized
    bool is_open_;

    // the allocated bytes
    uint64_t allocated_size_;
};
}
