if(UNIX) # Mac or Linux
    # OpenCV found through PkgConfig
    find_package(PkgConfig)
    if(PKGCONFIG_FOUND)
        pkg_check_modules(OPENCV opencv)
        list(APPEND COMMON_LIBS ${OPENCV_LDFLAGS})
        include_directories(${OPENCV_INCLUDE_DIRS})
        if(OPENCV_FOUND)
            add_definitions(-DWITH_OPENCV)
        endif()
    endif()

elseif(WIN32)
    #--- PRE-COMPILED STATIC LIBRARIES
    include_directories("${CMAKE_SOURCE_DIR}/external/OpenCV/include")

    set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")

    set(CV_CORE 
        optimized ${CMAKE_SOURCE_DIR}/external/OpenCV/lib/opencv_core453.lib
        debug ${CMAKE_SOURCE_DIR}/external/OpenCV/lib/opencv_core453d.lib)
	set(CV_IMGCODECS 
        optimized ${CMAKE_SOURCE_DIR}/external/OpenCV/lib/opencv_imgcodecs453.lib
        debug ${CMAKE_SOURCE_DIR}/external/OpenCV/lib/opencv_imgcodecs453d.lib)
	set(CV_IMGPROC 
        optimized ${CMAKE_SOURCE_DIR}/external/OpenCV/lib/opencv_imgproc453.lib
        debug ${CMAKE_SOURCE_DIR}/external/OpenCV/lib/opencv_imgproc453d.lib)
	set(CV_VDOIO 
        optimized ${CMAKE_SOURCE_DIR}/external/OpenCV/lib/opencv_videoio453.lib
        debug ${CMAKE_SOURCE_DIR}/external/OpenCV/lib/opencv_videoio453d.lib)
    set(CV_GUI
        optimized ${CMAKE_SOURCE_DIR}/external/OpenCV/lib/opencv_highgui453.lib
        debug ${CMAKE_SOURCE_DIR}/external/OpenCV/lib/opencv_highgui453d.lib)
    set(ZLIB
        optimized ${CMAKE_SOURCE_DIR}/external/OpenCV/lib/zlib.lib
        debug ${CMAKE_SOURCE_DIR}/external/OpenCV/lib/zlibd.lib)

    if(NOT COMMON_LIBS)
        set(COMMON_LIBS ${CV_CORE} ${CV_GUI} ${CV_IMGCODECS} ${CV_IMGPROC} ${CV_VDOIO} ${ZLIB})
    else()
        list(APPEND ${COMMON_LIBS} ${CV_CORE})
        list(APPEND ${COMMON_LIBS} ${CV_GUI})
		list(APPEND ${COMMON_LIBS} ${CV_IMGCODECS})
		list(APPEND ${COMMON_LIBS} ${CV_IMGPROC})
		list(APPEND ${COMMON_LIBS} ${CV_VDOIO})
        list(APPEND ${COMMON_LIBS} ${ZLIB})
    endif()
    add_definitions(-DWITH_OPENCV)
	
	set(OPENCV_DLL_DIR ${CMAKE_SOURCE_DIR}/external/OpenCV/dll/)
	
	set(CV_DLLS ${OPENCV_DLL_DIR}/opencv_core453.dll ${OPENCV_DLL_DIR}/opencv_imgcodecs453.dll 
	${OPENCV_DLL_DIR}/opencv_imgproc453.dll ${OPENCV_DLL_DIR}/opencv_videoio453.dll ${OPENCV_DLL_DIR}/opencv_highgui453.dll)
	
	# if you want to use your own libraries
#    include(FindOpenCV)
#    if(OpenCV2_FOUND)
#        list(APPEND COMMON_LIBS ${OpenCV2_LIBRARIES})
#        include_directories(${OpenCV2_INCLUDE_DIRS})
#        add_definitions(-DWITH_OPENCV)
#    else()
#        message(ERROR " Could not find OpenCV")
#    endif()
else()
    message(ERROR " Unknown platform.")
endif()
