cmake_minimum_required(VERSION 3.21)
project(flow LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

# ------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------
include(FetchContent)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863 # Corresponds to GLM 1.0.1+
)

# Make the library available to your build
FetchContent_MakeAvailable(glm)

add_subdirectory(external/glad)

# ------------------------------------------------------------
# engine_core — contracts + engine-agnostic utilities.
# ------------------------------------------------------------
 
add_library(engine_core STATIC
    src/core/file_io.cpp
    src/core/executable_path.cpp
)
target_include_directories(engine_core PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_compile_features(engine_core INTERFACE cxx_std_20)

# ------------------------------------------------------------
# platform_glfw — window backend. Sole owner of GLFW.
# ------------------------------------------------------------
# glfw is linked PRIVATE: its headers are visible inside this
# target's .cpp files and nowhere else. engine_core is PUBLIC
# so this target can implement the window.h contract.
 
add_library(platform_glfw STATIC
    src/platform/backends/glfw/window_glfw.cpp
)
target_link_libraries(platform_glfw
    PUBLIC  engine_core
    PRIVATE glfw
)

# ------------------------------------------------------------
# renderer_gl — GL backend. Sole owner of glad/OpenGL.
# ------------------------------------------------------------
# Note glad is PRIVATE for the same reason. The GL loader
# function should reach this target through the contract
# (e.g. a getProcAddress pointer in NativeWindowInfo), NOT by
# linking GLFW here — keep this target GLFW-free.
 
add_library(renderer_gl STATIC
    src/renderer/backends/gl/renderer_gl.cpp
    src/renderer/backends/gl/gl_shader.cpp
    src/renderer/backends/gl/gl_mesh.cpp
)
target_link_libraries(renderer_gl
    PUBLIC  engine_core
    PRIVATE glad
)

target_compile_definitions(renderer_gl PRIVATE
    PROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)
 
# ------------------------------------------------------------
# renderer_null — headless backend and boundary canary.
# ------------------------------------------------------------
# Links ONLY engine_core. If the contracts ever grow a GL or
# GLFW dependency, THIS target stops compiling first. Keep it
# in the default build precisely for that reason.
 
add_library(renderer_null STATIC
    src/renderer/backends/null/renderer_null.cpp
)
target_link_libraries(renderer_null PUBLIC engine_core)

# ------------------------------------------------------------
# engine — the executable
# ------------------------------------------------------------
 
add_executable(engine src/main.cpp)
target_link_libraries(engine
    PRIVATE
        engine_core
        platform_glfw
        renderer_gl
        renderer_null
        glm::glm
)

# ---- warnings: on for OUR targets, not for dependencies ----
set(ENGINE_TARGETS engine platform_glfw renderer_gl renderer_null)
foreach(t IN LISTS ENGINE_TARGETS)
    if(MSVC)
        target_compile_options(${t} PRIVATE /W4)
    else()
        target_compile_options(${t} PRIVATE -Wall -Wextra -Wshadow)
    endif()
endforeach()

# ---- shaders: copy next to the binary after every build ----
add_custom_command(TARGET engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data/shaders
        $<TARGET_FILE_DIR:engine>/data/shaders
    COMMENT "Copying shaders"
)

# ------------------------------------------------------------
# Future backends slot in as new targets, no changes elsewhere:
#
#   add_library(renderer_vk STATIC src/renderer/backends/vk/...)
#   target_link_libraries(renderer_vk PUBLIC engine_core
#                                     PRIVATE Vulkan::Vulkan)
#
#   add_library(platform_win32 STATIC src/platform/backends/win32/...)
#   # links nothing extra: Win32 API ships with the toolchain
# ------------------------------------------------------------