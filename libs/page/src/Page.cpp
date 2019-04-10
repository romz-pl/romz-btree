#include <romz/page/Page.h>
#include <romz/device/Device.h>
#include <romz/murmur/MurmurHash3.h>
#include <cassert>
#include <cstring>

namespace romz {

uint64_t Page::ms_page_count_flushed = 0;

Page::Page(Device *device, LocalDb *db)
    : device_(device)
    , db_(db)
    , node_proxy_( nullptr )
{
    persisted_data.raw_data = nullptr;
    persisted_data.is_dirty = false;
    persisted_data.is_allocated = false;
    persisted_data.address  = 0;
    persisted_data.size     = (uint32_t)device->page_size();
}

Page::~Page()
{
    assert(cursor_list.is_empty());
    free_buffer();
}

uint32_t
Page::usable_page_size()
{
    // uint32_t raw_page_size = db_->env->config.page_size_bytes;
    // return raw_page_size - Page::kSizeofPersistentHeader;
    return page_size_bytes - Page::kSizeofPersistentHeader;
}

void
Page::alloc(uint32_t type, uint32_t flags)
{
    device_->alloc_page(this);

    if (flags & kInitializeWithZeroes) {
        size_t page_size = device_->page_size();
        std::memset(raw_payload(), 0, page_size);
    }

    if (type)
        set_type(type);
}

void
Page::fetch(uint64_t address)
{
    device_->read_page(this, address);
    set_address(address);
}

void
Page::flush()
{
    if (persisted_data.is_dirty) {
        // update crc32
        if (enable_crc32 && !persisted_data.is_without_header) {
            MurmurHash3_x86_32(persisted_data.raw_data->header.payload,
                               persisted_data.size - (sizeof(PPageHeader) - 1),
                               (uint32_t)persisted_data.address,
                               &persisted_data.raw_data->header.crc32);
        }
        device_->write(persisted_data.address, persisted_data.raw_data,
                       persisted_data.size);
        persisted_data.is_dirty = false;
        ms_page_count_flushed++;
    }
}

void
Page::free_buffer()
{
    if (node_proxy_) {
        delete node_proxy_;
        node_proxy_ = nullptr;
    }
}

}
