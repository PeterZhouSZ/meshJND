#ifndef CONTRASTSENSITIVITY_H
#define CONTRASTSENSITIVITY_H

#include <cmath>

#include "genericparametricmodel.h"

//-----------------------------------------------------------------------------------

class SarkisonCSF
    : public ModelXd
{
  virtual void compute(const InputType& in, OutputType& out)
  {
    out = (1.f - m_param(0) + in/m_param(1)) * exp( -pow(in, m_param(2)) );
  }
};

//-----------------------------------------------------------------------------------

class BartenCSF
    : public ModelXXd
{
  virtual void compute(const InputType& in, OutputType& out)
  {
    double A = m_param(0) * pow( (1.+0.7) /in(1), m_param(3) );
    double B = m_param(1) * pow( (1.+100.)/in(1), m_param(4) );

    out = A*in(0) * exp(-B*in(0) * sqrt( 1 + m_param(2)*exp(B*in(0) );
  }
};

#endif //CONTRASTSENSITIVTY_H
