#include "renderer/renderer.h"

namespace {
    bool       null_init(const NativeWindowInfo*)                { return true; }
    void       null_shutdown()                 {}
    MeshHandle null_createMesh(const MeshDesc*) { return {}; }
    void       null_destroyMesh(MeshHandle)    {}
    MaterialHandle       null_createMaterial(const MaterialDesc*) { return {}; }
    void                 null_destroyMaterial(MaterialHandle) {}
    void       null_renderFrame(const FramePacket*) {}
}

RendererAPI createNullRenderer() {
    return { RENDERER_API_VERSION, "Null", WindowConfig{},
             null_init, null_shutdown,
             null_createMesh, null_destroyMesh,
             null_createMaterial, null_destroyMaterial,
             null_renderFrame };
}