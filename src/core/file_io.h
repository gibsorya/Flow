#ifndef FILE_IO
#define FILE_IO

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

static std::string read_shader_file(const std::string file_name) {
    std::string file_path = std::string(PROJECT_ROOT_DIR) + "/data/shaders/" + file_name;
    std::ifstream shader_file(file_path);

    if(!shader_file.is_open()) {
        std::cerr << "Error opening shader file: " << file_name << std::endl;
    }
    
    std::stringstream shader_code;
    shader_code << shader_file.rdbuf();

    shader_file.close();

    return shader_code.str();
}

#endif