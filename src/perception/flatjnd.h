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

    FacePair(int i=-1, bool bfs = false)
      : id(i), both_face_change(bfs)
    {}

    // attributes --------------------------------------------------------------
    int id;
    bool both_face_change;
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
                                    const LightType &ldir,
                                    const CamType& cam,
                                    const Vector3d &displacement) const;

// additional functions ---------------------------------------------------------

protected:
  double contrast(const FacePair& fp, const LightType& l, const Vector3d& d) const;
  double frequency(const FacePair& fp, const CamType& c, const Vector3d& d) const;
  bool is_ambigous(const FacePair& fp, const Vector3d& d) const;

// attributes -------------------------------------------------------------------
protected:
  NWHWD16_Threshold m_threshold;
  VisibilityModel   m_visibility;

  FlatContrastComputor  m_cc;
  FlatFrequencyComputor m_fc;

  std::vector< FacePairs > m_fp;
};

#endif //FLATJND_H
