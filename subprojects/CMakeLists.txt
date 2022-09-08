add_subdirectory(glfw)
# add_subdirectory(glm)
add_subdirectory(tinyobjloader)

# GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)

# stb
add_library(stb INTERFACE)
set(STB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stb)
target_sources(stb INTERFACE ${STB_DIR}/stb_image.h)
target_include_directories(stb INTERFACE ${STB_DIR})