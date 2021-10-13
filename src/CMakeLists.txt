cmake_minimum_required(VERSION 3.21.2)

# Creates a list of the subdirectories within this folder
scan_dirs(
    DIR ${CMAKE_CURRENT_SOURCE_DIR}
    LIST SUB_DIRS)

# List of all samples
# set(TOTAL_FILE_LIST)

# function(add_sub_dirs DIRECTORY)
#     scan_dirs(
#         DIR ${DIRECTORY}
#         LIST DIR_LIST)

#     foreach(CURR_DIR ${DIR_LIST})
#         if(EXISTS "${DIRECTORY}/${CURR_DIR}/CMakeLists.txt")
#             add_subdirectory(${DIRECTORY}/${CURR_DIR})
#             list(APPEND TOTAL_FILE_LIST ${CURR_DIR})
#         else()
#             add_sub_dirs(${DIRECTORY}/${CURR_DIR})
#         endif()
#         set(TOTAL_FILE_LIST ${TOTAL_FILE_LIST} PARENT_SCOPE)
#     endforeach()
# endfunction(add_sub_dirs)

# foreach(SUB_DIR ${SUB_DIRS})
#     add_sub_dirs(${CMAKE_CURRENT_SOURCE_DIR}/${SUB_DIR})
# endforeach()

# set(LIST)

# foreach(SAMPLE_ID ${TOTAL_SAMPLE_ID_LIST})
#     list(FIND LIST ${SAMPLE_ID} FOUND)
#     if(${FOUND} LESS 0)
#         list(APPEND LIST ${SAMPLE_ID})
#     endif()
# endforeach()

# set(TOTAL_FILE_LIST ${LIST} PARENT_SCOPE)

cmake_minimum_required(VERSION 3.21.2)

add_subdirectory(foundation)
add_subdirectory(app)
add_subdirectory(engine)
add_subdirectory(platform)