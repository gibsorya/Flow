target_sources(${FLOW_LIB_TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/flow_context.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/flow_context.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/instances.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/instances.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/surfaces.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/surfaces.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/devices.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/devices.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/queues.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/queues.hpp 
        ${CMAKE_CURRENT_SOURCE_DIR}/components/swapchains.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/swapchains.hpp
        
        
        
        
        ${CMAKE_CURRENT_SOURCE_DIR}/components/renderpasses.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/buffers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/syncobjects.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/pipelines/pipelines.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/pipelines/pipelineinfos.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/renderpasses.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/buffers.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/syncobjects.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/pipelines/pipelines.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/components/pipelines/pipelineinfos.hpp
)