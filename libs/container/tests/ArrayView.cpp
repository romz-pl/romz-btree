#include <gtest/gtest.h>
#include <romz/container/ArrayView.h>

namespace romz {
namespace test {


///
///
///
TEST( ArrayView, constructor )
{
    EXPECT_NO_THROW( ArrayView< int >{} );
    EXPECT_NO_THROW( ArrayView< int >(nullptr, 0) );

}

}
}
