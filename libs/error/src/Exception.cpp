#include <romz/error/Exception.h>

namespace romz {

Exception::Exception( std::uint32_t st)
    : code(st)
{
}


}
