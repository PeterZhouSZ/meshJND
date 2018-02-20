#include <iostream>
using namespace std;

#include "mesh.h"
#include "flatjnd.h"

int main()
{
  Mesh mesh;
  mesh.read("../data/mask.obj");
  mesh.update_face_normals();
  mesh.update_vertex_normals();
  mesh.compute_bounding_box();

  std::vector<Vector3d> dir;
  dir.reserve(mesh.vertices_size());
  for(Mesh::Vertex_iterator it = mesh.vertices_begin(); it != mesh.vertices_end(); ++it)
    dir.push_back(mesh.normal(*it));

  ScreenParam screen(1920, 1080, 55.);
  UserParam   user(50.);
  SceneParam  scene(1080, M_PI*0.3333);

  SarkisonCSF csf(SarkisonCSF::ParameterType(-15.13, 0.0096, 0.64));
  DalyMasking masking(DalyMasking::ParameterType(0.0078, 88.29, 1.0, 4.207));

  CamType cam = mesh.bbox().center() + Eigen::Vector3d(0., 0., 1.)*mesh.bbox().diagonal().norm();

  FlatJND jnd;
  jnd.set_mesh(&mesh);
  jnd.init();

  jnd.set_local_lightsource(256);

  jnd.set_screen(screen);
  jnd.set_user(user);
  jnd.set_scene(scene);

  jnd.set_CSF(csf);
  jnd.set_Masking(masking);

  jnd.set_algorithm_parameters(0.80, 0.1, 1.e-8);

  VectorXd threshold;
  jnd.compute_displacement_threshold(cam, dir, threshold);

  mesh.store_as_vertex_color(threshold,
                             threshold.maxCoeff());
  mesh.write("flatjnd.off");

  std::cout << "max threshold : " << threshold.maxCoeff() << std::endl;
  std::cout << "min threshold : " << threshold.minCoeff() << std::endl;

  return 0;
}
