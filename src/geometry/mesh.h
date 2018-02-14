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

public:
  void store_as_vertex_color(const Eigen::VectorXd& data, double max=1., double min=0.);

};

#endif //MESH_H
