#include "mesh.h"

#include <cmath>
#include "colormap.h"

Mesh::
Mesh()
  : BaseMesh()
{
}

Mesh::
~Mesh()
{
}

double
Mesh::dehedral_angle(Halfedge h) {
  Face f1 = face(h);
  Face f2 = face(opposite_halfedge(h));

  if(f1.is_valid() && f2.is_valid()){
    return acos(fabs(compute_face_normal(f1).dot(compute_face_normal(f2))));
  }

  return 0.;
}

void
Mesh::
compute_bounding_box()
{
  Vertex_iterator vb = vertices().begin();
  Vertex_iterator ve = vertices().end();

  m_bbox.setEmpty();
  for(Vertex_iterator it=vb; it!=ve; ++it)
    m_bbox.extend(position(*it));
}

void
Mesh::
store_as_vertex_color(const Eigen::VectorXd &data, double max, double min)
{
  //make sure the data vector has a suitable size
  if(data.size() != vertices_size())
    return;

  //compute colors
  ColorMap colorMap(ColorMap::BLACKBODY);
  Eigen::MatrixXd colors;
  colorMap.compute_colors(data, colors, min, max);

  //create and store color inside the mesh
  Vertex_property<Color> vcolor = vertex_property<Color>("v:color");
  if(vcolor) //make sure the property exist
    for(unsigned int i=0; i<vertices_size(); ++i){
      vcolor[BaseMesh::Vertex(i)] = colors.row(i);
    }
}
