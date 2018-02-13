#ifndef PSYCHOMETRICFUNCTION_H
#define PSYCHOMETRICFUNCTION_H

#include <cmath>

#include "genericparametricmodel.h"

//-----------------------------------------------------------------------------------

class WeibulPsychometricFunction
  : public ModelXd
{
  virtual void compute(const InputType& in, OutputType& out)
  {
    out = 1. - exp(-pow(in, m_param(0)));
  }
};

#endif //PSYCHOMETRICFUNCTION_H
