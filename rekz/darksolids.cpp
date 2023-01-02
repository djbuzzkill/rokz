

#include "darkrootgarden.h"


namespace darkroot {
  

  const DarkrootVert OctohedronVerts[] = {
    // top 
    {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.2, 0.3, 1.0 }, {0.5, 0.5}}, 

    {{ 0.0, 0.0,  1.0 }, {-0.707106, 0.0,  0.707106}, {0.2, 0.3, 0.8}, {0.0, 1.0}}, 
    {{ 1.0, 0.0,  0.0 }, { 0.707106, 0.0,  0.707106}, {0.2, 0.3, 0.8}, {1.0, 1.0}}, 
    {{ 0.0, 0.0, -1.0 }, { 0.707106, 0.0, -0.707106}, {0.2, 0.3, 0.8}, {1.0, 0.0}}, 
    {{-1.0, 0.0,  0.0 }, {-0.707106, 0.0, -0.707106}, {0.2, 0.3, 0.8}, {0.0, 0.0}}, 

    // bottom
    {{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, {0.2, 0.3, 1.0 }, {0.5, 0.5 }}, 
    
  }; 

  const size_t kOctohedronVertexCount = sizeof(OctohedronVerts) / sizeof (DarkrootVert);
  static_assert (kOctohedronVertexCount == 6, "");

  // CW
  const uint16_t OctohedronIndices[] = {
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

  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------
  const DarkrootMesh& DarkOctohedron () {

    static DarkrootMesh octo;
    octo.verts.assign  (OctohedronVerts, OctohedronVerts + kOctohedronVertexCount); 
    octo.indices.assign(OctohedronIndices, OctohedronIndices + kOctohedronIndicesCount);

    return octo;
  }





  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  const DarkrootVert dark_verts[] = {
    {{-0.7f, -0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.7f, -0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{ 0.7f,  0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.7f,  0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
                                      
    {{ 0.0f, -1.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{ 1.0f,  0.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{ 0.0f,  1.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{ -1.0f, 0.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
  };

  const uint16_t dark_indices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
  };

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------

  const DarkrootVert dark_cube[] = {
    {{-0.7f, -0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.7f, -0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{ 0.7f,  0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.7f,  0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{ 0.0f, -1.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{ 1.0f,  0.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{ 0.0f,  1.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{ -1.0f, 0.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
  };

  const uint16_t cube_indices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
  };


  
}


