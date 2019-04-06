#include <gtest/gtest.h>
#include <romz/os/File.h>

namespace romz {
namespace test {


///
///
///
TEST( File, constructor )
{
    EXPECT_NO_FATAL_FAILURE( File{} );
}

///
///
///
TEST( File, create )
{
    File f;
    f.create( "a.dat", 0 );
    std::vector< char > buffer( 100L );
    for( int i = 0; i < 100; i++ ) {
        f.write( buffer.data(), buffer.size() );
    }
}


///
///
///
TEST( File, mmap )
{
    File f;
    f.create( "a.dat", 0 );
    std::vector< std::uint8_t > buffer( 100L );
    std::uint8_t* p = buffer.data();
    f.mmap( 0, 10, true, &p );
}

}
}
