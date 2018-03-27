#include "mesh_utils.h"

#include <random>

void
add_jnd_noise(Mesh* in, 
              const std::vector< Vector3d >& dir, 
              const CamType& cam, 
              FlatJND& jnd,
              Mesh& out, 
              double beta)
{
  std::default_random_engine generator;
  std::bernoulli_distribution distribution(0.5);

  std::vector<Vector3d> oppDir(dir);
  for(auto& i : oppDir)
    i *= -1.;

  VectorXd threshold, oppThreshold;
  
  jnd.set_mesh(in);
  jnd.init();

  jnd.compute_displacement_threshold(cam, dir, threshold);
  jnd.compute_displacement_threshold(cam, oppDir, oppThreshold);

  out = (*in);
  for(unsigned int i=0; i<out.vertices_size(); ++i)
    out.position(Mesh::Vertex(i)) = distribution(generator) ? 
                                    out.position(Mesh::Vertex(i)) + beta*threshold(i)*dir[i] :
                                    out.position(Mesh::Vertex(i)) + beta*oppThreshold(i)*oppDir[i];
  
}
              

