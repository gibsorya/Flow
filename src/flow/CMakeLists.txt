target_sources(${FLOW_LIB_TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/foundation.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/debug/debug_tools.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/debug/debug_tools.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/errors/error.hpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/errors/error.cpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/errors/error_macros.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/errors/error_macros.cpp         
)

# cmake_minimum_required(VERSION 3.21.2)
# project(foundation)

# set(FOUNDATION_FILES 
#     #Header Files
#     foundation.hpp
# )


# set(DEBUG_FILES 
#     #Header Files
#     debug/debug_tools.hpp

#     #Source Files
#     debug/debug_tools.cpp
# )


# set(ERROR_FILES 
#     #Header Files
#     errors/error.hpp
#     errors/error_macros.hpp

#     #Source Files
#     errors/error.cpp
#     errors/error_macros.hpp
# )

# source_group("\\" FILES ${FOUNDATION_FILES})
# source_group("debug\\" FILES ${DEBUG_FILES})
# source_group("errors\\" FILES ${ERROR_FILES})

# set(PROJECT_FILES
#     ${FOUNDATION_FILES}
#     ${DEBUG_FILES}
#     ${ERROR_FILES}
# )

# add_library(${PROJECT_NAME} STATIC ${PROJECT_FILES})

# target_compile_options(${PROJECT_NAME} PUBLIC -fexceptions)

# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# target_link_libraries(${PROJECT_NAME} PUBLIC
#     volk
#     imgui
#     glm
#     stb
#     flow_engine
#     app

#     PRIVATE
#     glfw
# )
