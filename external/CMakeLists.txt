cmake_minimum_required(VERSION 3.21.2)

add_subdirectory(vulkan)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(tinyobjloader)

# GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_INSTALL OFF)

# # glm

# set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm)
# target_sources(glm INTERFACE ${GLM_DIR})
# target_include_directories(glm INTERFACE ${GLM_DIR})

# target_compile_definitions(glm INTERFACE
#     GLM_FORCE_SWIZZLE
#     GLM_FORCE_RADIANS
#     GLM_FORCE_CTOR_INIT
#     GLM_ENABLE_EXPERIMENTAL
# )

# # volk
# set(VOLK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/volk")
# set(VOLK_FILES
#     "${VOLK_DIR}/volk.c"
#     "${VOLK_DIR}/volk.h")


# add_library(volk STATIC ${VOLK_FILES})
# target_link_libraries(volk PUBLIC vulkan)

# target_include_directories(volk PUBLIC ${VOLK_DIR})
# if (VKB_WSI_SELECTION STREQUAL XCB)
#     target_include_directories(volk PUBLIC ${XCB_INCLUDE_DIRS})
# elseif (VKB_WSI_SELECTION STREQUAL XLIB)
#     target_include_directories(volk PUBLIC ${X11_INCLUDE_DIRS})
# elseif (VKB_WSI_SELECTION STREQUAL WAYLAND)
#     target_include_directories(volk PUBLIC ${WAYLAND_INCLUDE_DIRS})
# endif()

# set_property(TARGET volk PROPERTY FOLDER "ThirdParty")

# # imgui
# set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
# set(IMGUI_FILES
#     "${IMGUI_DIR}/imgui.cpp"
#     "${IMGUI_DIR}/imgui_demo.cpp"
#     "${IMGUI_DIR}/imgui_draw.cpp"
#     "${IMGUI_DIR}/imgui_widgets.cpp"
#     "${IMGUI_DIR}/imconfig.h"
#     "${IMGUI_DIR}/imgui.h"
#     "${IMGUI_DIR}/imgui_internal.h"
#     "${IMGUI_DIR}/imstb_rectpack.h"
#     "${IMGUI_DIR}/imstb_textedit.h"
#     "${IMGUI_DIR}/imstb_truetype.h")

# add_library(imgui STATIC ${IMGUI_FILES})

# target_include_directories(imgui PUBLIC ${IMGUI_DIR})

# set_property(TARGET imgui PROPERTY FOLDER "ThirdParty")



# add_subdirectory(glfw)

# set_property(TARGET glfw PROPERTY FOLDER "ThirdParty")
# target_include_directories(${FLOW_LIB_TARGET} PRIVATE glfw/src)

# # tinyobjectloader
# 