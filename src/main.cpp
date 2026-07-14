#include <iostream>
#include "platform/window.h"

int main()
{
    WindowAPI window = createGLFWWindowAPI();

    WindowDesc window_desc;
    window_desc.title = "Flow Engine";
    window.create(&window_desc);

    NativeWindowInfo native = window.getNativeInfo();

    Event events[256];
    bool running = true;
    while (running)
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
                if (events[i].key == Key::Escape)
                    running = false;
                break;
            case EventType::WindowResized:
                break;
            default:
                break;
            }
        }

        window.present();
    }
}