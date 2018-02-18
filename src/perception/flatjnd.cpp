#include "flatjnd.h"

#include "mesh.h"

void
FlatJND::
init()
{

}

void
FlatJND::
compute_visibility(int id, const Vector3d &ldir, const Vector3d &displacement)
{
  double initial_contrast;
  double current_contrast;

  double T;

  Vector2d in;
  in << initial_contrast-current_contrast, T;

  return m_visibility(in);
}
