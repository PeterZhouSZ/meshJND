#ifndef MESH_UTILS_H
#define MESH_UTILS_H

#include "mesh.h" 
#include "flatjnd.h"

void add_jnd_noise(Mesh* in, 
                   const std::vector< Vector3d >& dir, 
                   const CamType& cam, 
                   FlatJND& jnd,
                   Mesh& out, 
                   double beta = 0.8);

#endif //MESH_UTILS_H
