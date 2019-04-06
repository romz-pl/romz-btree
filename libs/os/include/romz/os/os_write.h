#pragma once

#include <cstdint>

namespace romz {

void os_write( int fd, const void *buffer, const std::size_t len );

}

