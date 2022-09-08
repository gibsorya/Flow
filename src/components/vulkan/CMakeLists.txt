target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/flow_vk.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/instances.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/surfaces.cpp
)

MESSAGE(STATUS ${FLOW_SOURCES})