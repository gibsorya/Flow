target_sources(${FLOW_APP_BUILD_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/windows_main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/os_windows.hpp        
)

# cmake_minimum_required(VERSION 3.21.2)
# project(Flow LANGUAGES C CXX)

# set(CMAKE_CXX_STANDARD 20)


# set(SRC
#     os_windows.hpp
#     windows_main.cpp
# )

# source_group("\\" FILES ${SRC})

# add_executable(${PROJECT_NAME} WIN32 ${SRC})

# target_include_directories(${PROJECT_NAME} PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${CMAKE_CURRENT_SOURCE_DIR}/../../engine/vk
#     ${CMAKE_CURRENT_SOURCE_DIR}/../../app
#     ${CMAKE_CURRENT_SOURCE_DIR}/../../foundation)

# target_link_libraries(${PROJECT_NAME} PRIVATE foundation flow_engine app)