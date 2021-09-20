cmake_minimum_required(VERSION 3.21.2)
project(FlowVK LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/**.c
	${CMAKE_SOURCE_DIR}/src/**.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/**.h
	${CMAKE_SOURCE_DIR}/src/**.hpp)


# find_package(Vulkan REQUIRED)

add_executable("${PROJECT_NAME}" "src/main.cpp")

find_package(Vulkan)

#Change this to where you installed the Vulkan SDK on your system
include_directories(${Vulkan_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw)

if(WIN32)
	if(NOT Vulkan_FOUND)
		find_library(Vulkan_LIBRARY NAMES vulkan-1 vulkan PATHS ${VULKAN_SDK}/1.2.182.0/libs/vulkan)
		if(Vulkan_LIBRARY)
			set(Vulkan_FOUND ON)
			MESSAGE("Using bundled Vulkan library version")
		endif()
	endif()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WIN32_KHR")
elseif(LINUX)
	IF (NOT Vulkan_FOUND)
		find_library(Vulkan_LIBRARY NAMES vulkan HINTS "$ENV{VULKAN_SDK}/lib" "${CMAKE_SOURCE_DIR}/libs/vulkan" REQUIRED)
		IF (Vulkan_LIBRARY)
			set(Vulkan_FOUND ON)
			MESSAGE("Using bundled Vulkan library version")
		ENDIF()
	ENDIF()
	find_package(Threads REQUIRED)
	IF(USE_D2D_WSI)
		MESSAGE("Using direct to display extension...")
		add_definitions(-D_DIRECT2DISPLAY)
	ELSEIF(USE_DIRECTFB_WSI)
		find_package(DirectFB REQUIRED)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_DIRECTFB_EXT")
		include_directories(${DIRECTFB_INCLUDE_DIR})
	ELSEIF(USE_WAYLAND_WSI)
		find_program(PKG_CONFIG pkg-config)
		if (NOT PKG_CONFIG)
			message(FATAL_ERROR "pkg-config binary not found")
		endif ()
		find_package(Wayland REQUIRED)
		if (NOT WAYLAND_FOUND)
			message(FATAL_ERROR "Wayland development package not found")
		endif ()
		pkg_check_modules(WAYLAND_PROTOCOLS REQUIRED wayland-protocols)
		if (NOT WAYLAND_PROTOCOLS_FOUND)
			message(FATAL_ERROR "Wayland protocols package not found")
		endif ()
		find_program(WAYLAND_SCANNER wayland-scanner)
		if (NOT WAYLAND_SCANNER)
			message(FATAL_ERROR "wayland-scanner binary not found")
		endif ()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WAYLAND_KHR")
		include_directories(${WAYLAND_INCLUDE_DIR})
		pkg_get_variable(protocol_dir wayland-protocols pkgdatadir)
		execute_process(COMMAND ${WAYLAND_SCANNER} client-header ${protocol_dir}/stable/xdg-shell/xdg-shell.xml ${CMAKE_BINARY_DIR}/xdg-shell-client-protocol.h
				COMMAND ${WAYLAND_SCANNER} private-code ${protocol_dir}/stable/xdg-shell/xdg-shell.xml ${CMAKE_BINARY_DIR}/xdg-shell-protocol.c)
		include_directories(${CMAKE_BINARY_DIR})
	ELSEIF(USE_HEADLESS)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_HEADLESS_EXT")
	ELSE(USE_D2D_WSI)
		find_package(XCB REQUIRED)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_XCB_KHR")
	ENDIF(USE_D2D_WSI)
elseif(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_MACOS_MVK -DVK_EXAMPLE_XCODE_GENERATED")
endif(WIN32)

if(NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
else()
	message(STATUS ${Vulkan_LIBRARY})
endif()

if(WIN32)
	link_libraries(${Vulkan_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
endif()
# find_package(Vulkan REQUIRED)
# include_directories(${VULKAN_SDK}/)
# option(AUTO_LOCATE_VULKAN "AUTO_LOCATE_VULKAN" OFF)

# if(AUTO_LOCATE_VULKAN)
# 	message(STATUS "Attempting auto locate Vulkan using CMake......")
	
# 	# Find Vulkan Path using CMake's Vulkan Module
# 	# This will return Boolean 'Vulkan_FOUND' indicating the status of find as success(ON) or fail(OFF).
# 	# Include directory path - 'Vulkan_INCLUDE_DIRS' and 'Vulkan_LIBRARY' with required libraries.
# 	find_package(Vulkan)
	
# 	# Try extracting VulkanSDK path from ${Vulkan_INCLUDE_DIRS}
# 	if (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
# 		set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
# 		STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
# 	endif()
		 
# 	if(NOT Vulkan_FOUND)
# 		# CMake may fail to locate the libraries but could be able to 
# 		# provide some path in Vulkan SDK include directory variable
# 		# 'Vulkan_INCLUDE_DIRS', try to extract path from this.
# 		message(STATUS "Failed to locate Vulkan SDK, retrying again...")
# 		if(EXISTS "${VULKAN_PATH}")
# 			message(STATUS "Successfully located the Vulkan SDK: ${VULKAN_PATH}")
# 		else()
# 			message("Error: Unable to locate Vulkan SDK. Please turn off auto locate option by specifying 'AUTO_LOCATE_VULKAN' as 'OFF'")
# 			message("and specify manually path using 'VULKAN_SDK' and 'VULKAN_VERSION' variables in the CMakeLists.txt.")
# 			return()
# 		endif()
# 	endif()
# else()
# 	message(STATUS "Attempting to locate Vulkan SDK using manual path......")
# 	set(VULKAN_SDK "C:/VulkanSDK")
# 	set(VULKAN_VERSION "1.2.182.0")
# 	set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")
# 	message(STATUS "Using manual specified path: ${VULKAN_PATH}")

# 	# Check if manual set path exists
# 	if(NOT EXISTS "${VULKAN_PATH}")
# 		message("Error: Unable to locate this Vulkan SDK path VULKAN_PATH: ${VULKAN_PATH}, please specify correct path.")
# 	   return()
# 	endif()
# endif()

# # Add any required preprocessor definitions here
# add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)

# # vulkan-1 library for build Vulkan application.
# set(VULKAN_LIB_LIST "vulkan-1")

# if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
# 	# Include Vulkan header files from Vulkan SDK
# 	include_directories(AFTER ${VULKAN_PATH}/Include)

# 	# Link directory for vulkan-1
# 	link_directories(${VULKAN_PATH}/Bin ${VULKAN_PATH}/Lib)
# endif()

# target_link_libraries(${PROJECT_NAME} ${VULKAN_LIB_LIST} )

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

# set(LIBS Vulkan glfw)
# target_link_libraries(${PROJECT_NAME} ${LIBS})

install(TARGETS "${PROJECT_NAME}" DESTINATION bin)
install(FILES "main.cpp" DESTINATION src)