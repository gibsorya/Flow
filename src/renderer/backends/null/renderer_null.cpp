#include "renderer/renderer.h"

namespace {
    bool       null_init(const NativeWindowInfo*)                { return true; }
    void       null_shutdown()                 {}
    MeshHandle null_createMesh(const MeshDesc*) { return {}; }
    void       null_destroyMesh(MeshHandle)    {}
    void       null_renderFrame(const FramePacket*) {}
}

RendererAPI createNullRenderer() {
    return { RENDERER_API_VERSION, "Null",
             null_init, null_shutdown,
             null_createMesh, null_destroyMesh,
             null_renderFrame };
}