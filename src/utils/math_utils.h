#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <vector>
using namespace std;

#include <Eigen/Core>
using namespace Eigen;

typedef vector<Vector3d> Polygone;

Vector3d compute_farthest_point(const Polygone& poly, const Vector3d& pt);

Vector3d compute_nearest_point(const Polygone& poly, const Vector3d& pt);

#endif
