#include <romz/memory/Memory.h>

namespace romz {

std::uint64_t Memory::ms_peak_memory = 0;
std::uint64_t Memory::ms_total_allocations = 0;
std::uint64_t Memory::ms_current_allocations = 0;

}
