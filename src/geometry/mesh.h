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
  const Eigen::AlignedBox3d& bbox() const { return m_bbox; }
  Eigen::AlignedBox3d& bbox() { return m_bbox; }

public:
  void compute_bounding_box();

  void store_as_vertex_color(const Eigen::VectorXd& data, double max=1., double min=0.);

protected:
  Eigen::AlignedBox3d m_bbox;

};

#endif //MESH_H
