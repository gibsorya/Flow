cmake_minimum_required(VERSION 3.21.2)
project(Flow VERSION 0.0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

set(FLOW_LIB_TARGET FlowLib)
add_library(${FLOW_LIB_TARGET} STATIC "")


add_subdirectory(src/app)
add_subdirectory(src/flow)
add_subdirectory(src/engine)

add_subdirectory(external)

target_link_libraries(${FLOW_LIB_TARGET} PUBLIC
	Vulkan-Headers
    # imgui
    glm
    # stb
	glfw
	tinyobjloader
)

target_include_directories(${FLOW_LIB_TARGET}
	PUBLIC
		${PROJECT_BINARY_DIR}
		${PROJECT_SOURCE_DIR}/src	
)

set(LIBS_DIR ${PROJECT_SOURCE_DIR}/libs)
target_link_libraries(${FLOW_LIB_TARGET}
	PRIVATE
		${LIBS_DIR}/glfw/lib-mingw-w64/libglfw3.a
		${LIBS_DIR}/vulkan/vulkan-1.lib
)


target_compile_definitions(${FLOW_LIB_TARGET} PUBLIC _FLOWENGINE_)

function(add_flowengine_target flowTarget flowRoot)
	if(WIN32)
		if(CMAKE_BUILD_TYPE STREQUAL Release)
			set_target_properties(
				${flowTarget} PROPERTIES
				LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
				LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
			)
		endif()
	endif()

	target_link_libraries(${flowTarget} FlowLib)
	target_link_directories(${flowTarget} PRIVATE ${flowRoot}/external/glfw/src)
endfunction()

set(FLOW_APP_BUILD_NAME Flow)
add_executable(${FLOW_APP_BUILD_NAME} "")

add_subdirectory(src/platform)

add_flowengine_target(${FLOW_APP_BUILD_NAME} ${PROJECT_SOURCE_DIR})

install(TARGETS ${FLOW_APP_BUILD_NAME} DESTINATION build)
install(FILES "main.cpp" DESTINATION src/platform/windows)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)



