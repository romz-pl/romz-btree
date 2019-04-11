#include <romz/os/File.h>
#include <cassert>
#include <errno.h>
#include <fcntl.h>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


static_assert( sizeof( off_t ) == sizeof( std::int64_t ), " The type off_t must be 64 bits long" );


namespace romz {

///
/// Creates a new file
///
File::File( const std::string& path )
    : m_fd( -1 )
    , m_posix_advice( 0 )
{
    const int flags = O_CREAT | O_RDWR | O_TRUNC | O_NOATIME;
    const mode_t mode = 0644;
    m_fd = ::open( path.c_str(), flags, mode );
    if( m_fd < 0 ) {
        exception_with_errno( "open failed." );
    }

    lock_exclusive();
    set_posix_advice();
}

///
/// Opens an existing file
///
File::File( const std::string& path, bool read_only )
    : m_fd( -1 )
    , m_posix_advice( 0 )
{

    int flags = O_NOATIME;

    if( read_only ) {
        flags |= O_RDONLY;
    }
    else {
        flags |= O_RDWR;
    }


    m_fd = ::open( path.c_str(), flags );
    if( m_fd < 0 ) {
        exception_with_errno( "open failed." );
    }

    lock_exclusive();
    set_posix_advice();

}

void File::lock_exclusive() const
{
    assert( m_fd >= 0 );

    const int operation = LOCK_EX | LOCK_NB; // ?????
    // const int operation = LOCK_EX; // safer?

    if( ::flock( m_fd, operation )  == -1 ) {
        exception_with_errno( "flock failed." );
    }
}

///
/// Closes the file
///
File::~File()
{
    if( m_fd < 0 ) {
        return;
    }

    // on posix, we most likely don't want to close descriptors 0 and 1
    assert(m_fd != 0 && m_fd != 1);

    const int operation = LOCK_UN;

    if( ::flock( m_fd, operation )  == -1 ) {
        exception_with_errno( "flock failed." );
    }

    // now close the descriptor
    if( ::close( m_fd ) == -1 ) {
        exception_with_errno( "close failed." );
    }
}


///
/// Get the page allocation granularity of the operating system
///
std::size_t File::page_size()
{
    return static_cast< size_t >( ::sysconf(_SC_PAGE_SIZE) );
}

///
/// Sets the parameter for posix_fadvise()
///
void File::set_posix_advice() const
{
    assert( m_fd >= 0 );

    const int r = ::posix_fadvise( m_fd, 0, 0, POSIX_FADV_RANDOM) ;
    if( r != 0 ) {
        exception_with_errno( "File::set_posix_advice failed." );
    }
}

///
/// Positional read from a file
///
void File::pread( void *buf, uint64_t count, uint64_t offset )
{
    // Check if casting is safe
    assert( count < std::numeric_limits< size_t >::max() );
    const size_t count_tmp = static_cast< size_t >( count );

    // Check if casting is safe
    assert( offset < std::numeric_limits< off_t >::max() );
    const off_t offset_tmp = static_cast< off_t >( offset );


    const ssize_t r = ::pread( m_fd, buf, count_tmp, offset_tmp );
    if( r < 0 ) {
        exception_with_errno( "File::pread failed." );
    }
}


///
/// Positional write to a file
///
void File::pwrite( const void *buf, uint64_t count, uint64_t offset )
{

    // Check if casting is safe
    assert( count < std::numeric_limits< size_t >::max() );
    const size_t count_tmp = static_cast< size_t >( count );

    // Check if casting is safe
    assert( offset < std::numeric_limits< off_t >::max() );
    const off_t offset_tmp = static_cast< off_t >( offset );

    const ssize_t r = ::pwrite( m_fd, buf, count_tmp, offset_tmp );
    if( r < 0 ) {
        exception_with_errno( "File::pwrite failed." );
    }
}

///
/// Reads data froam file.
/// The current file position is used.
///
void File::read( void *buf, uint64_t count )
{
    // Check if casting is safe
    assert( count < std::numeric_limits< size_t >::max() );
    const size_t count_tmp = static_cast< size_t >( count );

    const ssize_t r = ::read( m_fd, buf, count_tmp );
    if( r < 0 ) {
        exception_with_errno( "File::read failed." );
    }
}

///
/// Writes data to a file;
/// The current file position is used.
///
void File::write( const void *buf, uint64_t count )
{
    // Check if casting is safe
    assert( count < std::numeric_limits< size_t >::max() );
    const size_t count_tmp = static_cast< size_t >( count );

    const ssize_t r = ::write( m_fd, buf, count_tmp );
    if( r < 0 ) {
        exception_with_errno( "File::read failed." );
    }
}

///
/// Seek position in a file
///
void File::lseek( uint64_t offset ) const
{
    assert( offset < std::numeric_limits< off_t >::max() );
    off_t offset_tmp = static_cast< off_t >( offset );

    const int whence = SEEK_SET;
    if( ::lseek( m_fd, offset_tmp, whence ) < 0 ) {
        exception_with_errno( "lseek failed." );
    }
}

///
/// Tell the position in a file
///
std::uint64_t File::tell() const
{
    const off_t offset = ::lseek( m_fd, 0, SEEK_CUR );
    if( offset == static_cast< off_t >( -1 ) ) {
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
    const off_t size = ::lseek( m_fd, 0, SEEK_END );
    if( size == static_cast< off_t >( -1 ) ) {
        exception_with_errno( "lseek failed." );
    }
    assert( size >= 0 );
    return static_cast< std::uint64_t >( size );
}

///
/// Truncate/resize the file
///
void File::truncate( std::uint64_t length ) const
{
    assert( length < std::numeric_limits< off_t >::max() );
    const off_t length_tmp = static_cast< off_t >( length );

    if( ::ftruncate( m_fd, length_tmp ) ) {
        exception_with_errno( "ftruncate failed." );
    }
}

///
/// fsync a file
///
void File::fsync() const
{
    assert( m_fd >= 0 );

    if( ::fsync( m_fd ) == -1 ) {
        exception_with_errno( "fdatasync failed." );
    }
}

///
/// fdatasync a file
///
/// unlike fsync(), fdatasync() does not flush the metadata unless
/// it's really required. it's therefore a lot faster.
///
void File::fdatasync() const
{
    assert( m_fd >= 0 );

    if( ::fdatasync( m_fd ) == -1 ) {
        exception_with_errno( "fdatasync failed." );
    }
}

///
///
///
void File::exception_with_errno( const std::string& txt )
{
    std::stringstream ss;
    ss << txt
       << " errno = " << std::to_string(errno)
       << " strerror = " << strerror(errno);
    throw std::runtime_error( ss.str() );
}


}
