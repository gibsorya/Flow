target_sources(FLOW_SOURCES PRIVATE app.cpp)

add_subdirectory(audio)
add_subdirectory(physics)
add_subdirectory(vk)

# cmake_minimum_required(VERSION 3.21.2)

# project(flow_engine LANGUAGES CXX)

# set(AUDIO_FILES
#     #Header Files

#     #Source Files
# )

# set(PHYSICS_FILES 
#     #Header Files

#     #Source Files
# )

# set(VULKAN_FILES 
#     #Header Files
#     vk/flow_context.hpp
#     vk/components/instances.hpp
#     vk/components/surfaces.hpp
#     vk/components/queues.hpp
#     vk/components/devices.hpp
#     vk/components/swapchains.hpp

#     #Source Files
#     vk/flow_context.cpp
#     vk/components/instances.cpp
#     vk/components/surfaces.cpp
#     vk/components/queues.cpp
#     vk/components/devices.cpp
#     vk/components/swapchains.cpp
# )

# source_group("audio\\" FILES ${AUDIO_FILES})
# source_group("physics\\" FILES ${PHYSICS_FILES})
# source_group("vk\\" FILES ${VULKAN_FILES})

# set(PROJECT_FILES
#     ${AUDIO_FILES}
#     ${PHYSICS_FILES}
#     ${VULKAN_FILES}
# )

# add_library(${PROJECT_NAME} STATIC ${PROJECT_FILES})

# target_compile_options(${PROJECT_NAME} PUBLIC -fexceptions)

# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# target_link_libraries(${PROJECT_NAME} PUBLIC
#     volk
#     imgui
#     glm
#     stb
#     foundation
#     app

#     PRIVATE
#     glfw
# )

# # set(FLOW_FILES 
# #     #Header Files
# #     flow/foundation.hpp
# #     flow/debug/debug_tools.hpp
# #     flow/errors/error.hpp
# #     flow/error_macros.hpp

# #     #Source Files
# #     flow/foundation.cpp
# #     flow/debug/debug_tools.cpp
# #     flow/errors/error.cpp
# #     flow/error_macros.hpp
# # )

# # set(PLATFORM_FILES 
# #     #Header Files
# #     os_windows.hpp

# #     #Source Files
# #     windows_main.cpp
# # )

