#pragma once

#include <romz/device/Device.h>

namespace romz {

struct DeviceFactory
{
    // creates a new Device instance depending on the flags
    static Device *create(const EnvConfig &config);
};

}

