#pragma once

#include <romz/os/File.h>
#include <utility>

namespace romz {

struct DiskState
{
    DiskState() = default;
    DiskState(const DiskState&) = delete;
    DiskState& operator=(const DiskState&) = delete;

    DiskState(DiskState&&) = default;
    DiskState& operator=(DiskState&& ) = default;

    // the database file
    File file;

    // pointer to the the mmapped data
    std::uint8_t *mmapptr;

    // the size of mmapptr as used in mmap
    std::uint64_t mapped_size;

    // the (cached) size of the file
    std::uint64_t file_size;

    // excess storage at the end of the file
    std::uint64_t excess_at_end;

    // Allow state to be swapped
    friend void swap(DiskState& oldState, DiskState& newState)
    {
        std::swap(oldState, newState);
    }
};

}
