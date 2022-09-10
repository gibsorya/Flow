target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/flow_vk.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/instances.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/surfaces.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/devices.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/queues.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/swapchains.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pipelines.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_infos.cpp
)

MESSAGE(STATUS ${FLOW_SOURCES})