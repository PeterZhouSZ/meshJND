#ifndef THRESHOLD_H
#define THRESHOLD_H

#include "genericparametricmodel.h"

#include "contrastmasking.h"
#include "contrastsensitivity.h"

//-----------------------------------------------------------------------------------

struct NWHWD16_Param
{
  SarkisonCSF csf;
  DalyMasking masking;
};

/// implements the threshold mode in [ eq. (8) ]:
/// "Just noticeable distortion profile for flat-shaded 3D mesh surfaces." IEEE transactions on visualization and computer graphics 22.11 (2016).
///
/// The model takes:
/// - a Sarkison CSF and Daly Masking models as parameters
/// - contrast and frequency values as input
/// Output:
/// - a contrast threshold

class NWHWD16_Threshold
    : public GenericParametricModel<NWHWD16_Param, Eigen::Vector2d, double>
{

//-----------------------------------------------------------------------------------

  public:
    NWHWD16_Threshold()
      : GenericParametricModel<ParameterType, InputType, OutputType>()
    {
      default_params();
    }

    NWHWD16_Threshold(const ParameterType& param)
      : GenericParametricModel<ParameterType, InputType, OutputType>(param)
    {}

    virtual ~NWHWD16_Threshold() {}

//-----------------------------------------------------------------------------------

public:
  virtual void compute(const InputType &in, OutputType& out)
  {
    double c = in(0);
    double f = in(1);

    double T = m_param.masking( c*m_param.csf(f) ) / m_param.csf(f);

    return T;
  }

//-----------------------------------------------------------------------------------

protected:
  /// initialise the default params as in
  /// "Just noticeable distortion profile for flat-shaded 3D mesh surfaces." IEEE transactions on visualization and computer graphics 22.11 (2016).
  virtual void default_params()
  {
    m_param.csf     = SarkisonCSF( ParameterType(-13.59, 0.01, 0.62) );
    m_param.masking = DalyMasking( ParameterType(0.006, 90.66, 1.05, 4.53) );
  }

};

//=================================================================================//


#endif //THRESHOLD_H
