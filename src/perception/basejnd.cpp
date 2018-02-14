#include "basejnd.h"

#include "mesh.h"

double
BaseJND::
compute_displacement_threshold(int id, const Vector3d &dir)
{
  //inveral boundaries
  double a = 0;
  double b;

  //initialize threshold to upper boundary
  double T = b;

  //initialize visibility
  double v = compute_visibility(id, T*dir);
  while( fabs(v-m_tolerence) > m_precision ){
    T = a + 0.5*(b-a);

    //update visibility
    v = compute_visibility(id, T*dir);

    //update interval
    if(v > m_tolerence)
      b = T;
    else
      a = T;

    //check interval width
    if((b-a) < m_interval_width)
      break;
  }

  return T;
}

void
BaseJND::
compute_displacement_threshold(const std::vector<Vector3d> &dir, VectorXd &out)
{
  if(!m_mesh) //make sure that there is a mesh
    return;

  if(dir.size() != m_mesh->vertices_size()) //make sure that the direction vector is of correct length
    return;

  if(m_need_init) //init if necessary
    init();

  out.resize(m_mesh->vertices_size());
  out.setZero();

  for(unsigned int i=0; i<m_mesh->vertices_size(); ++i)
    out(i) = compute_displacement_threshold(i, dir(i));
}
