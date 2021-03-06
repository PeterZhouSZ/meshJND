#include "basejnd.h"

#include "mesh.h"
#include "lightsampler.h"

void
BaseJND::
set_global_lightsource(const Vector3d &ldir)
{
  MatrixX3d l;
  l.resize(1, 3); l.setZero();
  l.row(0) = ldir;

  set_global_lightsource(l);
}

void
BaseJND::
set_global_lightsource(const MatrixX3d &ldir)
{
  if(!m_mesh)
    return;

  if(m_verbose_level > 1)
    std::cout << "[BaseJND] setting up global light sources ... " << std::flush;

  m_light.clear();
  m_light.reserve(m_mesh->vertices_size());

  for(int i=0; i<m_mesh->vertices_size(); ++i)
    m_light.push_back(ldir);

  if(m_verbose_level > 1)
    std::cout << "done." << std::endl;
}

void
BaseJND::
set_local_lightsource(int nSamples)
{
  if(!m_mesh)
    return;

  if(m_verbose_level > 1)
    std::cout << "[BaseJND] setting up local light samples ... " << std::flush;

  LightSampler lsampler;

  m_light.clear();
  m_light.reserve(m_mesh->vertices_size());
  for(Mesh::Vertex_iterator it=m_mesh->vertices_begin(); it!=m_mesh->vertices_end(); ++it){

    double mean_angle = 0;
    int k = 0;

    Mesh::Halfedge h0 = m_mesh->halfedge(*it);
    Mesh::Halfedge h  = h0;
    do{
      Mesh::Face f = m_mesh->face(h);
      if(f.is_valid()){
        mean_angle += acos(m_mesh->normal(*it).dot(m_mesh->normal(f)));
        ++k;
      }
      h = m_mesh->next_halfedge(m_mesh->opposite_halfedge(h));
    }while(h!= h0);

    mean_angle = mean_angle/double(k);
    mean_angle = 0.5*M_PI - mean_angle;

    MatrixX3d samples;
    lsampler.sample_to_global(samples, nSamples, m_mesh->normal(*it), 0.80*mean_angle, 0.85*mean_angle);
    m_light.push_back(samples);
  }

  if(m_verbose_level > 1)
    std::cout << "done." << std::endl;
}

double
BaseJND::
compute_displacement_threshold(int id,
                               const LightType& ldir,
                               const CamType& cam,
                               const Vector3d& dir)
{
  //inveral boundaries
  double a = 0.;
  double b = 0.1*m_mesh->bbox().diagonal().norm();

  //initialize threshold to upper boundary
  double T = b;

  //initialize visibility
  double v = compute_visibility(id, ldir, cam, T*dir);

  while( fabs(v-m_tolerence) > m_precision ){
    //get interval middle
    T = a + 0.5*(b-a);

    //update visibility
    v = compute_visibility(id, ldir, cam, T*dir);

    //update interval
    if(v > m_tolerence)
      b = T;
    else
      a = T;

    //check interval width
    if((b-a) < m_interval_width)
      break;
  }

  return T;
}

double
BaseJND::
compute_displacement_threshold(int id, const CamType& cam, const Vector3d& dir)
{
  VectorXd T;
  T.resize(m_light[id].rows()); T.setOnes();

  for(unsigned int i=0; i<m_light[id].rows(); ++i){
    if(m_light[id].row(i).dot(m_mesh->normal(Mesh::Vertex(i))) > 0.)
      //no need to take into considertation light comming from the back
      T(i) = compute_displacement_threshold(id, m_light[id].row(i), cam, dir);
  }

  return T.minCoeff();
}

void
BaseJND::
compute_displacement_threshold(const CamType& cam, const std::vector<Vector3d> &dir, VectorXd &out)
{
  if(!m_mesh){ //make sure that there is a mesh
    std::cerr << "[BaseJND] Error : no mesh !!" << std::endl;
    return;
  }

  if(dir.size() != m_mesh->vertices_size()){
    std::cerr << "[BaseJND] Error : direction vector is not compatible !!" << std::endl;
    return;
  }

  if(m_need_init) //init if necessary
    init();

  if(m_verbose_level > 1)
    std::cout << "[BaseJND] computing displacement threshold ... " << std::flush;

  out.resize(m_mesh->vertices_size());
  out.setZero();

#if _USE_OPENMP_
  unsigned int i = 0;
#pragma omp parallel for
  for(i=0; i<m_mesh->vertices_size(); ++i)
    out(i) = compute_displacement_threshold(i, cam, dir[i]);
#else
  int previous_percent = 0.;
  for(unsigned int i=0; i<m_mesh->vertices_size(); ++i){

    if(m_verbose_level > 1){
      int percent = int(100. * double(i)/double(m_mesh->vertices_size()));
      if(percent - previous_percent > 9){
        std::cout <<percent << "% ... " << std::flush;
        previous_percent = percent;
      }
    }

    out(i) = compute_displacement_threshold(i, cam, dir[i]);
  }
#endif

  if(m_verbose_level > 1)
    std::cout << "done." << std::endl;
}
