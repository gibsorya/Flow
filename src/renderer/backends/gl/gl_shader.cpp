#include "renderer/backends/gl/gl_shader.h"
#include "core/pool.h"

GLuint gl_loadShader(const std::string file, GLenum type)
{
    char infoLog[512];
    int success;

    std::string shader_code = read_shader_file(file);
    const char* shader_src = shader_code.c_str();

    const GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shader_src, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << std::to_string(type) << "::COMPILATION_FAILED\n"
                    << infoLog << std::endl;
        return false;
    };

    return shader;
}

GLuint gl_loadProgram(const GLuint vertexShader, const GLuint fragmentShader)
{
    char infoLog[512];
    int success;

    GLuint id = glCreateProgram();

    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                    << infoLog << std::endl;
        return false;
    }

    return id;
}