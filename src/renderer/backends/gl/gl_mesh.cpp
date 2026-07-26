#include "renderer/backends/gl/gl_mesh.h"
#include "core/pool.h"
#include <glad/gl.h>

namespace { Pool<GLMesh, MeshTag> g_meshes; }

void makeAttribVBO(int location, int size, const float* values, uint32_t count, GLuint vbo) {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), values, GL_STATIC_DRAW);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, 0, 0);
}

MeshHandle gl_createMesh(const MeshDesc* desc) {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLMesh mesh;
    mesh.vao = vao;

    makeAttribVBO(0, 3, desc->positions, desc->vertexCount, mesh.vbos[0]);

    if(desc->normals)
        makeAttribVBO(1, 3, desc->normals, desc->vertexCount, mesh.vbos[1]);
    if(desc->colors)
        makeAttribVBO(2, 3, desc->colors, desc->vertexCount, mesh.vbos[2]);
    if(desc->uvs)
        makeAttribVBO(3, 2, desc->uvs, desc->vertexCount, mesh.vbos[3]);

    if(desc->indices) {
        glGenBuffers(1, &mesh.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, desc->indexCount * sizeof(uint32_t), desc->indices, GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return g_meshes.create(mesh);
}

void gl_destroyMesh(MeshHandle mesh) {
    GLMesh &m = g_meshes.get(mesh);
    glDeleteBuffers(sizeof(m.vbos), m.vbos);
    g_meshes.destroy(mesh);
}

GLMesh* gl_resolveMesh(MeshHandle h) { 
    return g_meshes.tryGet(h); 
}