PROJECT(MeshJND)

################################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.2.0)

################################################################################
## CONFIG                                                                     ##
################################################################################

if(WIN32)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
else()
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if(WIN32)
  ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
endif()

################################################################################
## DIRECTORIES                                                                ##
################################################################################

INCLUDE_DIRECTORIES(extern/)

INCLUDE_DIRECTORIES(src/)
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
    )

SET(MESHJND_HEADER_FILES
    src/geometry/mesh.h
    src/utils/genericparametricmodel.h
    )

## ALL FILES ###################################################################

SET(FILES
    ${SURFACE_MESH_SRC_FILES} ${SURFACE_MESH_HEADER_FILES}
    ${MESHJND_SRC_FILES}      ${MESHJND_HEADER_FILES}
   )

################################################################################
## LIBRARIES                                                                  ##
################################################################################

add_library(jnd ${FILES})

################################################################################
## EXECUTABLES                                                                ##
################################################################################
