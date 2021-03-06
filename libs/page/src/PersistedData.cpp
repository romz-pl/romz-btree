#include <romz/page/PersistedData.h>
#include <romz/memory/Memory.h>

namespace romz {

PersistedData::PersistedData()
    : address(0)
    , size(0)
    , is_dirty(false)
    , is_allocated(false)
    , is_without_header(false)
    , raw_data(nullptr)
{
}


PersistedData::PersistedData(const PersistedData &other)
    : address(other.address)
    , size(other.size)
    , is_dirty(other.is_dirty)
    , is_allocated(other.is_allocated)
    , is_without_header(other.is_without_header)
    , raw_data(other.raw_data)
{
}


PersistedData::~PersistedData() {
#ifdef NDEBUG
    mutex.safe_unlock();
#endif
    if (is_allocated)
        Memory::release(raw_data);
    raw_data = nullptr;
}
}
