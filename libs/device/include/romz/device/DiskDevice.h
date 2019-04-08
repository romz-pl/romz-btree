#pragma once

#include <romz/device/Device.h>
#include <romz/device/DiskState.h>
#include <romz/os/File.h>

// Tylko dla Spinlok - usunac
#include <romz/page/Page.h>

namespace romz {





///
/// Device-implementation for disk-based files. Exception safety is "strong"
/// for most operations, but currently it's possible that the Page is modified
/// if DiskDevice::read_page fails in the middle.
///
/// a File-based device
///
class DiskDevice : public Device {


public:
    DiskDevice(const EnvConfig &config_);

    void create() override;
    void open() override;
    bool is_open() override;

    void close() override;

    void flush() override;

    void truncate(uint64_t new_file_size) override;

    uint64_t file_size() override;

    void seek(uint64_t offset, int whence) override;

    uint64_t tell() override;

    void read(uint64_t offset, void *buffer, size_t len) override;

    void write(uint64_t offset, void *buffer, size_t len) override;

    uint64_t alloc(size_t requested_length) override;

    void read_page(Page *page, uint64_t address) override;

    void alloc_page(Page *page) override;

    void free_page(Page *page) override;

    bool is_mapped(uint64_t file_offset, size_t size) const override;

    void reclaim_space() override;

    uint8_t *mapped_pointer(uint64_t address) const;

private:
    void truncate_nolock(uint64_t new_file_size);

private:
    // For synchronizing access
    Spinlock m_mutex;

    DiskState m_state;
};
}
