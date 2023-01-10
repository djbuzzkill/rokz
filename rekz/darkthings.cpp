

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


  //
  // brep is associated with a geom
  HalfEdge::BRep& BuildBoundaryRep (HalfEdge::BRep& brep, const DarkrootMesh& geom) {

    auto nverts = geom.verts.size ();
    auto ninds = geom.indices.size ();

    if (ninds % 3) {
      printf ("[%s] --> TriMesh should be:0\n", __FUNCTION__); 
      return brep;
    }

    auto nfaces = ninds / 3;
  
    brep.edgei.resize (nverts, HalfEdge::NullIndex);
    brep.edges.resize (nfaces * 3);
    brep.faces.resize (nfaces);

    auto edge_count = 0; 
    auto index_count = 0;
  
    for (size_t iface = 0; iface < nfaces; ++iface) {
      //geom.verts[
      // geom.indices[index_count+0]
      // geom.indices[index_count+1]
      // geom.indices[index_count+2]
      brep.faces[iface].edgei = edge_count+0;
      brep.edges[edge_count+0].facei = iface;
      brep.edges[edge_count+0].verti = geom.indices[index_count+0];
      brep.edges[edge_count+0].nexti = edge_count + 1;
      brep.edges[edge_count+0].pairi = HalfEdge::NullIndex; //<-- we dont know yet
        
      brep.edges[edge_count+1].facei = iface;
      brep.edges[edge_count+1].verti = geom.indices[index_count+1];
      brep.edges[edge_count+1].nexti = edge_count + 2;
      brep.edges[edge_count+1].pairi = HalfEdge::NullIndex; // <-- we dont know yet
      
      brep.edges[edge_count+2].facei = iface;
      brep.edges[edge_count+2].verti = geom.indices[index_count+2];
      brep.edges[edge_count+2].nexti = edge_count + 0;
      brep.edges[edge_count+2].pairi = HalfEdge::NullIndex; // <-- we dont know yet
      // verti : vertex at the end of the half-edge 
      // pairi : oppositely oriented adjacent half-edge 
      // facei : face the half-edge borders
      // nexti : next half-edge around the face
      edge_count  += 3;
      index_count += 3; 
    }

    // find pair 
    for (size_t iedge = 0; iedge < brep.edges.size (); ++iedge) {
      // make sure this isnt paried
      if (brep.edges[iedge].pairi != HalfEdge::NullIndex) 
        continue;
      
      for (size_t iiedge = 0; iiedge < brep.edges.size (); ++iiedge) {
        // if not itself & not paired
        if (iedge == iiedge || brep.edges[iiedge].pairi != HalfEdge::NullIndex) 
          continue;
      
        if (brep.edges[iedge].verti == brep.edges[iiedge].verti) {
          // iedge.pairi = iiedge.nexti.nexti;
          brep.edges[iedge].pairi = brep.edges[ brep.edges[iiedge].nexti ].nexti;
          
          // iiedge.nexti.nexti.pairi = iedge; 
          brep.edges[ brep.edges[ brep.edges[iiedge].nexti ].nexti ].pairi = iedge; 
        }
      }
    
    }
  
    return brep;
  }

  
}


