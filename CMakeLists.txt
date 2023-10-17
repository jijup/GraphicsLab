cmake_minimum_required(VERSION 3.10)

project(GraphicsLab)

#--- Load the common configuration
include(common/config.cmake)

#--- Load third party packages
include(common/Eigen.cmake)
include(common/OpenCV.cmake)

#--- Subprojects
#add_subdirectory(hello)
#add_subdirectory(vectors)
#add_subdirectory(raytrace)
#add_subdirectory(sampling)
#add_subdirectory(texturing)
add_subdirectory(rasterize)


#--- C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)









