cmake_minimum_required(VERSION 3.22.3)
project(Flow VERSION 0.0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_GENERATOR Ninja)

IF(UNIX AND NOT APPLE)
	set(LINUX TRUE)
ENDIF()

add_custom_target(FLOW_SOURCES ALL
	COMMAND echo "FLOW_SOURCES target created successfully!"
)

add_subdirectory(src/flow)
add_subdirectory(src/engine)
add_subdirectory(subprojects)

add_executable(${PROJECT_NAME} ${FLOW_SOURCES})

add_subdirectory(src/platform)

target_include_directories(${PROJECT_NAME} PUBLIC include)

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)

find_package(glfw3 CONFIG REQUIRED)
# target_link_libraries(${PROJECT_NAME} glfw)

IF(WIN32)
	IF(NOT Vulkan_FOUND)
		# find_library(Vulkan_LIBRARY NAMES vulkan-1 vulkan PATHS ${PROJECT_SOURCE_DIR}/libs)

		IF(Vulkan_LIBRARY)
			set(Vulkan_FOUND ON)
			MESSAGE("Using bundled Vulkan library version")
		ENDIF()
	ENDIF()
ELSEIF(LINUX)
ELSEIF(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_MACOS_MVK -DVK_EXAMPLE_XCODE_GENERATED")
	set($ENV{VK_ICD_FILENAMES} $ENV{VULKAN_SDK}/share/vulkan/icd.d/MoltenVK_icd.json)
	set($ENV{VK_LAYER_PATH} $ENV{VULKAN_SDK}/share/vulkan/explicit_layer.d)

	target_include_directories(${PROJECT_NAME} PUBLIC "/opt/homebrew/include" $ENV{VULKAN_SDK}/include)
	target_link_directories(${PROJECT_NAME} PUBLIC "/opt/homebrew/lib" $ENV{VULKAN_SDK}/lib)
	
	find_library(GLFW_LIB libglfw.3.3.dylib "opt/homebrew/lib")
	target_link_libraries(${PROJECT_NAME} PUBLIC ${GLFW_LIB})

	IF(NOT Vulkan_FOUND)
		IF(Vulkan_LIBRARY)
			set(Vulkan_FOUND ON)
			MESSAGE("Using bundled Vulkan library version")
		ENDIF()
	ENDIF()
ENDIF(WIN32)

IF(NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
ELSE()
	message(STATUS ${Vulkan_LIBRARY})
ENDIF()

# IF(APPLE)
# 	set($ENV{VK_ICD_FILENAMES} $ENV{VULKAN_SDK}/sha)
# ENDIF()

# add_subdirectory(src/app)
# add_subdirectory(src/flow)
# add_subdirectory(src/engine)

# add_subdirectory(subprojects)

# target_include_directories(${FLOW_LIB_TARGET}
# 	PUBLIC
# 	${PROJECT_BINARY_DIR}
# 	${PROJECT_SOURCE_DIR}/include
# )

# set(LIBS_DIR ${PROJECT_SOURCE_DIR}/libs)
# target_link_libraries(${FLOW_LIB_TARGET}
# 	PRIVATE
# 	${LIBS_DIR}/glfw/lib-mingw-w64/libglfw3.a
# 	${LIBS_DIR}/vulkan/vulkan-1.lib
# )

# target_compile_definitions(${FLOW_LIB_TARGET} PUBLIC _FLOWENGINE_)

# function(add_flowengine_target flowTarget flowRoot)
# 	IF(WIN32)
# 		IF(CMAKE_BUILD_TYPE STREQUAL Release)
# 			set_target_properties(
# 				${flowTarget} PROPERTIES
# 				LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
# 				LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
# 			)
# 		ENDIF()
# 	ENDIF()

# 	target_link_libraries(${flowTarget} FlowLib)
# 	target_link_directories(${flowTarget} PRIVATE ${flowRoot}/external/glfw/src)
# endfunction()

# IF(APPLE)
# 	target_include_directories(${FLOW_APP_BUILD_NAME} PUBLIC "/opt/homebrew/include" $ENV{VULKAN_SDK}/include)
# 	target_link_directories(${FLOW_APP_BUILD_NAME} PUBLIC "/opt/homebrew/lib" $ENV{VULKAN_SDK}/lib)
# ENDIF()

# add_subdirectory(src/platform)

# add_flowengine_target(${FLOW_APP_BUILD_NAME} ${PROJECT_SOURCE_DIR})

# install(TARGETS ${FLOW_APP_BUILD_NAME} DESTINATION build)
# install(FILES "main.cpp" DESTINATION src/platform/windows)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Example projects
# add_subdirectory(examples/HelloTriangle)
