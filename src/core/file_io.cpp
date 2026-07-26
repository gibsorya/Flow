#include "core/file_io.h"
#include "core/executable_path.h"

std::string read_shader_file(const std::string file_name) {
    namespace fs = std::filesystem;

    fs::path fullPath = executableDir() / "shaders" / file_name;
    std::string file_path = "./shaders/" + file_name;

    std::ifstream shader_file(fullPath, std::ios::binary);
    if(!shader_file.is_open()) {
        std::cerr << "Error opening shader file: " << fullPath << std::endl;
    }
    
    std::stringstream ss;
    ss << shader_file.rdbuf();
    return ss.str();
}
