#include "renderer/renderer.h"
#include "platform/window.h"

#include <cstdio>
#include <iostream>

int main()
{
    WindowAPI window = createGLFWWindowAPI();

    WindowDesc window_desc;
    window_desc.title = "Hello VK Triangle";
    window_desc.width = 1920;
    window_desc.height = 1080;
    window_desc.resizable = false;
    window_desc.requirements.needsGraphicsContext = false;

    if(!window.create(&window_desc))
    {
        fprintf(stderr, "GLFW window failed to be created");
        return -1;
    }

    const NativeWindowInfo native = window.getNativeInfo();

    RendererAPI renderer = createVKRenderer();
    
    if(!renderer.init(&native))
    {
        fprintf(stderr, "Vulkan failed to initialize.\n");
        return -1;
    }

    Event events[256];
    bool keyDown[(int)Key::Count] = {};
    bool running = true;

    while(running)
    {
        uint32_t n = window.pollEvents(events, 256);
        for (uint32_t i = 0; i < n; ++i)
        {
            switch (events[i].type)
            {
            case EventType::WindowClosed:
                running = false;
                break;
            case EventType::KeyDown:
                keyDown[(int)events[i].key] = true;
                if (events[i].key == Key::Escape)
                    running = false;
                break;
            case EventType::KeyUp:
                keyDown[(int)events[i].key] = false;
                break;
            case EventType::WindowResized:
                // window.getFramebufferSize
                break;
            default:
                break;
            }
        }
    }

    renderer.shutdown();
    window.destroy();

    return 0;
}