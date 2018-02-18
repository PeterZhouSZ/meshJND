#ifndef FLATCONTRASTCOMPUTOR_H
#define FLATCONTRASTCOMPUTOR_H

#include "basecontrastcomputor.h"

class FlatContrastComputor
    : public BaseContrastComputor
{
public:
  FlatContrastComputor()
    : BaseContrastComputor()
  {}

  FlatContrastComputor(Mesh* mesh)
    : BaseContrastComputor(mesh)
  {}

  virtual ~FlatContrastComputor() {}

  double compute(const Eigen::Vector3d& n1,
                 const Eigen::Vector3d& n2,
                 const Eigen::Vector3d& ldir);

  virtual double compute(const LightType& ldir, int id);
  virtual void   compute(const LightType& ldir, Eigen::VectorXd& out);
};

#endif //FLATCONTRASTCOMPUTOR_H
