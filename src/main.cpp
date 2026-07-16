#include <iostream>
#include "platform/window.h"
#include "renderer/renderer.h"
#include "core/math.h"

int main()
{
    WindowAPI window = createGLFWWindowAPI();

    WindowDesc window_desc;
    window_desc.title = "Flow Engine";
    window.create(&window_desc);

    const float vertices[] = {
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };

    const uint32_t indices[] = {
        0, 1, 3,
        3, 1, 2
    };

    MeshDesc mesh_desc;
    mesh_desc.positions = vertices;
    mesh_desc.vertexCount = sizeof(vertices);
    mesh_desc.indices = indices;
    mesh_desc.indexCount = sizeof(indices);

    const NativeWindowInfo native = window.getNativeInfo();
    
    RendererAPI renderer = createGLRenderer();
    renderer.init(&native);
    MeshHandle mesh = renderer.createMesh(&mesh_desc);
    DrawCommand draws;
    draws.mesh = mesh;

    FramePacket packet;

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

        uint32_t w, h;
        window.getFramebufferSize(&w, &h);
        float aspect = (float)w/(float)h;
        makeOrtho(packet.projMatrix, aspect);
        packet.viewportHeight = h;
        packet.viewportWidth = w;
        packet.drawCount = 1;
        packet.draws = &draws;

        renderer.renderFrame(&packet);

        window.present();
    }
}