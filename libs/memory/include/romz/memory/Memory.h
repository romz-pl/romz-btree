#pragma once

#include <stdexcept>

///
///
/// Memory handling
///
///
/// The static Memory class provides memory management functions in a common
/// c++ namespace. The functions can allocate, reallocate and free memory
/// while tracking usage statistics.
///
/// If tcmalloc is used then additional metrics will be available.
///
/// This class only has static members and methods. It does not have a
/// constructor.
///
namespace romz {

struct Memory
{
    // allocates |size| bytes, casted into type |T *|;
    // returns null if out of memory.
    // usage:
    //
    //     char *p = Memory::allocate<char>(1024);
    //
    template<typename T>
    static T *allocate(size_t size) {
        ms_total_allocations++;
        ms_current_allocations++;
#ifdef UPS_USE_TCMALLOC
        T *t = static_cast< T* >( ::tc_malloc(size) );
#else
        T *t = static_cast< T* >( ::malloc(size) );
#endif
        if (!t)
            throw std::runtime_error( "UPS_OUT_OF_MEMORY" );
        return t;
    }

    // allocates |size| bytes; returns null if out of memory. initializes
    // the allocated memory with zeroes.
    // usage:
    //
    //     const char *p = Memory::callocate<const char>(50);
    //
    template<typename T>
    static T *callocate(size_t size) {
        ms_total_allocations++;
        ms_current_allocations++;

#ifdef UPS_USE_TCMALLOC
        T *t = static_cast< T* >( ::tc_calloc(1, size) );
#else
        T *t = static_cast< T* >( ::calloc(1, size) );
#endif
        if (!t)
            throw std::runtime_error( "UPS_OUT_OF_MEMORY" );
        return t;
    }

    // re-allocates |ptr| for |size| bytes; returns null if out of memory.
    // |ptr| can be null on first use.
    // usage:
    //
    //     p = Memory::reallocate<char>(p, 100);
    //
    template<typename T>
    static T *reallocate(T *ptr, size_t size) {
        if (ptr == nullptr) {
            ms_total_allocations++;
            ms_current_allocations++;
        }
#ifdef UPS_USE_TCMALLOC
        T *t = static_cast< T* >( ::tc_realloc(ptr, size) );
#else
        T *t = static_cast< T* >( ::realloc(ptr, size) );
#endif
        if (!t)
            throw std::runtime_error( "UPS_OUT_OF_MEMORY" );
        return t;
    }

    // releases a memory block; can deal with NULL pointers.
    static void release(void *ptr) {
        if (ptr != nullptr) {
            ms_current_allocations--;
#ifdef UPS_USE_TCMALLOC
            ::tc_free(ptr);
#else
            ::free(ptr);
#endif
        }
    }

    // updates and returns the collected metrics
    //static void get_global_metrics(ups_env_metrics_t *metrics);

    // peak memory usage
    static uint64_t ms_peak_memory;

    // total memory allocations
    static uint64_t ms_total_allocations;

    // currently active allocations
    static uint64_t ms_current_allocations;
};

}
