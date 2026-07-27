#include <iostream>

#include "renderer/renderer.h"
#include "core/pool.h"
#include "platform/window.h"
#include "core/file_io.h"
#include "renderer/backends/gl/gl_mesh.h"
#include "renderer/backends/gl/gl_shader.h"
#include "renderer/backends/gl/gl_material.h"

#include <glad/gl.h>

namespace
{
    struct GLState {
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

    const int GRID_WIDTH = 256;
    const int GRID_HEIGHT = 256;
    const int GRID_DEPTH = 256;  
    GLuint create3DVoxelTexture()
    {
        uint32_t solidCount = 0;
        std::vector<unsigned char> voxelData(GRID_WIDTH * GRID_HEIGHT * GRID_DEPTH * 4);

        for (int z = 0; z < GRID_DEPTH; ++z)
        {
            for (int y = 0; y < GRID_HEIGHT; ++y)
            {
                for (int x = 0; x < GRID_WIDTH; ++x)
                {
                    int index = (x + y * GRID_WIDTH + z * GRID_WIDTH * GRID_HEIGHT) * 4;
                    

                    // Simple generation logic: Build a floor and a few random floating islands
                    if (y < 20 || (y < 30 && std::rand() % 100 > 60))
                    {
                        voxelData[index + 0] = static_cast<unsigned char>(x * 4); // R
                        voxelData[index + 1] = static_cast<unsigned char>(y * 4); // G
                        voxelData[index + 2] = static_cast<unsigned char>(z * 4); // B
                        voxelData[index + 3] = 255;                               // Alpha (Solid)
                        solidCount++;
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

        printf("Solid voxels: %u / %u (%.2f%% filled)\n",
               solidCount, GRID_WIDTH * GRID_HEIGHT * GRID_DEPTH,
               100.0f * solidCount / (GRID_WIDTH * GRID_HEIGHT * GRID_DEPTH));

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

        return true;
    }

    void shutdown()
    {
        // glDeleteProgram(g_state.programID);
    }

    void render_frame(const FramePacket* packet)
    {
        glClearColor(packet->clearColor[0], packet->clearColor[1], packet->clearColor[2], packet->clearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, packet->viewportWidth, packet->viewportHeight);

        GLuint lastProgram = 0;

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_3D, g_state.textureID);
        // GLint voxelGridLocation = glGetUniformLocation(g_state.programID, "u_VoxelGrid");
        // glUniform1i(voxelGridLocation, 0);

        if(packet->viewProj.m[0] == 0 && packet->viewProj.m[5] == 0 && packet->viewProj.m[10] == 0 && packet->viewProj.m[15] == 0) {
            std::cerr << "Warning: viewProj matrix is uninitialized (all zeros). This may lead to incorrect rendering." << std::endl;
        } else {
            glUniformMatrix4fv(g_state.uVP, 1, GL_FALSE, packet->viewProj.m);
        }

        for (uint32_t i = 0; i < packet->drawCount; i++)
        {
            const DrawCommand& draw = packet->draws[i];
            GLMaterial* mat = gl_resolveMaterial(draw.material);
            if(mat->program != lastProgram) {
                glUseProgram(mat->program);
                glUniform3f(mat->uCameraPos, packet->camPos.x,packet->camPos.y,packet->camPos.z);
                glUniform3f(mat->uCameraDir, packet->camDir.x,packet->camDir.y,packet->camDir.z);
                glUniform3f(mat->uCameraUp, packet->camUp.x,packet->camUp.y,packet->camUp.z);
                glUniform2f(mat->uResolution, (float)packet->viewportWidth, (float)packet->viewportHeight);
                lastProgram = mat->program;
            }
            
            GLMesh* mesh = gl_resolveMesh(draw.mesh);
            glBindVertexArray(mesh->vao);

            // glUniformMatrix4fv(g_state.uModel, 1, GL_FALSE, draw.model.m);

            // glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        // glBindVertexArray(0);
    }
}

RendererAPI createGLRenderer()
{
    return {RENDERER_API_VERSION, "OpenGL", WindowConfig{},
            gl_init, shutdown, gl_createMesh,
            gl_destroyMesh, gl_createMaterial, gl_destroyMaterial, render_frame};
}