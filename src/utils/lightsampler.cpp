#include "lightsampler.h"

#include <random>
#include <cmath>

void
LightSampler::
sample(MatrixX3d &samples, int n, double phi_min, double phi_max, Method method) const
{
  switch(method){
    case NAIVE:
    default:
      sample_naive(samples, n, phi_min, phi_max);
      break;
  }
}

void
LightSampler::
sample_to_global(MatrixX3d &samples, int n, const Vector3d &up, double phi_min, double phi_max, Method method) const
{
  MatrixX3d local;
  sample(local, n, phi_min, phi_max, method);
  to_global(local, samples, up);
}

void
LightSampler::
to_global(const MatrixX3d &local, MatrixX3d &global, const Vector3d &up) const
{
  Vector3d oUp(Vector3d::UnitZ());

  double c = oUp.dot(up);
  if( c == 1. ) //nothing to do
    global = local;
  else if ( c == -1. ) //opposite direction
    global = -local;
  else{
    Vector3d v = oUp.cross(up);
    Matrix3d vx;
    vx << 0, -v(2), v(1),
          v(2), 0, -v(0),
          -v(1), v(0), 0;
    vx = -vx;

    //compute rotation matrix
    Matrix3d R = Matrix3d::Identity() + vx + vx*vx/(1+c);

    global = local*R;
  }
}

void
LightSampler::
sample_naive(MatrixX3d &samples, int n, double phi_min, double phi_max) const
{
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<> dis1(0., 2.*M_PI);
  std::uniform_real_distribution<> dis2(phi_min, phi_max);

  samples.resize(n, 3);
  samples.setZero();
  for(int i=0; i<n; ++i){
    double theta = dis1(gen);
    double phi   = dis2(gen);

    samples.row(i) << cos(theta)*sin(phi), sin(theta)*sin(phi), cos(phi);
  }
}
