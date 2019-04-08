#include <romz/os/File.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cassert>
#include <romz/os/exception_with_errno.h>
#include <romz/os/lock_exclusive.h>
// #include <romz/os/enable_largefile.h>
#include <romz/os/os_read.h>
#include <romz/os/os_write.h>
#include <limits>


static_assert( sizeof( off_t ) == sizeof( std::int64_t ), " The type off_t must be 64 bits long" );


namespace romz {

///
/// Creates an empty File handle
///
File::File()
    : m_fd( UPS_INVALID_FD )
    , m_posix_advice( 0 )
{
}

///
/// Moves ownership of the file handle
///
File::File(File &&other)
    : m_fd( other.m_fd )
    , m_posix_advice( other.m_posix_advice )
{
    other.m_fd = UPS_INVALID_FD;
}

///
/// Closes the file
///
File::~File()
{
    close();
}

///
/// Moves ownership of the file handle
///
File& File::operator=(File &&other)
{
    m_fd = other.m_fd;
    other.m_fd = UPS_INVALID_FD;
    return *this;
}

///
/// Get the page allocation granularity of the operating system
///
std::size_t File::granularity()
{
    return static_cast< size_t >( ::sysconf(_SC_PAGE_SIZE) );
}

///
/// Sets the parameter for posix_fadvise()
///
void File::set_posix_advice(int advice)
{
    m_posix_advice = advice;
    assert(m_fd != UPS_INVALID_FD);

#if HAVE_POSIX_FADVISE
    if (m_posix_advice == UPS_POSIX_FADVICE_RANDOM) {
        int r = ::posix_fadvise(m_fd, 0, 0, POSIX_FADV_RANDOM);
        if (r != 0) {
            ups_log(("posix_fadvise failed with status %d (%s)",
                     errno, strerror(errno)));
            throw Exception(UPS_IO_ERROR);
        }
    }
#endif
}

///
/// Maps a file in memory
///
/// mmap is called with MAP_PRIVATE - the allocated buffer
/// is just a copy of the file; writing to the buffer will not alter
/// the file itself.
///
void File::mmap(std::uint64_t offset, std::size_t size, bool readonly, std::uint8_t **buffer)
{
    assert( size  > 0 );

    int prot = PROT_READ;
    if( !readonly )
        prot |= PROT_WRITE;

    const int flags = MAP_PRIVATE;
    assert( offset < std::numeric_limits< off_t >::max() );
    const off_t offset_tmp = static_cast< off_t >( offset );
    *buffer = static_cast<uint8_t *>( ::mmap( nullptr, size, prot, flags, m_fd, offset_tmp) );
    if( *buffer == reinterpret_cast< void* >( -1 ) ) {
        *buffer = nullptr;
        exception_with_errno( "mmap failed." );
    }

    const int r = ::madvise( *buffer, size, MADV_RANDOM );
    if( r != 0 ) {
        exception_with_errno( "madvise failed." );
    }
}

///
/// Unmaps a buffer
///
void File::munmap(void *buffer, size_t size)
{
    const int r = ::munmap( buffer, size );
    if( r == -1 ) {
        exception_with_errno( "munmap failed." );
    }

}

///
/// Positional read from a file
///
void File::pread(uint64_t addr, void *buffer, size_t len)
{
    std::size_t total = 0;

    while( total < len ) {
        assert( addr + total < std::numeric_limits< off_t >::max() );
        const off_t offset = static_cast< off_t >( addr + total );
        const ssize_t r = ::pread(m_fd, static_cast< std::uint8_t* >( buffer ) + total, len - total, offset);
        if( r < 0 ) {
            exception_with_errno( "pread failed." );
        }

        if( r == 0 ) {
            break;
        }

        assert( r > 0 );
        total += static_cast< std::size_t >( r );
    }

    if( total != len ) {
        exception_with_errno( "pread failed with short read." );
    }
}


///
/// Positional write to a file
///
void File::pwrite(uint64_t addr, const void *buffer, std::size_t len)
{
    std::size_t total = 0;

    while( total < len ) {
        assert( addr + total < std::numeric_limits< off_t >::max() );
        const off_t offset = static_cast< off_t >( addr + total );
        const ssize_t s = ::pwrite(m_fd, buffer, len, offset);
        if( s < 0 ) {
            exception_with_errno( "pwrite failed." );
        }

        if( s == 0 ) {
            break;
        }

        assert( s > 0 );
        total += static_cast< std::size_t >( s );
    }

    if( total != len ) {
        exception_with_errno( "pwrite failed with short read." );
    }
}

///
/// Write data to a file; uses the current file position
///
void File::write(const void *buffer, size_t len)
{
    os_write(m_fd, buffer, len);
}

///
/// Seek position in a file
///
void File::seek(uint64_t offset, int whence) const
{
    assert( offset < std::numeric_limits< off_t >::max() );
    off_t offset_tmp = static_cast< off_t >( offset );
    if( ::lseek(m_fd, offset_tmp, whence) < 0 ) {
        exception_with_errno( "lseek failed." );
    }
}

///
/// Tell the position in a file
///
std::uint64_t File::tell() const
{
    off_t offset = ::lseek( m_fd, 0, SEEK_CUR );
    if (offset == static_cast< off_t >( -1 ) ) {
        exception_with_errno( "lseek failed." );
    }
    assert( offset >= 0 );
    return static_cast< std::uint64_t >( offset );
}

///
/// Returns the size of the file
///
uint64_t File::file_size() const
{
    seek( 0, SEEK_END );
    uint64_t size = tell();
    return size;
}

///
/// Truncate/resize the file
///
void File::truncate(uint64_t newsize)
{
    assert( newsize < std::numeric_limits< off_t >::max() );
    const off_t length = static_cast< off_t >( newsize );
    if( ::ftruncate( m_fd, length ) ) {
        exception_with_errno( "ftruncate failed." );
    }
}

///
/// Creates a new file
///
void File::create(const char *filename, int mode)
{
    int osflags = O_CREAT | O_RDWR | O_TRUNC;
#if HAVE_O_NOATIME
    osflags |= O_NOATIME;
#endif

    int fd = ::open(filename, osflags, mode ? mode : 0644);
    if (fd < 0) {
        exception_with_errno( "open failed." );
    }

    /* lock the file - this is default behaviour since 1.1.0 */
    lock_exclusive( fd, true );

    /* enable O_LARGEFILE support */
    // enable_largefile( fd );

    m_fd = fd;
}

///
/// Flushes a file
///
void File::flush()
{
    /* unlike fsync(), fdatasync() does not flush the metadata unless
   * it's really required. it's therefore a lot faster. */
#if HAVE_FDATASYNC && !__APPLE__
    if (fdatasync(m_fd) == -1) {
#else
    if( ::fsync( m_fd ) == -1 ) {
#endif
        exception_with_errno( "fdatasync failed." );
    }
}

///
/// Opens an existing file
///
void File::open(const char *filename, bool read_only)
{
    int osflags = 0;

    if (read_only) {
        osflags |= O_RDONLY;
    }
    else {
        osflags |= O_RDWR;
    }

#if HAVE_O_NOATIME
    osflags |= O_NOATIME;
#endif

    const int fd = ::open( filename, osflags );
    if( fd < 0 ) {
        exception_with_errno( "open failed." );
    }

    /* lock the file - this is default behaviour since 1.1.0 */
    lock_exclusive( fd, true );

    /* enable O_LARGEFILE support */
    // enable_largefile(fd);

    m_fd = fd;
}

///
/// Returns true if the file is open
///
bool File::is_open() const
{
    return m_fd != UPS_INVALID_FD;
}

///
/// Closes the file descriptor
///
void File::close()
{
    if( m_fd == UPS_INVALID_FD ) {
        return;
    }

    // on posix, we most likely don't want to close descriptors 0 and 1
    assert(m_fd != 0 && m_fd != 1);

    // unlock the file - this is default behaviour since 1.1.0
    lock_exclusive( m_fd, false );

    // now close the descriptor
    if( ::close(m_fd) == -1 ) {
        exception_with_errno( "close failed." );
    }

    m_fd = UPS_INVALID_FD;
}

}
