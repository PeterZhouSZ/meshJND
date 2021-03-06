PROJECT(MeshJND)

################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.2.0)

################################################################################
## CONFIG                                                                     ##
################################################################################

## PROJECT #####################################################################
SET(COMPILE_TESTS true)
SET(SHARED_LIBRARY false)
SET(ENABLE_MULTITHREADING true)

## C++ #########################################################################

if(WIN32)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
else()
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if(WIN32)
  ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
endif()

if(ENABLE_MULTITHREADING)
  find_package(OpenMP REQUIRED)
  if (OPENMP_FOUND)
    ADD_DEFINITIONS(-D_USE_OPENMP_)

    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
  endif()
endif()

################################################################################
## DIRECTORIES                                                                ##
################################################################################

INCLUDE_DIRECTORIES(extern/)

INCLUDE_DIRECTORIES(src/)
INCLUDE_DIRECTORIES(src/geometry)
INCLUDE_DIRECTORIES(src/models)
INCLUDE_DIRECTORIES(src/perception)
INCLUDE_DIRECTORIES(src/utils)

################################################################################
## FILES                                                                      ##
################################################################################

## SURFACE MESH ################################################################

SET(SURFACE_MESH_SRC_FILES
    extern/surface_mesh/IO.cpp
    extern/surface_mesh/IO_off.cpp
    extern/surface_mesh/IO_poly.cpp
    extern/surface_mesh/IO_obj.cpp
    extern/surface_mesh/IO_stl.cpp
    extern/surface_mesh/Surface_mesh.cpp
    extern/surface_mesh/tinyply.cpp
    )

SET(SURFACE_MESH_HEADER_FILES
    extern/surface_mesh/IO.h
    extern/surface_mesh/properties.h
    extern/surface_mesh/Surface_mesh.h
    extern/surface_mesh/tinyply.h
    extern/surface_mesh/types.h
    extern/surface_mesh/types.h
    extern/surface_mesh/Vector.h
    )

## MESHJND #####################################################################

SET(MESHJND_SRC_FILES
    src/geometry/mesh.cpp

    src/perception/flatcontrastcomputor.cpp
    src/perception/flatfrequencycomputor.cpp
    src/perception/basejnd.cpp
    src/perception/flatjnd.cpp

    src/utils/colormap.cpp
    src/utils/lightsampler.cpp
    src/utils/math_utils.cpp
    )

SET(MESHJND_HEADER_FILES
    src/geometry/mesh.h

    src/models/contrastsensitivity.h
    src/models/contrastmasking.h
    src/models/psychometricfunction.h
    src/models/threshold.h
    src/models/visibility.h

    src/perception/basecontrastcomputor.h
    src/perception/basefrequencycomputor.h
    src/perception/basejnd.h
    src/perception/flatjnd.h
    src/perception/flatcontrastcomputor.h
    src/perception/flatfrequencycomputor.h
    src/perception/types.h

    src/utils/colormap.h
    src/utils/genericparametricmodel.h
    src/utils/lightsampler.h
    src/utils/math_utils.h
    src/utils/unitconverter.h
    )

## ALL FILES ###################################################################

SET(FILES
    ${SURFACE_MESH_SRC_FILES} ${SURFACE_MESH_HEADER_FILES}
    ${MESHJND_SRC_FILES}      ${MESHJND_HEADER_FILES}
   )

################################################################################
## LIBRARIES                                                                  ##
################################################################################

if(SHARED_LIBRARY)
    add_library(jnd SHARED ${FILES})
else()
    add_library(jnd STATIC ${FILES})
endif()

################################################################################
## EXECUTABLES                                                                ##
################################################################################

## TESTS #######################################################################
if(COMPILE_TESTS)
    #add_executable(test_models src/test/test_models.cpp)
    #target_link_libraries(test_models jnd)

    add_executable(test_attributes src/test/test_attributes.cpp)
    target_link_libraries(test_attributes jnd)

    add_executable(test_sampler src/test/test_sampler.cpp)
    target_link_libraries(test_sampler jnd)

    add_executable(test_flatjnd src/test/test_flatjnd.cpp)
    target_link_libraries(test_flatjnd jnd)
endif()
