#ifndef MESH_H
#define MESH_H

#include <surface_mesh/Surface_mesh.h>
using namespace surface_mesh;
typedef Surface_mesh BaseMesh;

class Mesh
    : public BaseMesh
{

public:
  Mesh();

  virtual ~Mesh();

};

#endif //MESH_H
