#include <romz/device/Device.h>

namespace romz {

Device::Device(const EnvConfig &config_)
    : config(config_) {
}

// virtual destructor
Device::~Device() = default;


std::size_t Device::page_size() const {
    return config.page_size_bytes;
}

}
