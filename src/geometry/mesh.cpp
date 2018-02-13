#include "mesh.h"

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

void
Mesh::
store_as_vertex_color(const Eigen::VectorXd &data)
{
  //make sure the data vector has a suitable size
  if(data.size() != vertices_size())
    return;

  //compute colors
  ColorMap colorMap(ColorMap::BLACKBODY);
  Eigen::MatrixXd colors;
  colorMap.compute_colors(data, colors, data.minCoeff(), data.maxCoeff());

  //create and store color inside the mesh
  Vertex_property<Eigen::Vector3d> vcolor = get_vertex_property<Eigen::Vector3d>("v:color");
  if(vcolor) //make sure the property exist
    for(unsigned int i=0; i<vertices_size(); ++i){
      vcolor[BaseMesh::Vertex(i)] = colors.row(i);
    }
}
