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
    std::vector< char > buffer( 100000000L );
    for( int i = 0; i < 100; i++ ) {
        f.write( buffer.data(), buffer.size() );
    }
}


}
}
