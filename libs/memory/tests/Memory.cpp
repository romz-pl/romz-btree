#include <gtest/gtest.h>
#include <romz/memory/Memory.h>

namespace romz {
namespace test {


///
///
///
TEST( Memory, allocate )
{

    EXPECT_TRUE( Memory::ms_current_allocations == 0 );
    EXPECT_TRUE( Memory::ms_total_allocations == 0 );

    auto * p = Memory::allocate< int >( 10u );
    EXPECT_TRUE( p != nullptr );
    EXPECT_TRUE( Memory::ms_current_allocations == 1 );
    EXPECT_TRUE( Memory::ms_total_allocations == 1 );

    Memory::release( p );
    EXPECT_TRUE( Memory::ms_current_allocations == 0 );
    EXPECT_TRUE( Memory::ms_total_allocations == 1 );

}

}
}
