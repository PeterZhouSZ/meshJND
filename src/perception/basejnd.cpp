#include "basejnd.h"

#include "mesh.h"

void
BaseJND::
set_global_lightsource(const Vector3d &ldir)
{
  MatrixX3d l;
  l.resize(1, 3); l.setZero();
  l.row(0) = ldir;

  set_global_lightsource(l);
}

void
BaseJND::
set_global_lightsource(const MatrixX3d &ldir)
{
  if(!m_mesh)
    return;

  m_light.clear();
  m_light.reserve(m_mesh->vertices_size());

  for(int i=0; i<m_mesh->vertices_size(); ++i)
    m_light.push_back(ldir);
}

double
BaseJND::
compute_displacement_threshold(int id, const Vector3d& ldir, const Vector3d& dir)
{
  //inveral boundaries
  double a = 0;
  double b = 0.1 * m_mesh->bbox().diagonal().norm();

  //initialize threshold to upper boundary
  double T = b;

  //initialize visibility
  double v = compute_visibility(id, ldir, T*dir);
  while( fabs(v-m_tolerence) > m_precision ){
    //get interval middle
    T = a + 0.5*(b-a);

    //update visibility
    v = compute_visibility(id, ldir, T*dir);

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

double
BaseJND::
compute_displacement_threshold(int id, const Vector3d &dir)
{
  VectorXd T;
  T.resize(m_light[id].rows()); T.setOnes();

  for(unsigned int i=0; i<m_light[id].rows(); ++i)
    T(i) = compute_displacement_threshold(id, m_light[id].row(i), dir);

  return T.minCoeff();
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
    out(i) = compute_displacement_threshold(i, dir[i]);
}
