#include "math_utils.h"

Vector3d
compute_farthest_point(const Polygone &poly, const Vector3d &pt)
{
  vector<double>d; d.resize(poly.size(), 0.);
  for(unsigned int i=0; i<poly.size(); ++i)
    d[i] = poly[i].dot(pt);

  unsigned int id = std::distance(d.begin(), std::min_element(d.begin(), d.end()));

  Vector3d b(Vector3d::Zero());
  b(id) = 1.;

  return b;
}

#define EPSILON 1.e-8

Vector3d
compute_nearest_point(const Polygone &poly, const Vector3d &pt)
{
  Vector3d b(Vector3d::Zero());

  bool flag = false;

  Vector3d A = poly[2]-poly[0];
  Vector3d B = poly[2]-poly[1];
  Vector3d C = pt - poly[2];

  double AA = A.dot(A);
  double BB = B.dot(B);
  double AB = A.dot(B);
  double CA = C.dot(A);
  double CB = C.dot(B);

  if(AA==BB){

    A = poly[1]-poly[2];
    B = poly[1]-poly[0];
    C = pt - poly[1];

    AA = A.dot(A);
    BB = B.dot(B);
    AB = A.dot(B);
    CA = C.dot(A);
    CB = C.dot(B);

    flag = true;
  }

  double alpha = 0.;
  double beta  = 0.;

  if(AA < 1.e-8)
    beta = std::max(std::min(-CB/BB, 1.), 0.);
  else if (BB < 1.e-8)
    alpha = std::max(std::min(-CA/AA, 1.), 0.);
  else{
    alpha = (CB*AB - CA*BB)/(BB*AA - AB*AB);
    beta  = -(CB+AB*alpha)/BB;

    if(alpha < 1.e-8){
      alpha = 0.;
      beta  = std::max(std::min(- CB/BB, 1.), 0.);
    }
    else if (beta < 1.e-8){
      alpha = std::max(std::min(- CA/AA, 1.), 0.);
      beta  = 0;
    }
    else if (alpha+beta > 1.){
      alpha = std::max(std::min(-(AB+CA-BB-CB)/(AA+BB-2.*AB), 1.), 0.);
      beta = 1.-alpha;
    }
  }

  if(!flag){
    b(0) = alpha;
    b(1) = beta;
    b(2) = 1. - b(0) - b(1);
  }
  else{
    b(2) = alpha;
    b(0) = beta;
    b(1) = 1. - b(0) - b(2);
  }

  return b;
}
