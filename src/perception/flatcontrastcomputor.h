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

  virtual double compute(const LightType &ldir, int id);
  virtual void   compute(const LightType& ldir, Eigen::VectorXd& out);
}

#endif //FLATCONTRASTCOMPUTOR_H
