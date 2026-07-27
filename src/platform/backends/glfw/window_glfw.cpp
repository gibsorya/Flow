#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "platform/window.h"

namespace
{
    GLFWwindow *window = nullptr;

    Event g_queue[1024];
    uint32_t g_queueCount = 0;

    void push(const Event &e)
    {
        if (g_queueCount < 1024)
            g_queue[g_queueCount++] = e;
    }

    Key translateKey(int glfwKey)
    {
        if (glfwKey >= GLFW_KEY_A  && glfwKey <= GLFW_KEY_Z)
            return Key((int)Key::A    + (glfwKey - GLFW_KEY_A));
        if (glfwKey >= GLFW_KEY_0  && glfwKey <= GLFW_KEY_9)
            return Key((int)Key::Num0 + (glfwKey - GLFW_KEY_0));
        if (glfwKey >= GLFW_KEY_F1 && glfwKey <= GLFW_KEY_F12)
            return Key((int)Key::F1   + (glfwKey - GLFW_KEY_F1));

        switch (glfwKey) {
            case GLFW_KEY_ESCAPE:        return Key::Escape;
            case GLFW_KEY_SPACE:         return Key::Space;
            case GLFW_KEY_ENTER:         return Key::Enter;
            case GLFW_KEY_TAB:           return Key::Tab;
            case GLFW_KEY_BACKSPACE:     return Key::Backspace;
            case GLFW_KEY_DELETE:        return Key::Delete;
            case GLFW_KEY_LEFT:          return Key::Left;
            case GLFW_KEY_RIGHT:         return Key::Right;
            case GLFW_KEY_UP:            return Key::Up;
            case GLFW_KEY_DOWN:          return Key::Down;
            case GLFW_KEY_LEFT_SHIFT:    return Key::LShift;
            case GLFW_KEY_RIGHT_SHIFT:   return Key::RShift;
            case GLFW_KEY_LEFT_CONTROL:  return Key::LCtrl;
            case GLFW_KEY_RIGHT_CONTROL: return Key::RCtrl;
            case GLFW_KEY_LEFT_ALT:      return Key::LAlt;
            case GLFW_KEY_RIGHT_ALT:     return Key::RAlt;
            default:                     return Key::Unknown;
        }
    }

    void onKey(GLFWwindow *, int key, int, int action, int)
    {
        Event e;
        e.type = (action == GLFW_RELEASE) ? EventType::KeyUp
                                          : EventType::KeyDown;
        e.key = translateKey(key);
        e.flag = (action == GLFW_REPEAT) ? 1 : 0;
        push(e);
    }

    void onClose(GLFWwindow *)
    {
        push({EventType::WindowClosed});
    }
}

static void onGlfwError(int code, const char* desc) {
    fprintf(stderr, "[GLFW %d] %s\n", code, desc);
}

static bool glfw_create(const WindowDesc *desc)
{
    glfwSetErrorCallback(onGlfwError); 

    if(!glfwInit()) {
        std::cerr << "Error creating GLFW window!" << std::endl;
        return false;
    }

    if(desc->requirements.needsGraphicsContext) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, desc->requirements.contextMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, desc->requirements.contextMinor);

        if(desc->requirements.contextCoreProfile) {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        }
    } else {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    window = glfwCreateWindow(desc->width, desc->height, desc->title, nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        return false;
    }

    glfwSetWindowCloseCallback(window, onClose);
    glfwSetKeyCallback(window, onKey);

    if(desc->requirements.needsGraphicsContext) glfwMakeContextCurrent(window);

    return window != nullptr;
}

static uint32_t glfw_pollEvents(Event* out, uint32_t cap) {
    g_queueCount = 0;
    glfwPollEvents();                 // fires callbacks -> queue
    uint32_t n = (g_queueCount < cap) ? g_queueCount : cap;
    memcpy(out, g_queue, n * sizeof(Event));
    return n;
}

static int32_t glfw_createVulkanSurface(uint64_t instance, uint64_t* outSurface) {
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkResult r = glfwCreateWindowSurface((VkInstance)instance, window, nullptr, &surface);
    *outSurface = (uint64_t)surface;
    return (int32_t)r; 
}

static NativeWindowInfo glfw_nativeInfo() {
    return { 
        NativeWindowKind::GLFW,
        window,
        0,
        reinterpret_cast<void*(*)(const char*)>(glfwGetProcAddress),
        reinterpret_cast<int32_t(*)(uint64_t,uint64_t*)>(glfw_createVulkanSurface),
        reinterpret_cast<const char** (*)(uint32_t*)>(glfwGetRequiredInstanceExtensions)
    };
}

static void glfw_present() {
    glfwSwapBuffers(window);
}
static void glfw_destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

static void glfw_fbSize(uint32_t* w, uint32_t* h) {
    int iw, ih; glfwGetFramebufferSize(window, &iw, &ih);
    *w = iw; *h = ih;
}

WindowAPI createGLFWWindowAPI() {
    return { 1, "GLFW",
            glfw_create, glfw_destroy, glfw_pollEvents,
            glfw_fbSize, glfw_nativeInfo, glfw_present };
}