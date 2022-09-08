cmake_minimum_required(VERSION 3.24.1)
project(Flow VERSION 0.0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_GENERATOR Ninja)

IF(UNIX AND NOT APPLE)
	set(LINUX TRUE)
ENDIF()

# add_custom_target(FLOW_SOURCES ALL
# 	COMMAND echo "FLOW_SOURCES target created successfully!"
# )

add_executable(${PROJECT_NAME})
add_subdirectory(src/platform)
add_subdirectory(src/flow)
add_subdirectory(src/components)
add_subdirectory(subprojects)





target_include_directories(${PROJECT_NAME} PUBLIC include)

add_subdirectory(include)

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
	# message($ENV{VK_ICD_FILENAMES})
	# message("TEST")
	# set($ENV{VK_ICD_FILENAMES} ${VULKAN_SDK}/share/vulkan/icd.d/MoltenVK_icd.json)
	# set($ENV{VK_LAYER_PATH} ${VULKAN_SDK}/share/vulkan/explicit_layer.d)

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

install(TARGETS ${PROJECT_NAME})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
