#include <romz/device/DeviceFactory.h>
#include <romz/device/InMemoryDevice.h>
#include <romz/device/DiskDevice.h>

namespace romz {

// creates a new Device instance depending on the flags
Device *DeviceFactory::create(const EnvConfig &config)
{
  if (config.in_memory)
    return new InMemoryDevice(config);
  else
    return new DiskDevice(config);
}
}
