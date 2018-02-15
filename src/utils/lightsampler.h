#ifndef LIGHTSAMPLER_H
#define LIGHTSAMPLER_H

#include <cmath>

#include <Eigen/Core>

class LightSampler
{
private:
  typedef Eigen::MatrixX3d MatX3;
  typedef Eigen::Vector3d  Vec3;

public:
  enum Method { NAIVE };

public:
  LightSampler() {}
  virtual ~LightSampler() {}

public:
  void sample(MatX3& samples, int n,
              Method method=NAIVE,
              double phi_min = 0.,
              double phi_max = M_PI) const;

  void sample_to_global(MatX3& samples, int n,
                        Method method=NAIVE,
                        const Vec3& up = Vec3(0., 0., 1.),
                        double phi_min = 0.,
                        double phi_max = M_PI) const;

  void to_global(const MatX3& local,
                 MatX3& global,
                 const Vec3& up = Vec3(0., 0., 1.)) const;

protected:
  void sample_naive(MatX3& samples, int n,
                    double phi_min = 0., double phi_max = M_PI) const;
};

#endif //LIGHTSAMPLER_H
