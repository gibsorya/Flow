#pragma once

#include "renderer/renderer_types.h"

struct GLMesh
{
    uint32_t vao;
    uint32_t vbos[4];
    uint32_t ebo;
    uint32_t vertexCount;
    uint32_t indexCount;
};

MeshHandle gl_createMesh(const MeshDesc* desc);   // "this function exists,
void       gl_destroyMesh(MeshHandle h);          //  here's its shape"
GLMesh*    gl_resolveMesh(MeshHandle h);