#include <romz/os/os_read.h>
#include <romz/os/exception_with_errno.h>
#include <unistd.h>
#include <cassert>

namespace romz {

void os_read(int fd, std::uint8_t *buffer, const std::size_t len)
{
    std::size_t total = 0;

    while( total < len ) {
        const ssize_t r = ::read( fd, &buffer[total], len - total );
        if( r < 0 ) {
            exception_with_errno( "os_read failed." );
        }

        if( r == 0 ) {
            break;
        }

        assert( r > 0 );
        total += static_cast< std::size_t >( r );
    }

    if( total != len ) {
        exception_with_errno( "os_read() failed with short read." );
    }
}

}
