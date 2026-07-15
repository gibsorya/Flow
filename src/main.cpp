#include <iostream>
#include "platform/window.h"
#include "renderer/renderer.h"

int main()
{
    WindowAPI window = createGLFWWindowAPI();

    WindowDesc window_desc;
    window_desc.title = "Flow Engine";
    window.create(&window_desc);

    const float vertices[] = {
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };
    MeshDesc mesh_desc;
    mesh_desc.positions = vertices;
    mesh_desc.vertexCount = sizeof(vertices);

    const NativeWindowInfo native = window.getNativeInfo();
    
    RendererAPI renderer = createGLRenderer();
    renderer.init(&native);
    MeshHandle mesh = renderer.createMesh(&mesh_desc);
    DrawCommand draws;
    draws.mesh = mesh;

    FramePacket packet;
    packet.drawCount = 1;
    packet.draws = &draws;

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

        // MeshHandle
        

        renderer.renderFrame(&packet);

        window.present();
    }
}