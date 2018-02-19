#include "flatjnd.h"

#include "mesh.h"

void
FlatJND::
init()
{
  if(!m_mesh) // make sure that the mesh is assigned
    return;

  //set the mesh in the contrast computor and frequency computor
  m_cc.set_mesh(m_mesh);
  m_fc.set_mesh(m_mesh);

  //get the affected face pairs for each vertex
  m_fp.clear();
  m_fp.reserve(m_mesh->vertices_size());

  for(Mesh::Vertex_iterator it=m_mesh->vertices_begin(); it!= m_mesh->vertices_end(); ++it){
    Mesh::Vertex v = *it;

    Mesh::Halfedge h0 = m_mesh->halfedge(v);
    Mesh::Halfedge h  = h0;

    FacePairs fpairs;
    do{
      fpairs.push_back(FacePair(m_mesh->edge(h).idx()));
      fpairs.push_back(FacePair(m_mesh->edge(m_mesh->next_halfedge(h)).idx()));

      h = m_mesh->next_halfedge(m_mesh->opposite_halfedge(h));
    }while(h != h0);

    m_fp.push_back(fpairs);
  }

  m_need_init = false;
}

double
FlatJND::
compute_visibility(int id, const Vector3d &ldir, const Vector3d &displacement)
{
  double v = 0.;

  FacePairs& fp = m_fp[id];
  for(unsigned int i=0; i<fp.size(); ++i){
    double f ;   //frequency
    double ic = m_cc.compute(ldir, fp[i].id);  //initial contrast

    double c ;  //contrast

    double T = m_threshold_model(NWHWD16_Threshold::InputType(ic, f));
    double v_fp = m_visibility(VisibilityModel::InputType(c-ic, T));

    v = std::max(v, v_fp);
  }

  return v;
}
