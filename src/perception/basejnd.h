#ifndef BASEJND_H
#define BASEJND_H

#include <vector>
using namespace std;

#include <Eigen/Core>
using namespace Eigen;

class Mesh;

class BaseJND
{
public:
  BaseJND()
    : m_mesh(0), m_tolerence(0.95), m_precision(3e-2), m_interval_width(1e-3), m_need_init(true)
  {}

  BaseJND(double tolerence, double precision, double interval_width)
    : m_mesh(0), m_tolerence(tolerence), m_precision(precision), m_interval_width(interval_width), m_need_init(true)
  {}

  virtual ~BaseJND()
  {
    m_mesh = 0;
  }

public:
  void set_mesh(Mesh* mesh)
  {
    m_mesh = mesh;
    m_need_init = true;
  }

  void set_algorithm_parameters(double tolerence, double precision, double interval_width)
  {
    m_tolerence = tolerence;
    m_precision = precision;
    m_interval_width = interval_width;
  }

public:
  ///performs precomputation needed to compute the displacement threshold
  virtual void init() = 0;

  ///computes the displacement threshold of a vertex in a given direction dir
  /// implements alogorithm 1 of:
  /// "Just noticeable distortion profile for flat-shaded 3D mesh surfaces." IEEE transactions on visualization and computer graphics 22.11 (2016).
  double compute_displacement_threshold(int id, const Vector3d& ldir, const Vector3d& dir);

  ///computes the displacement threshold of all the vertices in the direction dir
  void compute_displacement_threshold(const std::vector< Vector3d >& dir,
                                      VectorXd& out);

protected:
  ///computes the visibility of the a vertex when displaced by a certain vertor
  virtual double compute_visibility(int id, const Vector3d& ldir, const Vector3d& displacement) = 0;

protected:
  Mesh* m_mesh;

  //jnd algorithm parameters
  double m_tolerence;
  double m_precision;
  double m_interval_width;

  bool m_need_init;

  //initial perceptual attributes
  VectorXd m_init_contrast;
  VectorXd m_init_frequency;
};

#endif //BASEJND_H
