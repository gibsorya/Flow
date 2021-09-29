cmake_minimum_required(VERSION 3.21.2)
project(FlowVK VERSION 0.0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

configure_file(FlowConfig.h.in FlowConfig.h)

set(FLOW_LIB_TARGET FlowLib)
add_library(${FLOW_LIB_TARGET} STATIC "")

add_subdirectory(src/engine)
add_subdirectory(src/engine/flow)
add_subdirectory(src/engine/vk)



find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIR})

# add_subdirectory(cmake)
add_subdirectory(external/tinyobjloader)

target_include_directories(${FLOW_LIB_TARGET}
	PUBLIC
		${PROJECT_BINARY_DIR}
		${PROJECT_SOURCE_DIR}/external
		${PROJECT_SOURCE_DIR}/external/glfw/include
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
	target_include_directories(${flowTarget} PUBLIC
		${PROJECT_BINARY_DIR}
		${PROJECT_SOURCE_DIR}/external/
		${PROJECT_SOURCE_DIR}/external/glfw/include
		${PROJECT_SOURCE_DIR}/src	
	)

	add_custom_command(TARGET ${flowTarget} PRE_LINK
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${flowRoot}/data $<TARGET_FILE_DIR:${flowTarget}>/data
	)

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

add_subdirectory(src/app)

add_flowengine_target(${FLOW_APP_BUILD_NAME} ${PROJECT_SOURCE_DIR})

install(TARGETS ${FLOW_APP_BUILD_NAME} DESTINATION build)
install(FILES "main.cpp" DESTINATION src/app)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)



