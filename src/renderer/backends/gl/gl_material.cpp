#include "renderer/backends/gl/gl_material.h"
#include "renderer/backends/gl/gl_shader.h"

namespace {
    Pool<GLMaterial, MaterialTag> g_materials;
}

GLMaterial* gl_resolveMaterial(MaterialHandle h) {
    return g_materials.tryGet(h);
}

MaterialHandle gl_createMaterial(const MaterialDesc* desc) {
    GLuint vertexShader = gl_loadShader(desc->vertexShaderPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = gl_loadShader(desc->fragmentShaderPath, GL_FRAGMENT_SHADER);

    if (!vertexShader || !fragmentShader) {
        return MaterialHandle{};
    }

    GLuint program = gl_loadProgram(vertexShader, fragmentShader);
    if (!program) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return MaterialHandle{};
    }

    // Clean up shaders after linking
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLMaterial material;
    material.program = program;
    material.textureCount = desc->textureCount;

    for (uint32_t i = 0; i < desc->textureCount && i < 4; ++i) {
        // Assuming textures are already created and valid
        material.textureIDs[i] = desc->textures[i].index; // Assuming TextureHandle has an index member
    }

    return g_materials.create(material);
}

void gl_destroyMaterial(MaterialHandle material) {
    GLMaterial &m = g_materials.get(material);
    glDeleteProgram(m.program);
    g_materials.destroy(material);
}