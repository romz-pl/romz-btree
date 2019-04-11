#include <gtest/gtest.h>
#include <romz/os/File.h>

namespace romz {
namespace test {

///
///
///
TEST( File, page_size )
{
    ASSERT_TRUE( File::page_size() > 0 );
}

///
///
///
TEST( File, create_open )
{
    const std::string path( "a.dat" );
    EXPECT_NO_THROW( File{ path } );
    EXPECT_NO_THROW( File( path, true ) );
}

///
///
///
TEST( File, pwrite_pread )
{
    using T = std::uint16_t;

    const uint64_t offset = 0;
    const std::vector< T > bufa = { 1u, 2u, 3u, 4u };
    const uint64_t count = bufa.size() * sizeof( T );
    File f( "a.dat" );

    ASSERT_NO_THROW( f.pwrite( bufa.data(), count, offset ) );

    std::vector< T > bufb( bufa.size(), 0 );
    ASSERT_EQ( bufa.size(), bufb.size() );

    ASSERT_NO_THROW( f.pread( bufb.data(), count, offset ) );

    ASSERT_EQ( bufa, bufb );
}


///
///
///
TEST( File, write_read )
{
    using T = std::uint16_t;

    File f( "a.dat" );

    const std::vector< T > inpa = { 1u, 2u, 3u, 4u };
    const uint64_t counta = inpa.size() * sizeof( T );

    const std::vector< T > inpb = { 11u, 22u, 33u, 44u, 55u, 66u };
    const uint64_t countb = inpb.size() * sizeof( T );

    ASSERT_NO_THROW( f.write( inpa.data(), counta ) );
    ASSERT_NO_THROW( f.write( inpb.data(), countb ) );

    std::vector< T > outa( inpa.size(), 0 );
    ASSERT_EQ( outa.size(), inpa.size() );

    std::vector< T > outb( inpb.size(), 0 );
    ASSERT_EQ( outb.size(), inpb.size() );

    ASSERT_NO_THROW( f.lseek( 0 ) );

    ASSERT_NO_THROW( f.read( outa.data(), counta ) );
    ASSERT_EQ( inpa, outa );

    ASSERT_NO_THROW( f.read( outb.data(), countb ) );
    ASSERT_EQ( inpb, outb );

    ASSERT_EQ( f.file_size(), counta + countb );
}


}
}
