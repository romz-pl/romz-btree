#include <gtest/gtest.h>
#include <romz/container/DynamicArray.h>

namespace romz {
namespace test {


///
///
///
TEST( DynamicArray, constructor )
{
    EXPECT_NO_THROW( DynamicArray< int >{} );
    EXPECT_NO_THROW( DynamicArray< int >{10} );
    EXPECT_NO_THROW( DynamicArray< int >(20, 1) );

}

}
}
