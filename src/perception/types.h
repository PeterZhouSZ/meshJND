#ifndef TYPES_H
#define TYPES_H

#include <Eigen/Core>
using namespace Eigen;

//-----------------------------------------------------------------------------------

typedef Eigen::Vector3d LightType;
typedef Eigen::Vector3d CamType;

//-----------------------------------------------------------------------------------

struct ScreenParam
{
  ScreenParam(int h=0, int v=0, double d=0.)
    : hres(h), vres(v), diag(d)
  {}

  int    hres;
  int    vres;
  double diag;
};

//-----------------------------------------------------------------------------------

struct UserParam
{
  UserParam(double d=0)
    : dist(d)
  {}

  double dist;
};

//-----------------------------------------------------------------------------------

struct SceneParam
{
  SceneParam(int h=0, double f=0.)
    : hvport(h), fov(f)
  {}

  int    hvport;
  double fov;
};

//-----------------------------------------------------------------------------------

#endif //TYPES_H
