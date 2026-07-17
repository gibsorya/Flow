#include "renderer/backends/gl/gl_mesh.h"
#include "core/pool.h"
#include <glad/gl.h>

namespace { Pool<GLMesh, MeshTag> g_meshes; }

MeshHandle gl_createMesh(const MeshDesc* desc) {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, desc->vertexCount * sizeof(float), desc->positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return g_meshes.create({vao, vbo, 0, desc->vertexCount, desc->indexCount});
}

void gl_destroyMesh(MeshHandle mesh) {
    GLMesh &m = g_meshes.get(mesh);
    glDeleteBuffers(1, &m.vbo);
    g_meshes.destroy(mesh);
}