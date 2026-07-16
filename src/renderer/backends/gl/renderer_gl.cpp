#include <iostream>

#include "renderer/renderer.h"
#include "core/pool.h"
#include "platform/window.h"
#include "core/file_io.h"

#include <glad/gl.h>
#include <glm/glm.hpp>

namespace
{
    struct GLState {
        GLuint   programID = 0;
        // grows: default framebuffer info, cached GL state, big buffers...
        GLuint uProj = 0;
    };
    GLState g_state;

    struct GLMesh
    {
        uint32_t vao;
        uint32_t vbo;
        uint32_t vertexCount;
        uint32_t indexCount;
    };

    static Pool<GLMesh, MeshTag> g_meshes;

    bool gl_init(const NativeWindowInfo *native)
    {
        char infoLog[512];
        int success;

        if (!gladLoadGL((GLADloadfunc)native->getProcAddress))
        {
            std::cerr << "Error loading GL functions" << std::endl;
            return false;
        }

        printf("GL VERSION: %s\n", glGetString(GL_VERSION));
        printf("GL RENDERER: %s", glGetString(GL_RENDERER));

        std::string vertCode = read_shader_file("main.vert");
        std::string fragCode = read_shader_file("main.frag");

        const char* vertSrc = vertCode.c_str();
        const char* fragSrc = fragCode.c_str();

        const GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertSrc, NULL);
        glCompileShader(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
            return false;
        };

        const GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragSrc, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
            return false;
        };

        g_state.programID = glCreateProgram();
        glAttachShader(g_state.programID, vertex);
        glAttachShader(g_state.programID, fragment);
        glLinkProgram(g_state.programID);

        glGetProgramiv(g_state.programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(g_state.programID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
            return false;
        }
        
        g_state.uProj = glGetUniformLocation(g_state.programID, "projection");

        glDetachShader(g_state.programID, vertex);
        glDetachShader(g_state.programID, fragment);

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return true;
    }

    MeshHandle create_mesh(const MeshDesc *desc)
    {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, desc->vertexCount, desc->positions, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        const float colors[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f
        };

        GLuint color_vbo;
        glGenBuffers(1, &color_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        GLuint index_vbo;
        glGenBuffers(1, &index_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, desc->indexCount, desc->indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return g_meshes.create({vao, vbo, desc->vertexCount, desc->indexCount});
    }

    void shutdown()
    {
        glDeleteProgram(g_state.programID);
    }

    void destroy_mesh(MeshHandle mesh)
    {
        GLMesh &m = g_meshes.get(mesh);
        glDeleteBuffers(1, &m.vbo);
        g_meshes.destroy(mesh);
    }

    void render_frame(const FramePacket *packet)
    {
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(g_state.programID);
        glUniformMatrix4fv(g_state.uProj, 1, GL_FALSE, packet->projMatrix);
        for (uint32_t i = 0; i < packet->drawCount; i++)
        {
            GLMesh &mesh = g_meshes.get(packet->draws[i].mesh);
            glBindVertexArray(mesh.vao);            
            glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
        }
    }
}

RendererAPI createGLRenderer()
{
    return {RENDERER_API_VERSION, "OpenGL",
            gl_init, shutdown, create_mesh,
            destroy_mesh, render_frame};
}