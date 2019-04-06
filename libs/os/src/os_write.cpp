#include <romz/os/os_write.h>
#include <romz/os/exception_with_errno.h>
#include <unistd.h>
#include <cassert>

namespace romz {

void os_write( int fd, const void *buffer, const std::size_t len )
{
    std::size_t total = 0;
    const char *p = static_cast< const char* >( buffer );

    while( total < len ) {
        const ssize_t w = ::write(fd, p + total, len - total);
        if( w < 0 ) {
            exception_with_errno( "write failed." );
        }

        if( w == 0 ) {
            break;
        }

        assert( w > 0 );
        total += static_cast< std::size_t >( w );
    }

    if( total != len ) {
        exception_with_errno( "os_write() failed with short read." );
    }
}

}
