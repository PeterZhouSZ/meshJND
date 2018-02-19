#include "flatfrequencycomputor.h"

#include "mesh.h"
#include "unitconverter.h"

double
FlatFrequencyComputor::
compute(const Eigen::Vector3d &p1,
        const Eigen::Vector3d &p2,
        const CamType& cam) const
{
  double cdist  = (0.5*(p1+p2) - cam).norm();
  double length = (p1-p2).norm();

  //convert length to cpd
  int    npx = wDistance_to_pixel(m_scene.hvport, m_scene.fov, cdist, length);
  double cpd = pixel_to_cpd(m_screen.hres, m_screen.vres, m_screen.diag, m_user.dist, npx);

  return cpd;
}

double
FlatFrequencyComputor::
compute(const LightType &ldir, const CamType& cam, int id) const
{
  //ldir is not used here

  BaseMesh::Edge e(id);

  BaseMesh::Vertex v1 = m_mesh->to_vertex(m_mesh->next_halfedge(m_mesh->halfedge(e, 0)));
  BaseMesh::Vertex v2 = m_mesh->to_vertex(m_mesh->next_halfedge(m_mesh->halfedge(e, 1)));

  Eigen::Vector3d p1 = m_mesh->position(v1);
  Eigen::Vector3d p2 = m_mesh->position(v2);

  return compute(p1, p2, cam);
}

void
FlatFrequencyComputor::
compute(const LightType &ldir, const CamType& cam, Eigen::VectorXd &out) const
{
  out.resize(m_mesh->edges_size());
  out.setZero();

  for(unsigned int i=0; i<m_mesh->edges_size(); ++i)
    out(i) = compute(ldir, cam, i);
}
