#ifndef BASEFREQUENCYCOMPUTOR_H
#define BASEFREQUENCYCOMPUTOR_H

#include <Eigen/Core>

typedef Eigen::Vector3d LightType;
typedef Eigen::Vector3d CamType;

class Mesh;

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

struct UserParam
{
  UserParam(double d=0)
    : dist(d)
  {}

  double dist;
};

struct SceneParam
{
  SceneParam(int h=0, double f=0.)
    : hvport(h), fov(f)
  {}

  int    hvport;
  double fov;
};

//-----------------------------------------------------------------------------------

class BaseFrequencyComputor
{
public:
  BaseFrequencyComputor()
    : m_mesh(0)
  {}

  BaseFrequencyComputor(Mesh* mesh)
    : m_mesh(mesh)
  {}

  BaseFrequencyComputor(Mesh *mesh,
                        const ScreenParam& screen,
                        const UserParam& user,
                        const SceneParam& scene)
    : m_mesh(mesh), m_screen(screen), m_user(user), m_scene(scene)
  {}

  virtual ~BaseFrequencyComputor()
  {
    m_mesh = 0;
  }

  void set_mesh(Mesh* mesh) { m_mesh = mesh; }

  void set_screenParam(const ScreenParam& screen) { m_screen = screen; }
  void set_userParam  (const UserParam&   user  ) { m_user   = user  ; }
  void set_sceneParam (const SceneParam&  scene ) { m_scene  = scene ; }

  virtual double compute(const LightType& ldir, int id) = 0;
  virtual void   compute(const LightType& ldir, Eigen::VectorXd& out) = 0;

protected:
  Mesh* m_mesh;

  ScreenParam m_screen;
  UserParam   m_user  ;
  SceneParam  m_scene ;
};

#endif //BASEFREQUENCYCOMPUTOR_H
