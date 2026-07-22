#pragma once

#include "renderer/renderer_types.h"
#include "core/file_io.h"

#include <glad/gl.h>

GLuint gl_loadShader(const std::string file, GLenum type);
GLuint gl_loadProgram(const GLuint vertexShader, const GLuint fragmentShader);