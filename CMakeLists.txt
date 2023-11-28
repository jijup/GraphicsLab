cmake_minimum_required(VERSION 3.10)

project(GraphicsLab)

#--- Load the common configuration
include(common/config.cmake)

#--- Load third party packages
include(common/Eigen.cmake)
include(common/OpenCV.cmake)
include(common/OpenGP.cmake)

#--- OpenGL configuration
include(common/OpenGL.cmake)
include(common/GLEW.cmake)
include(common/GLFW.cmake)

#--- Subprojects
#add_subdirectory(hello)
#add_subdirectory(vectors)
#add_subdirectory(raytrace)
#add_subdirectory(sampling)
#add_subdirectory(texturing)
#add_subdirectory(rasterize)
#add_subdirectory(shaders)
#add_subdirectory(PerlinNoise)
#add_subdirectory(Beach)
add_subdirectory(Mesh3D)

#---Assignments
#add_subdirectory(Terrains)

#-----GLSL examples
#add_subdirectory(apps/Ex1_Simple)
#add_subdirectory(apps/Ex2_Shaders)
#add_subdirectory(apps/Ex3_Geometry)
#add_subdirectory(apps/Ex4_Shading)


#--- C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)









