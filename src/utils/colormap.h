#ifndef COLORMAP_H
#define COLORMAP_H

#include <Eigen/Core>
using namespace Eigen;

class ColorMap
{
public:
  enum Type { GREY, SEASHORE, BLACKBODY, COLD_WARM };

public:
  ColorMap(Type t = GREY)
  {
    setColorMap(t);
  }

  ~ColorMap(){}

  void setColorMap(Type t);

  void compute_colors(const VectorXd& data, MatrixXd& colors, double min = 0., double max = 1.) const;

protected:
  inline int getMapSize() const { return m_map.rows(); }

  Eigen::Vector3d get_pixel_value(double value) const;

protected:
  Eigen::MatrixX3d m_map;

};

#endif // COLORMAP_H
