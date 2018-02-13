#ifndef VISUALMASKING_H
#define VISUALMASkING_H

#include <cmath>

#include "genericparametricmodel.h"

//-----------------------------------------------------------------------------------

class DalyMasking
  : public ModelXd
{
  virtual void compute(const InputType& in, OutputType& out)
  {
    out = pow(1. + pow(m_param(0) * pow (m_param(1)*in, m_param(2)), m_param(3)), 1./m_param(3));
  }
}

#endif //VISUALMASKING_H
