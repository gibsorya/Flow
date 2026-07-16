#include <iostream>
#include "platform/window.h"
#include "renderer/renderer.h"
#include "core/math.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    mesh_desc.vertexCount = sizeof(vertices) / sizeof(float);
    mesh_desc.indices = indices;
    mesh_desc.indexCount = sizeof(indices) / sizeof(uint32_t);

    const NativeWindowInfo native = window.getNativeInfo();
    
    RendererAPI renderer = createGLRenderer();
    renderer.init(&native);
    MeshHandle mesh = renderer.createMesh(&mesh_desc);
    DrawCommand draws;
    

    FramePacket packet;

    Event events[256];
    bool running = true;

    /** MAIN LOOP */
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

        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        draws.mesh = mesh;
        draws.model = *reinterpret_cast<const Mat4*>(glm::value_ptr(model));
        packet.viewProj = *reinterpret_cast<const Mat4*>(glm::value_ptr(projection * view));
        packet.viewportHeight = h;
        packet.viewportWidth = w;
        packet.drawCount = 1;
        packet.draws = &draws;

        renderer.renderFrame(&packet);

        window.present();
    }
}