cmake_minimum_required(VERSION 3.21.2)
project(HelloTriangle)

set(HELLO_TRIANGLE_APP HelloTriangle)
add_executable(${HELLO_TRIANGLE_APP} "")

target_sources(${HELLO_TRIANGLE_APP}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/HelloTriangle.cpp    
)

target_link_libraries(${HELLO_TRIANGLE_APP} FlowLib)