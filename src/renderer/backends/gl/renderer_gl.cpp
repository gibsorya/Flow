#include <iostream>

#include "renderer/renderer.h"
#include "core/pool.h"
#include "platform/window.h"
#include "core/file_io.h"
#include "renderer/backends/gl/gl_mesh.h"
#include "renderer/backends/gl/gl_shader.h"

#include <glad/gl.h>

namespace
{
    struct GLState {
        GLuint   programID = 0;
        // grows: default framebuffer info, cached GL state, big buffers...
        GLuint uVP = 0;
        GLuint uModel = 0;
        GLuint textureID = 0;
        GLuint uCameraPos = 0;
        GLuint uCameraDir = 0;
        GLuint uCameraUp = 0;
        GLuint uResolution = 0;
    };
    GLState g_state;

    // static Pool<GLMesh, MeshTag> g_meshes;

    const int GRID_WIDTH = 64;
    const int GRID_HEIGHT = 64;
    const int GRID_DEPTH = 64;  
    GLuint create3DVoxelTexture()
    {
        std::vector<unsigned char> voxelData(GRID_WIDTH * GRID_HEIGHT * GRID_DEPTH * 4);

        for (int z = 0; z < GRID_DEPTH; ++z)
        {
            for (int y = 0; y < GRID_HEIGHT; ++y)
            {
                for (int x = 0; x < GRID_WIDTH; ++x)
                {
                    int index = (x + y * GRID_WIDTH + z * GRID_WIDTH * GRID_HEIGHT) * 4;

                    // Simple generation logic: Build a floor and a few random floating islands
                    if (y < 10 || (y < 30 && std::rand() % 100 > 98))
                    {
                        voxelData[index + 0] = static_cast<unsigned char>(x * 4); // R
                        voxelData[index + 1] = static_cast<unsigned char>(y * 4); // G
                        voxelData[index + 2] = static_cast<unsigned char>(z * 4); // B
                        voxelData[index + 3] = 255;                               // Alpha (Solid)
                    }
                    else
                    {
                        // Empty space
                        voxelData[index + 0] = 0;
                        voxelData[index + 1] = 0;
                        voxelData[index + 2] = 0;
                        voxelData[index + 3] = 0; // Alpha (Empty)
                    }
                }
            }
        }

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_3D, textureID);

        // Upload texture configuration parameters
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // Load data to GPU
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, GRID_WIDTH, GRID_HEIGHT, GRID_DEPTH, 0, GL_RGBA, GL_UNSIGNED_BYTE, voxelData.data());

        return textureID;
    }

    bool gl_init(const NativeWindowInfo *native)
    {
        if (!gladLoadGL((GLADloadfunc)native->getProcAddress))
        {
            std::cerr << "Error loading GL functions" << std::endl;
            return false;
        }

        glEnable(GL_DEPTH_TEST);

        printf("GL VERSION: %s\n", glGetString(GL_VERSION));
        printf("GL RENDERER: %s\n", glGetString(GL_RENDERER));

        const GLuint vertex = gl_loadShader("voxels.vert", GL_VERTEX_SHADER);
        const GLuint fragment = gl_loadShader("voxels.frag", GL_FRAGMENT_SHADER);

        if(!vertex || !fragment) return false;

        g_state.programID = gl_loadProgram(vertex, fragment);
        
        if(!g_state.programID) return false;
        
        // g_state.uVP = glGetUniformLocation(g_state.programID, "vp");
        // g_state.uModel = glGetUniformLocation(g_state.programID, "model");

        g_state.uCameraPos    = glGetUniformLocation(g_state.programID, "u_CameraPos");
        g_state.uCameraDir    = glGetUniformLocation(g_state.programID, "u_CameraDir");
        g_state.uCameraUp     = glGetUniformLocation(g_state.programID, "u_CameraUp");
        g_state.uResolution   = glGetUniformLocation(g_state.programID, "u_Resolution");

        g_state.textureID = create3DVoxelTexture();

        glDetachShader(g_state.programID, vertex);
        glDetachShader(g_state.programID, fragment);

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return true;
    }

    // MeshHandle create_mesh(const MeshDesc *desc)
    // {


    //     GLuint vbo;
    //     glGenBuffers(1, &vbo);
    //     glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //     glBufferData(GL_ARRAY_BUFFER, desc->vertexCount * sizeof(float), desc->positions, GL_STATIC_DRAW);
    //     glEnableVertexAttribArray(0);
    //     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    //     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    //     // const float colors[] = {
    //     //     1.0f, 0.0f, 0.0f,
    //     //     0.0f, 1.0f, 0.0f,
    //     //     0.0f, 0.0f, 1.0f,
    //     //     1.0f, 1.0f, 1.0f,
    //     //     1.0f, 0.0f, 0.0f,
    //     //     0.0f, 1.0f, 0.0f,
    //     //     0.0f, 0.0f, 1.0f,
    //     //     1.0f, 1.0f, 1.0f
    //     // };

    //     // GLuint color_vbo;
    //     // glGenBuffers(1, &color_vbo);
    //     // glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    //     // glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    //     // glEnableVertexAttribArray(1);
    //     // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //     // GLuint index_vbo;
    //     // glGenBuffers(1, &index_vbo);
    //     // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
    //     // glBufferData(GL_ELEMENT_ARRAY_BUFFER, desc->indexCount * sizeof(uint32_t), desc->indices, GL_STATIC_DRAW);

    //     glBindBuffer(GL_ARRAY_BUFFER, 0);
    //     glBindVertexArray(0);

    //     return g_meshes.create({vao, vbo, 0, desc->vertexCount, desc->indexCount});
    // }

    void shutdown()
    {
        glDeleteProgram(g_state.programID);
    }

    void render_frame(const FramePacket* packet)
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(g_state.programID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, g_state.textureID);
        GLint voxelGridLocation = glGetUniformLocation(g_state.programID, "u_VoxelGrid");
        glUniform1i(voxelGridLocation, 0);
        
        // glUniformMatrix4fv(g_state.uVP, 1, GL_FALSE, packet->viewProj.m);
        // for(auto f : packet->viewProj.m) {
        //     std::cout << f << ", " << std::ends;
        // }
        // std::cout << std::endl;
        glUniform3f(g_state.uCameraPos, packet->camPos.x,packet->camPos.y,packet->camPos.z);
        glUniform3f(g_state.uCameraDir, packet->camDir.x,packet->camDir.y,packet->camDir.z);
        glUniform3f(g_state.uCameraUp, packet->camUp.x,packet->camUp.y,packet->camUp.z);
        glUniform2f(g_state.uResolution, (float)packet->viewportWidth, (float)packet->viewportHeight);

        for (uint32_t i = 0; i < packet->drawCount; i++)
        {
            const DrawCommand& draw = packet->draws[i];
            GLMesh* mesh = gl_resolveMesh(draw.mesh);
            glBindVertexArray(mesh->vao);

            // glUniformMatrix4fv(g_state.uModel, 1, GL_FALSE, draw.model.m);

            // glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        // glBindVertexArray(0);
    }
}

RendererAPI createGLRenderer()
{
    return {RENDERER_API_VERSION, "OpenGL",
            gl_init, shutdown, gl_createMesh,
            gl_destroyMesh, render_frame};
}