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
    window_desc.title = "Flow Engine";
    window.create(&window_desc);

    const float vertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
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

    glm::vec3 camPos  = glm::vec3(128.0f, 153.0f, -38.0f);  // grid space: 0..64
    float     yaw     = 90.0f;    // degrees, facing +Z into the grid
    float     pitch   = 0.0f;

    Event events[256];
    bool keyDown[(int)Key::Count] = {};
    auto lastTime = std::chrono::steady_clock::now();
    bool running = true;

    /** MAIN LOOP */
    while (running)
    {
        auto now = std::chrono::steady_clock::now();
        float dt = std::chrono::duration<float>(now - lastTime).count();
        lastTime = now;
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

        const float turnSpeed = 90.0f; // deg/sec
        if (keyDown[(int)Key::Left])
            yaw -= turnSpeed * dt;
        if (keyDown[(int)Key::Right])
            yaw += turnSpeed * dt;
        if (keyDown[(int)Key::Up])
            pitch += turnSpeed * dt;
        if (keyDown[(int)Key::Down])
            pitch -= turnSpeed * dt;
        pitch = glm::clamp(pitch, -89.0f, 89.0f);

        glm::vec3 dir;
        dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        dir.y = sin(glm::radians(pitch));
        dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        dir = glm::normalize(dir);

        const glm::vec3 worldUp(0, 1, 0);
        glm::vec3 right = glm::normalize(glm::cross(dir, worldUp));
        glm::vec3 up    = glm::normalize(glm::cross(right, dir));

        const float moveSpeed = 20.0f;  // voxels/sec
        glm::vec3 vel(0.0f);
        if (keyDown[(int)Key::W]) vel += dir;
        if (keyDown[(int)Key::S])  {
            std::cout << "dir: " << dir.x << ", " << dir.y << ", " << dir.z << std::endl;
            vel -= dir;
        }
        if (keyDown[(int)Key::D]) vel += right;
        if (keyDown[(int)Key::A]) vel -= right;
        if (keyDown[(int)Key::Space])  vel += worldUp;
        if (keyDown[(int)Key::LShift]) vel -= worldUp;
        if (glm::length(vel) > 0.0f)
            camPos += glm::normalize(vel) * moveSpeed * dt; 

        uint32_t w, h;
        window.getFramebufferSize(&w, &h);
        float aspect = (float)w/(float)h;

        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);

        projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        view = glm::translate(view, glm::vec3(-32.0f, -32.0f, -40.0f));

        // glm::vec3 cameraPos = glm::vec3(32.0f, 32.0f, -10.0f); // Positioned slightly outside the 64³ grid
        // float yaw   = -90.0f; // Facing forward along the -Z axis
        // float pitch =   0.0f;

        // glm::vec3 front;
        // front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)); front.y = sin(glm::radians(pitch)); front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        // glm::vec3 cameraDir = glm::normalize(front);

        // glm::vec3 worldUp   = glm::vec3(0.0f, 1.0f, 0.0f); 
        // glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDir, worldUp));
        // glm::vec3 cameraUp    = glm::normalize(glm::cross(cameraRight, cameraDir));

        // packet.camPos = *reinterpret_cast<const Vec3*>(glm::value_ptr(cameraPos));
        // packet.camDir = *reinterpret_cast<const Vec3*>(glm::value_ptr(cameraDir));
        // packet.camUp = *reinterpret_cast<const Vec3*>(glm::value_ptr(cameraUp));


        // glm::mat4 invView = glm::inverse(view);

        // // 2. Extract the true, calculated world-space vectors from the matrix columns
        // glm::vec3 trueCamPos   = glm::vec3(invView[3]);        // Column 4 is Position
        // glm::vec3 trueCamDir   = -glm::normalize(glm::vec3(invView[2])); // Column 3 is Forward (Negated in OpenGL)
        // glm::vec3 trueCamUp    = glm::normalize(glm::vec3(invView[1])); 

        packet.camPos = *reinterpret_cast<const Vec3*>(glm::value_ptr(camPos));
        packet.camDir = *reinterpret_cast<const Vec3*>(glm::value_ptr(dir));
        packet.camUp  = *reinterpret_cast<const Vec3*>(glm::value_ptr(up));

        // model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.3f, 0.54f));
        draws.mesh = mesh;
        // draws.model = *reinterpret_cast<const Mat4*>(glm::value_ptr(model));
        // packet.viewProj = *reinterpret_cast<const Mat4*>(glm::value_ptr(projection * view));
        packet.viewportHeight = h;
        packet.viewportWidth = w;
        packet.drawCount = 1;
        packet.draws = &draws;

        renderer.renderFrame(&packet);

        window.present();
    }
}