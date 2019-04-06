#pragma once

#include <cstdint>

namespace romz {

void os_read( int fd, std::uint8_t *buffer, const std::size_t len );

}
