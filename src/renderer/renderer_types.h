#pragma once

#include "core/pool.h"

enum class RendererKind : uint32_t {
    OpenGL,
    Vulkan
};

struct MeshTag {};
using MeshHandle = Handle<MeshTag>;

struct MeshDesc {
    const float*    positions   = nullptr;  // 3 floats per vertex
    uint32_t        vertexCount = 0;

    const float*    normals     = nullptr;
    const float*    colors      = nullptr;
    const float*    uvs         = nullptr;
    
    const uint32_t* indices     = nullptr;  // optional: null = non-indexed
    uint32_t        indexCount  = 0;
};

struct Mat4 { float m[16]; };
struct Vec3 { float x; float y; float z; };

struct DrawCommand {
    MeshHandle mesh;
    Mat4 model;
    // later, in this order: MaterialHandle material;
    //                       uint32_t transformIndex;
    //                       uint64_t sortKey;
};
 
struct FramePacket {
    float              clearColor[4] = { 0.1f, 0.1f, 0.12f, 1.0f };
    uint32_t           viewportWidth  = 0;
    uint32_t           viewportHeight = 0;
 
    const DrawCommand* draws     = nullptr;
    uint32_t           drawCount = 0;
 
    // later: view/proj matrices, transforms array, lights...
    Mat4 viewProj;

    Vec3 camUp;
    Vec3 camDir;
    Vec3 camPos;
};
