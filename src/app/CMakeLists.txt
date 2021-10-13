target_sources(${FLOW_LIB_TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/app.cpp     
        ${CMAKE_CURRENT_SOURCE_DIR}/app.hpp           
)

# cmake_minimum_required(VERSION 3.21.2)

# project(app)

# set(SRC
#     app.cpp
#     app.hpp
# )

# source_group("\\" FILES ${SRC})
# set(PROJECT_FILES
#     ${SRC}
# )

# add_library(${PROJECT_NAME} STATIC ${PROJECT_FILES})

# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# target_link_libraries(${PROJECT_NAME} PRIVATE flow_engine foundation)