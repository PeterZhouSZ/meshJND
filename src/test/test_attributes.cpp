#include <iostream>
using namespace std;

#include "mesh.h"
#include "flatfrequencycomputor.h"
#include "flatcontrastcomputor.h"

int main()
{
  LightType light(0., 0., 1.); light.normalize();
  CamType   cam(0., 1.42, 0.5);

  ScreenParam screen(1920, 1080, 58.);
  UserParam   user(50.);
  SceneParam  scene(1080, M_PI/3.);

  Eigen::VectorXd contrast, frequency;

//###################################################################################

  Mesh mesh;

  std::cout << "loading mesh .... " << std::flush;

  mesh.read("../data/OldMan.obj");
  mesh.update_face_normals();

  std::cout << "done." << std::endl;

//###################################################################################

  FlatContrastComputor fcc;
  fcc.set_mesh(&mesh);

  std::cout << "computing contrast for light = [" << light.transpose() << "] .... " << std::flush;

  fcc.compute(light, contrast);

  std::cout << "done." << std::endl;

//###################################################################################

  FlatFrequencyComputor ffc;
  ffc.set_screenParam(screen);
  ffc.set_userParam(user);
  ffc.set_sceneParam(scene);
  ffc.set_mesh(&mesh);

  std::cout << "computing frequency ... " << std::flush;

  ffc.compute(light, cam, frequency);

  std::cout << "done." << std::endl;

//###################################################################################

  //aggregate contast on vertices then output with color map
  Eigen::VectorXd vcontrast, vfrequency;

  vcontrast.resize(mesh.vertices_size()) ; vcontrast.setZero();
  vfrequency.resize(mesh.vertices_size()); vfrequency.setZero();

  for(unsigned int i=0; i<mesh.vertices_size(); ++i){
    Mesh::Vertex v(i);
    Mesh::Halfedge h0 = mesh.halfedge(v);
    Mesh::Halfedge h  = mesh.next_halfedge(mesh.opposite_halfedge(h0));

    double c = 0.;
    double f = 0.;
    double w = 0.;
    while(h != h0){
      double wi = mesh.edge_length(mesh.edge(h));

      c += wi*contrast(mesh.edge(h).idx());
      f += wi*frequency(mesh.edge(h).idx());

      w += wi;

      h = mesh.next_halfedge(mesh.opposite_halfedge(h));
    }

    vcontrast(i)  = c/w;
    vfrequency(i) = f/w;

  }

  mesh.store_as_vertex_color(vcontrast, vcontrast.maxCoeff()*0.001);
  mesh.write("contrast.off");

  mesh.store_as_vertex_color(vfrequency, 15., 0.);
  mesh.write("frequency.off");

  return 0;
}
