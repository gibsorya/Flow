#pragma once

#include <glad/gl.h>

#include "renderer/renderer_types.h"

struct GLMaterial
{
    GLuint program;
    GLuint textureIDs[4];
    uint32_t textureCount;

    GLuint uCameraPos = -1;
    GLuint uCameraDir = -1;
    GLuint uCameraUp = -1;
    GLuint uResolution = -1;
};

MaterialHandle gl_createMaterial(const MaterialDesc* desc);
void gl_destroyMaterial(MaterialHandle h);
GLMaterial* gl_resolveMaterial(MaterialHandle h);