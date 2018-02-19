#ifndef FLATJND_H
#define FLATJND_H

#include "basejnd.h"

#include "threshold.h"
#include "visibility.h"

#include "flatcontrastcomputor.h"
#include "flatfrequencycomputor.h"

class FlatJND
    : public BaseJND
{

// internal Type ----------------------------------------------------------------

protected:
  struct FacePair{

    FacePair(int i=-1)
      : id(i)
    {}

    bool is_valid() const;

    // attributes --------------------------------------------------------------

    int id;
  };

  typedef std::vector<FacePair> FacePairs;

// constructors -----------------------------------------------------------------

public:
  FlatJND()
    : BaseJND()
  {}

  FlatJND(double tolerence, double precision, double interval_width)
    : BaseJND(tolerence, precision, interval_width)
  {}

  virtual ~FlatJND()
  {}

// virtual functions ------------------------------------------------------------

public:
  virtual void init();

protected:
  virtual double compute_visibility(int id,
                                    const Vector3d &ldir,
                                    const Vector3d &displacement);

// additional functions ---------------------------------------------------------

// attributes -------------------------------------------------------------------
protected:
  NWHWD16_Threshold m_threshold_model;
  VisibilityModel   m_visibility;

  FlatContrastComputor  m_cc;
  FlatFrequencyComputor m_fc;

  std::vector< FacePairs > m_fp;
};

#endif //FLATJND_H
