#include <iostream>
using namespace std;

#include "lightsampler.h"
#include "mesh.h"

int main()
{
  int n = 64;

  Eigen::Vector3d up(0., 0., 1.); up.normalize();
  Eigen::MatrixX3d light;

  LightSampler sampler;
  sampler.sample_to_global(light, n, up, 0.4*M_PI, 0.45*M_PI);

  std::cout << "sampling done" << std::endl;

  //save samples as point cloud
  Mesh mesh;
  mesh.reserve(n, 0, 0);
  for(int i=0; i<light.rows(); ++i)
    mesh.add_vertex(light.row(i));
  mesh.write("samples.ply");

  return 0;
}
