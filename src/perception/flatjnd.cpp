#include "flatjnd.h"

#include "mesh.h"

void
FlatJND::
init()
{
  if(!m_mesh) // make sure that the mesh is assigned
    return;

  if(m_verbose_level > 1)
    std::cout << "[FlatJND] Initialing JND computor ... " << std::endl;

  if(m_verbose_level > 1)
    std::cout << "  * computing mesh attibutes ... " << std::flush;
  //make sure that the mesh has its normals and bounding box computed
  m_mesh->update_face_normals();
  m_mesh->update_vertex_normals();
  m_mesh->compute_bounding_box();

  if(m_verbose_level > 1)
    std::cout << "done." << std::endl;

  //set the mesh in the contrast computor and frequency computor
  m_cc.set_mesh(m_mesh);
  m_fc.set_mesh(m_mesh);

  if(m_verbose_level > 1)
    std::cout << "  * computing affected facepairs ... " << std::flush;

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

  if(m_verbose_level > 1)
    std::cout << "done." << std::endl;

  m_need_init = false;

  if(m_verbose_level > 1)
    std::cout << "done." << std::endl;
}

double
FlatJND::
compute_visibility(int id,
                   const LightType &ldir,
                   const CamType &cam,
                   const Vector3d &displacement) const
{
  double v = 0.;

  const FacePairs& fp = m_fp[id];
  for(unsigned int i=0; i<fp.size(); ++i){

    double iF = m_fc.compute(ldir, cam, m_mesh->edge(Mesh::Halfedge(fp[i].id)).idx()); //initial frequency
    double iC = m_cc.compute(ldir, m_mesh->edge(Mesh::Halfedge(fp[i].id)).idx())     ; //initial contrast

    //uses eq 9 to compute new normal and then evaluates contrast
    //return negative value if ambigous case (eq 12)
    double c = contrast(fp[i], ldir, displacement);
    //uses eq 10 to compute new distance and then evaluates frequency
    double f = frequency(fp[i], cam, displacement);

    //threshold model is not that accurate for very low frequencies
    double T1 = m_threshold(NWHWD16_Threshold::InputType(iC, std::max(1., iF)));
    double T2 = m_threshold(NWHWD16_Threshold::InputType(iC, std::max(1., f )));

    bool is_ambigous = c < 0.;
    c = fabs(c); //we need to positive value

    double dc = is_ambigous ? c+iC : fabs(c-iC);

    double v_fp = std::max( m_visibility(VisibilityModel::InputType(dc, T1)),
                            m_visibility(VisibilityModel::InputType(dc, T2)) );

    v = std::max(v, v_fp);

    if(v == 1.) //no need to continue
      break;
  }

  return v;
}

double
FlatJND::
contrast(const FacePair& fp, const LightType& l, const Vector3d& d) const
{
  Vector3d n1(Vector3d::Zero());
  Vector3d n2(Vector3d::Zero());

  double sign = 1.; //indicates whether there is an could have been an ambigous case (eq 12 TVCG paper)

  Mesh::Halfedge h(fp.id);

  if(fp.both_face_change){

    /* drawing of what we have
     *
     *    v3 ------- v2
     *      -       -  -
     *       -     -    -
     *        -   -      -
     *          v1 ------ v4
     *
     * h is supposed to be the v1->v2 halfedge
     * v1 is the moving vertex
     * v2, v3, v4 are fixed vertices
     *
     * n1 is the normal of v1, v2, v3
     * n2 is the normal of v2, v4, v3
     *
     * n1 and n2 are changing
    */

    Vector3d v1 = m_mesh->position(m_mesh->from_vertex(h)); // the vertex that is moving
    Vector3d v2 = m_mesh->position(m_mesh->to_vertex(h));
    Vector3d v3 = m_mesh->position(m_mesh->to_vertex(m_mesh->next_halfedge(h)));
    Vector3d v4 = m_mesh->position(m_mesh->to_vertex(m_mesh->next_halfedge(m_mesh->opposite_halfedge(h))));

    n1 = (v2-(v1+d)).cross(v3-(v1+d)).normalized();
    n2 = (v4-(v1+d)).cross(v2-(v1+d)).normalized();

    Vector3d t = (v4 - 0.5*(v1+v2)).normalized();
    sign = m_mesh->normal(m_mesh->face(h)).dot(t) * n1.dot(t) < 0. ? -1. : 1.;
  }
  else{

    /* drawing of what we have
     *
     *    v1 ------- v3
     *      -       -  -
     *       -     -    -
     *        -   -      -
     *          v2 ------ v4
     *
     * h is supposed to be the v2->v3 halfedge
     * v1 is the moving vertex
     * v2, v3, v4 are fixed vertices
     *
     * n1 is the normal of v1, v2, v3
     * n2 is the normal of v2, v4, v3
     *
     * only n1 is changing
    */

    Vector3d v1 = m_mesh->position(m_mesh->to_vertex(m_mesh->next_halfedge(h))); // the vertex that is moving    
    Vector3d v2 = m_mesh->position(m_mesh->from_vertex(h));
    Vector3d v3 = m_mesh->position(m_mesh->to_vertex(h));
    Vector3d v4 = m_mesh->position(m_mesh->to_vertex((m_mesh->next_halfedge(m_mesh->opposite_halfedge(h)))));

    n1 = (v3-v2).cross((v1+d)-v2).normalized();
    n2 = m_mesh->normal(m_mesh->face(m_mesh->opposite_halfedge(h)));

    Vector3d t = (v4 - 0.5*(v2+v3)).normalized();
    sign = m_mesh->normal(m_mesh->face(h)).dot(t) * n1.dot(t) < 0. ? -1. : 1.;
  }

  return m_cc.compute(n1, n2, l);
}

double
FlatJND::
frequency(const FacePair& fp, const CamType& c, const Vector3d& d) const
{
  Vector3d a(Vector3d::Zero());
  Vector3d b(Vector3d::Zero());

  Mesh::Halfedge h(fp.id);

  if(fp.both_face_change){

    /* drawing of what we have
     *
     *    v3(a) ---- v2
     *      -       -  -
     *       -     -    -
     *        -   -      -
     *          v1 ------ v4(b)
     *
     * h is supposed to be the v1->v2 halfedge
     * v1 is the moving vertex
     * v2, v3, v4 are fixed vertices
     *
     * we want p3->p4
    */

    a = m_mesh->position(m_mesh->to_vertex(m_mesh->next_halfedge(h)));
    b = m_mesh->position(m_mesh->to_vertex(m_mesh->next_halfedge(m_mesh->opposite_halfedge(h))));

  }
  else{

    /* drawing of what we have
     *
     *    v1(a) ---- v3
     *      -       -  -
     *       -     -    -
     *        -   -      -
     *          v2 ------ v4(b)
     *
     * h is supposed to be the v2->v3 halfedge
     * v1 is the moving vertex
     * v2, v3, v4 are fixed vertices
     *
     * we want p1->p4
    */

    a = m_mesh->position(m_mesh->from_vertex(h)) + d; //v1 is displaced by d
    b = m_mesh->position(m_mesh->to_vertex(m_mesh->next_halfedge(m_mesh->opposite_halfedge(h))));

  }


  return m_fc.compute(a, b, c);
}

