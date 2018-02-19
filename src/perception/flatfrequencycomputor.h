#ifndef FLATFREQUENCYCOMPUTOR_H
#define FLATFREQUENCYCOMPUTOR_H

#include "basefrequencycomputor.h"

class FlatFrequencyComputor
    : public BaseFrequencyComputor
{
public:
  FlatFrequencyComputor()
    : BaseFrequencyComputor()
  {}

  FlatFrequencyComputor(Mesh* mesh)
    : BaseFrequencyComputor(mesh)
  {}

  FlatFrequencyComputor(Mesh *mesh,
                        const ScreenParam& screen,
                        const UserParam& user,
                        const SceneParam& scene)
    : BaseFrequencyComputor(mesh, screen, user, scene)
  {}

  virtual ~FlatFrequencyComputor() {}

  double compute(const Eigen::Vector3d& p1,
                 const Eigen::Vector3d& p2,
                 const CamType& cam) const;

  virtual double compute(const LightType &ldir, const CamType& cam, int id) const;
  virtual void   compute(const LightType &ldir, const CamType& cam, Eigen::VectorXd& out) const;
};

#endif // FLATFREQUENCYCOMPUTOR_H
