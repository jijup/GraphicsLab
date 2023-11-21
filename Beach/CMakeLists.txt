get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SHADERS "*.glsl")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS} ${SHADERS})
if(WIN32)
        target_link_libraries(${EXERCISENAME} "legacy_stdio_definitions.lib")
endif()
target_link_libraries(${EXERCISENAME} ${COMMON_LIBS})

# Texture imports
file(COPY ${PROJECT_SOURCE_DIR}/Beach/Textures/sand.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/Beach/Textures/water.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})



