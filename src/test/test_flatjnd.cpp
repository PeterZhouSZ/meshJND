#include <iostream>
using namespace std;

#include "mesh.h"
#include "flatjnd.h"

int main()
{
  Mesh mesh;
  mesh.read("../data/bimba.obj");
  mesh.update_vertex_normals();

  std::vector<Vector3d> dir;
  dir.reserve(mesh.vertices_size());
  for(Mesh::Vertex_iterator it = mesh.vertices_begin(); it != mesh.vertices_end(); ++it)
    dir.push_back(mesh.normal(*it));

  ScreenParam screen(1920, 1080, 55.);
  UserParam   user(50.);
  SceneParam  scene(1080, M_PI*0.3333);

  SarkisonCSF csf(SarkisonCSF::ParameterType(-13.59, 0.01, 0.62));
  DalyMasking masking(DalyMasking::ParameterType(0.006, 90.66, 1.05, 4.53));

  CamType cam(0., 0., 1.);

  FlatJND jnd;
  jnd.set_mesh(&mesh);
  jnd.init();

  jnd.set_local_lightsource(64);

  jnd.set_screen(screen);
  jnd.set_user(user);
  jnd.set_scene(scene);

  jnd.set_CSF(csf);
  jnd.set_Masking(masking);

  VectorXd threshold;
  jnd.compute_displacement_threshold(cam, dir, threshold);

  mesh.store_as_vertex_color(threshold, threshold.maxCoeff());
  mesh.write("flatjnd.off");

  return 0;
}
