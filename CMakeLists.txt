cmake_minimum_required(VERSION 3.21.2)
project(FlowVK LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

add_executable("${PROJECT_NAME}" "src/main.cpp")

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIR})

add_subdirectory(external/glfw)
add_subdirectory(external/tinyobjloader)

include_directories( PUBLIC external/glfw/include
					 PUBLIC external/glm
					 PUBLIC external/stb
					 PUBLIC external/tinyobjloader)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

target_link_directories(${PROJECT_NAME} PRIVATE external/glfw/src)


set(LIBS Vulkan::Vulkan glfw)
target_link_libraries(${PROJECT_NAME} ${LIBS})

install(TARGETS "${PROJECT_NAME}" DESTINATION bin)
install(FILES "main.cpp" DESTINATION src)