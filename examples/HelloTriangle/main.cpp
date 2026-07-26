#include <iostream>
#include "platform/window.h"
#include "renderer/renderer.h"
#include "core/math.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

int main()
{
    WindowAPI window = createGLFWWindowAPI();

    WindowDesc window_desc;
    window_desc.title = "Hello Triangle";
    window_desc.width = 1920;
    window_desc.height = 1080;
    window.create(&window_desc);

    const float vertices[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    const float colors[] = {
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f
    };

    MeshDesc mesh_desc;
    mesh_desc.positions = vertices;
    mesh_desc.vertexCount = sizeof(vertices) / sizeof(float);
    mesh_desc.colors = colors;
    
    const NativeWindowInfo native = window.getNativeInfo();

    RendererAPI renderer = createGLRenderer();
    renderer.init(&native);
    MeshHandle mesh = renderer.createMesh(&mesh_desc);

    MaterialDesc mat_desc;
    mat_desc.vertexShaderPath = "triangle.vert";
    mat_desc.fragmentShaderPath = "triangle.frag";
    

    MaterialHandle material = renderer.createMaterial(&mat_desc);

    FramePacket packet;
    DrawCommand draws;

    Event events[256];
    bool keyDown[(int)Key::Count] = {};

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

        uint32_t w, h;
        window.getFramebufferSize(&w, &h);

        draws.mesh = mesh;
        draws.material = material;
        packet.viewportHeight = h;
        packet.viewportWidth = w;
        packet.drawCount = 1;
        packet.draws = &draws;

        renderer.renderFrame(&packet);

        window.present();
    }

    renderer.destroyMaterial(material);
    renderer.destroyMesh(mesh);
}