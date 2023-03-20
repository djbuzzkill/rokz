
#include "rekz.h"


using namespace rekz;

namespace { 

  const platonic::Vert OctohedronVerts[] = {
    // top 
    {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.2, 0.3, 1.0 }, {0.5, 0.5}}, 

    {{ 0.0, 0.0,  1.0 }, {-0.707106, 0.0,  0.707106}, {0.2, 0.3, 0.8}, {0.0, 1.0}}, 
    {{ 1.0, 0.0,  0.0 }, { 0.707106, 0.0,  0.707106}, {0.2, 0.3, 0.8}, {1.0, 1.0}}, 
    {{ 0.0, 0.0, -1.0 }, { 0.707106, 0.0, -0.707106}, {0.2, 0.3, 0.8}, {1.0, 0.0}}, 
    {{-1.0, 0.0,  0.0 }, {-0.707106, 0.0, -0.707106}, {0.2, 0.3, 0.8}, {0.0, 0.0}}, 

    // bottom
    {{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, {0.2, 0.3, 1.0 }, {0.5, 0.5 }}, 
    
  }; 

  const size_t kOctohedronVertexCount = sizeof(OctohedronVerts) / sizeof (platonic::Vert);
  static_assert (kOctohedronVertexCount == 6, "");

  // CW
  const uint16_t OctohedronIndices[] = {
    0, 2, 1,  0, 3, 2,  0, 4, 3,  0, 1, 4,
    5, 1, 2,  5, 2, 3,  5, 3, 4,  5, 4, 1
  }; 

  const std::array<uint16_t, 24> OctohedronIndices16[] = {
    0, 2, 1,  0, 3, 2,  0, 4, 3,  0, 1, 4,
    5, 1, 2,  5, 2, 3,  5, 3, 4,  5, 4, 1
  }; 

// CCW 
  // const uint16_t OctohedronIndices[] = {
  //   0, 1, 2,  0, 2, 3,  0, 3, 4,  0, 4, 1,
  //   5, 2, 1,  5, 3, 2,  5, 4, 3,  5, 1, 4
  // }; 

  const size_t kOctohedronIndicesCount = sizeof(OctohedronIndices) / sizeof (uint16_t); 
  static_assert (kOctohedronIndicesCount == 24, "");


  struct OctoMesh : public platonic::Mesh {

    OctoMesh () {
      verts.assign  (OctohedronVerts, OctohedronVerts + kOctohedronVertexCount); 
      indices.assign(OctohedronIndices, OctohedronIndices + kOctohedronIndicesCount);
    }
  }; 


  const OctoMesh kOctoMesh;
}

// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------
const rekz::platonic::Mesh& rekz::platonic::Octohedron () {
  return kOctoMesh;
}
