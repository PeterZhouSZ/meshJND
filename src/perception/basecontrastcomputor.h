#ifndef BASECONTRASTCOMPUTOR_H
#define BASECONTRASTCOMPUTOR_H

#include "types.h"

class Mesh;

class BaseContrastComputor
{
public:
  BaseContrastComputor()
    : m_mesh(0)
  {}

  BaseContrastComputor(Mesh* mesh)
    : m_mesh(mesh)
  {}

  virtual ~BaseContrastComputor()
  {
    m_mesh = 0;
  }

  void set_mesh(Mesh* mesh) { m_mesh = mesh; }

  virtual double compute(const LightType& ldir, int id) const = 0;
  virtual void   compute(const LightType& ldir, Eigen::VectorXd& out) const = 0;

protected:
  Mesh* m_mesh;
};

#endif //BASECONTRASTCOMPUTOR_H
