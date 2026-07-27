#pragma once

#include <cstdint>

struct WindowConfig {
    // Does the renderer want the window to create a graphics context?
    // GL: yes, and it needs version/profile. 
    // Vulkan: no (renderer makes its own surface).
    bool needsGraphicsContext = true;

    // Defaults for when wantsGraphicsContext = true
    uint32_t contextMajor = 4;
    uint32_t contextMinor = 1;
    bool contextCoreProfile = true;
};
