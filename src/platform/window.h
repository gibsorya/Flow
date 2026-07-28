#pragma once

#include <cstdint>
#include "platform/input_events.h"
#include "core/config.h"

enum class NativeWindowKind : uint32_t {
    Win32,      // handle1 = HWND,     handle2 = HINSTANCE
    X11,        // handle1 = Display*, handle2 = Window (as u64)
    Wayland,    // handle1 = wl_display*, handle2 = wl_surface*
    Cocoa,      // handle1 = NSWindow*
    GLFW,       // handle1 = GLFWwindow*  (lets GL backend call
                //   glfwMakeContextCurrent/SwapBuffers while GLFW
                //   is still the backend; a custom backend later
                //   reports its real kind instead)
};

struct NativeWindowInfo {
    NativeWindowKind kind;
    void*    handle1 = nullptr;
    uint64_t handle2 = 0;

    void* (*getProcAddress)(const char*);
    int32_t (*createVulkanSurface)(uint64_t instance, uint64_t* outSurface);
    const char** (*getRequiredInstanceExtensions)(uint32_t* count);
};

struct WindowDesc {
    const char* title  = "engine";
    uint32_t    width  = 1280;
    uint32_t    height = 720;
    bool        resizable = true;

    WindowConfig requirements;
};

struct WindowAPI {
    uint32_t apiVersion;                  // bump on struct changes
    const char* name;                     // "GLFW", "Win32", ...
 
    bool (*create)(const WindowDesc* desc);
    void (*destroy)(void);
 
    // THE input boundary: fills 'out' with up to 'capacity'
    // events that occurred since last call; returns count.
    // Engine calls this exactly once per frame and iterates
    // the array. No callbacks cross this line.
    uint32_t (*pollEvents)(Event* out, uint32_t capacity);
 
    void (*getFramebufferSize)(uint32_t* w, uint32_t* h);
    NativeWindowInfo (*getNativeInfo)(void);
 
    // Present hook. For GL this is SwapBuffers; for Vulkan the
    // renderer presents via the swapchain and this is a no-op.
    void (*present)(void);
};

// Each backend exposes exactly one factory:
WindowAPI createGLFWWindowAPI();
// WindowAPI createWin32WindowAPI();   // future
// WindowAPI createX11WindowAPI();     // future