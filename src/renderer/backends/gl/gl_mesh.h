#pragma once

#include "renderer/renderer_types.h"

MeshHandle gl_createMesh(const MeshDesc* desc);   // "this function exists,
void       gl_destroyMesh(MeshHandle h);          //  here's its shape"

struct GLMesh
{
    uint32_t vao;
    uint32_t vbo;
    uint32_t color_vbo;
    uint32_t vertexCount;
    uint32_t indexCount;
};