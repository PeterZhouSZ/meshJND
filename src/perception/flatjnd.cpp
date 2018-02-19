#include "flatjnd.h"

#include "mesh.h"

double
FlatJND::FacePair::
contrast(Mesh *mesh, const FlatContrastComputor &cc, const LightType &l, const Vector3d &d) const
{
  Vector3d n1(Vector3d::Zero());
  Vector3d n2(Vector3d::Zero());

  Mesh::Halfedge h(id);

  if(both_face_change){
    Vector3d v1 = mesh->position(mesh->from_vertex(h));
    Vector3d v2 = mesh->position(mesh->to_vertex(h));
    Vector3d v3 = mesh->position(mesh->to_vertex(mesh->next_halfedge(h)));
    Vector3d v5 = mesh->position(mesh->to_vertex(mesh->next_halfedge(mesh->opposite_halfedge(h))));

    n1 =
  }
  else{

  }

  return cc.compute(n1, n2, l);
}

void
FlatJND::
init()
{
  if(!m_mesh) // make sure that the mesh is assigned
    return;

  //make sure that the mesh has its normals and bounding box computed
  m_mesh->update_face_normals();
  m_mesh->update_vertex_normals();
  m_mesh->compute_bounding_box();

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

    //lambda function to make sure that is not a border edge

    FacePairs fpairs;
    do{
      if(!m_mesh->is_boundary(m_mesh->edge(h)))
        fpairs.push_back( FacePair(h.idx(), true) );

      if(!m_mesh->is_boundary(m_mesh->edge(m_mesh->next_halfedge(h))))
        fpairs.push_back(FacePair( m_mesh->next_halfedge(h).idx(), false) );

      h = m_mesh->next_halfedge(m_mesh->opposite_halfedge(h));
    }while(h != h0);

    m_fp.push_back(fpairs);
  }

  m_need_init = false;
}

double
FlatJND::
compute_visibility(int id, const LightType &ldir, const CamType &cam, const Vector3d &displacement) const
{
  double v = 0.;

  const FacePairs& fp = m_fp[id];
  for(unsigned int i=0; i<fp.size(); ++i){
    double iF = m_fc.compute(ldir, cam, m_mesh->edge(Mesh::Halfedge(fp[i].id)).idx());   //initial frequency
    double iC = m_cc.compute(ldir, m_mesh->edge(Mesh::Halfedge(fp[i].id)).idx())     ;   //initial contrast

    double c =fp[i].contrast(m_mesh, m_cc, ldir, displacement);  //contrast

    double T = 0;//m_threshold_model(NWHWD16_Threshold::InputType(iC, iF));
    double v_fp = 0;//m_visibility(VisibilityModel::InputType(c-iC, T));

    v = std::max(v, v_fp);
  }

  return v;
}
