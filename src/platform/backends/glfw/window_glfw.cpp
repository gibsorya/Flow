#include <iostream>

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
        switch (glfwKey)
        {
        case GLFW_KEY_ESCAPE:
            return Key::Escape;
        case GLFW_KEY_SPACE:
            return Key::Space;
        case GLFW_KEY_A:
            return Key::A;
        // ... table-driven in real code
        default:
            return Key::Unknown;
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

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    window = glfwCreateWindow(desc->width, desc->height, desc->title, nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        return false;
    }

    glfwSetWindowCloseCallback(window, onClose);
    glfwSetKeyCallback(window, onKey);
    glfwMakeContextCurrent(window);

    return window != nullptr;
}

static uint32_t glfw_pollEvents(Event* out, uint32_t cap) {
    g_queueCount = 0;
    glfwPollEvents();                 // fires callbacks -> queue
    uint32_t n = (g_queueCount < cap) ? g_queueCount : cap;
    memcpy(out, g_queue, n * sizeof(Event));
    return n;
}

static NativeWindowInfo glfw_nativeInfo() {
    return { NativeWindowKind::GLFW, window, 0, reinterpret_cast<void*(*)(const char*)>(glfwGetProcAddress) };
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