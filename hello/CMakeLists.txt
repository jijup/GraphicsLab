cmake_minimum_required(VERSION 3.10)

get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS})
