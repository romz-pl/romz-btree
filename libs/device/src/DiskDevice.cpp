#include <romz/device/DiskDevice.h>
#include <romz/page/Page.h>
#include <romz/memory/Memory.h>
#include <cassert>
#include <stdexcept>
#include <romz/page/Page.h>

namespace romz {


DiskDevice::DiskDevice( const std::string& path )
    : file( path )
    , excess_at_end( 0 )
{

}

DiskDevice::DiskDevice( const std::string& path, bool read_only )
    : file( path, read_only )
    , excess_at_end( 0 )
{

}


// flushes the device
void DiskDevice::fsync()
{
    file.fsync();
}

// truncate/resize the device
void DiskDevice::truncate(uint64_t new_file_size)
{
    truncate_nolock(new_file_size);
}

// get the current file/storage size
uint64_t DiskDevice::file_size()
{
    return file.file_size();
}

// seek to a position in a file
void DiskDevice::lseek(uint64_t offset)
{
    file.lseek(offset);
}

// tell the position in a file
uint64_t DiskDevice::tell()
{
    return file.tell();
}

// reads from the device; this function does NOT use mmap
void DiskDevice::read(uint64_t offset, void *buffer, size_t len)
{
    file.pread(buffer, len, offset);
#ifdef UPS_ENABLE_ENCRYPTION
    if (config.is_encryption_enabled) {
        AesCipher aes(config.encryption_key, offset);
        aes.decrypt((uint8_t *)buffer, (uint8_t *)buffer, len);
    }
#endif
}

// writes to the device; this function does not use mmap,
// and is responsible for writing the data is run through the file
// filters
void DiskDevice::write(uint64_t offset, void *buffer, size_t len)
{
#ifdef UPS_ENABLE_ENCRYPTION
    if (config.is_encryption_enabled) {
        // encryption disables direct I/O -> only full pages are allowed
        assert(offset % len == 0);

        uint8_t *encryption_buffer = (uint8_t *)::alloca(len);
        AesCipher aes(config.encryption_key, offset);
        aes.encrypt((uint8_t *)buffer, encryption_buffer, len);
        m_state.file.pwrite(offset, encryption_buffer, len);
        return;
    }
#endif
    file.pwrite(buffer, len, offset);
}

// allocate storage from this device; this function
// will *NOT* return mmapped memory
uint64_t DiskDevice::alloc(size_t requested_length)
{
    uint64_t address;

    if (excess_at_end >= requested_length) {
        address = file_size() - excess_at_end;
        excess_at_end -= requested_length;
    }
    else {
        uint64_t excess = 0;
        bool allocate_excess = true;

        // If the file is large enough then allocate more space to avoid
        // frequent calls to ftruncate(); these calls cause bad performance
        // spikes.
        //
        // Disabled on win32 because truncating a mapped file is not allowed!
#ifdef WIN32
        if (m_state.mapped_size != 0)
            allocate_excess = false;
#endif

        if (allocate_excess) {
            if (file_size() < requested_length * 100)
                excess = 0;
            else if (file_size() < requested_length * 250)
                excess = requested_length * 100;
            else if (file_size() < requested_length * 1000)
                excess = requested_length * 250;
            else
                excess = requested_length * 1000;
        }

        address = file_size();
        truncate_nolock(address + requested_length + excess);
        excess_at_end = excess;
    }
    return address;
}

// reads a page from the device; this function CAN return a
// pointer to mmapped memory
void DiskDevice::read_page(Page *page, uint64_t address)
{
    // this page is not in the mapped area; allocate a buffer
    if (page->data() == nullptr) {
        // note that |p| will not leak if file.pread() throws; |p| is stored
        // in the |page| object and will be cleaned up by the caller in
        // case of an exception.
        uint8_t *p = Memory::allocate<uint8_t>(Page::page_size_bytes);
        page->assign_allocated_buffer(p, address);
    }

    file.pread(page->data(), Page::page_size_bytes, address);
#ifdef UPS_ENABLE_ENCRYPTION
    if (config.is_encryption_enabled) {
        AesCipher aes(config.encryption_key, page->address());
        aes.decrypt((uint8_t *)page->data(), (uint8_t *)page->data(),
                    config.page_size_bytes);
    }
#endif
}

// Allocates storage for a page from this device; this function
// will *NOT* return mmapped memory
void DiskDevice::alloc_page(Page *page)
{
    uint64_t address = alloc(Page::page_size_bytes);
    page->set_address(address);

    // allocate a memory buffer
    uint8_t *p = Memory::allocate<uint8_t>(Page::page_size_bytes);
    page->assign_allocated_buffer(p, address);
}

// Frees a page on the device; plays counterpoint to |alloc_page|
void DiskDevice::free_page(Page *page)
{
    assert(page->data() != nullptr);
    page->free_buffer();
}

// Removes unused space at the end of the file
void DiskDevice::reclaim_space()
{
    if (excess_at_end > 0) {
        truncate_nolock(file_size() - excess_at_end);
        excess_at_end = 0;
    }
}

// truncate/resize the device, sans locking
void DiskDevice::truncate_nolock(uint64_t new_file_size)
{
    if (new_file_size > file_size_limit_bytes)
        throw std::runtime_error( "UPS_LIMITS_REACHED" );
    file.truncate(new_file_size);
}



}
