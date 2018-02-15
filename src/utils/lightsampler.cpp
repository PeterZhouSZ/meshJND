#include "lightsampler.h"

#include <random>
#include <cmath>

void
LightSampler::
sample(MatX3 &samples, int n, Method method, double phi_min, double phi_max) const
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
sample_to_global(MatX3 &samples, int n, Method method, const Vec3 &up, double phi_min, double phi_max) const
{
  MatX3 local;
  sample(local, n, method, phi_min, phi_max);
  to_global(local, samples, up);
}

void
LightSampler::
to_global(const MatX3 &local, MatX3 &global, const Vec3 &up) const
{
  global = local;
}

void
LightSampler::
sample_naive(MatX3 &samples, int n, double phi_min, double phi_max) const
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
