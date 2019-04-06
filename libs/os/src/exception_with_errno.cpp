#include <romz/os/exception_with_errno.h>
#include <sstream>
#include <stdexcept>
#include <errno.h>
#include <string.h>

namespace romz {

void exception_with_errno( const std::string& txt )
{
    std::stringstream ss;
    ss << txt
       << " errno = " << std::to_string(errno)
       << " strerror = " << strerror(errno);
    throw std::runtime_error( ss.str() );
}

}
