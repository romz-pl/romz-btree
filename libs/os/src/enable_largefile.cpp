#include <romz/os/enable_largefile.h>
#include <unistd.h>
#include <fcntl.h>

namespace romz {

void enable_largefile(int fd)
{
    const int oflag = ::fcntl(fd, F_GETFL, 0);
    ::fcntl(fd, F_SETFL, oflag | O_LARGEFILE);
}

}
