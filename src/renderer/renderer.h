#ifndef FLOW_RENDERER_CONTRACT
#define FLOW_RENDERER_CONTRACT

#include <cstdint>
#include "renderer/renderer_types.h"

#define RENDERER_API_VERSION 1

struct NativeWindowInfo;

struct RendererAPI {
    uint32_t apiVersion = 0;
    const char* name = "unknown";

    bool (*init)(const NativeWindowInfo* native)   = nullptr;
    void (*shutdown)(void)                         = nullptr;
    MeshHandle (*createMesh)(const MeshDesc* desc) = nullptr;
    void       (*destroyMesh)(MeshHandle mesh)     = nullptr;
    void (*renderFrame)(const FramePacket* packet) = nullptr;
};

RendererAPI createGLRenderer();
RendererAPI createNullRenderer();

#endif