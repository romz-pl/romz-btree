#include <romz/os/lock_exclusive.h>
#include <romz/os/exception_with_errno.h>
#include <sys/file.h>

namespace romz {

void lock_exclusive(int fd, bool lock)
{
#ifdef UPS_SOLARIS
    // SunOS 5.9 doesn't have LOCK_* unless i include /usr/ucbinclude; but then,
    // mmap behaves strangely (the first write-access to the mmapped buffer
    // leads to a segmentation fault).
    //
    // Tell me if this troubles you/if you have suggestions for fixes.
#else
    int flags;

    if( lock ) {
        flags = LOCK_EX | LOCK_NB; // ?????
        // flags = LOCK_EX; // to jest bezpieczniejsze?
    }
    else {
        flags = LOCK_UN;
    }

    if( ::flock( fd, flags )  == -1 ) {
        exception_with_errno( "flock failed." );

        // it seems that linux does not only return EWOULDBLOCK, as stated
        // in the documentation (flock(2)), but also other errors...
        //    if (errno && lock)
        //      throw Exception(UPS_WOULD_BLOCK);
        //    throw Exception(UPS_IO_ERROR);
    }
#endif
}

}
