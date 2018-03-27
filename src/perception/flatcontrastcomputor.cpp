#include "flatcontrastcomputor.h"

#include "mesh.h"

double
FlatContrastComputor::
compute(const Eigen::Vector3d &n1,
        const Eigen::Vector3d &n2,
        const Eigen::Vector3d &ldir) const
{
  double cos_phi   = fabs(n1.dot(n2));
  double gcontrast = sqrt((1. - cos_phi)/(1. + cos_phi));

  Eigen::Vector3d ns = n1 + n2;
  Eigen::Vector3d nd = n1 - n2;

  double theta = std::max(ns.dot(ldir), 0.);
  double alpha = fabs(nd.dot(ldir));

  double lcontrast = (theta == 0 || std::isnan(alpha)) ? 0. : alpha / theta;

  return lcontrast * gcontrast;
}

double
FlatContrastComputor::
compute(const LightType &ldir, int id) const
{
  BaseMesh::Edge e(id);

  BaseMesh::Face f1 = m_mesh->face(m_mesh->halfedge(e, 0));
  BaseMesh::Face f2 = m_mesh->face(m_mesh->halfedge(e, 1));

  Eigen::Vector3d n1 = m_mesh->normal(f1);
  Eigen::Vector3d n2 = m_mesh->normal(f2);

  return compute(n1, n2, ldir);
}

void
FlatContrastComputor::
compute(const LightType &ldir, Eigen::VectorXd &out) const
{
  out.resize(m_mesh->edges_size());
  out.setZero();

  for(unsigned int i=0; i<m_mesh->edges_size(); ++i)
    out(i) = compute(ldir, i);
}
