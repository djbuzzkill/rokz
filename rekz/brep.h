
#ifndef REKZ_BREP_INCLUDE
#define REKZ_BREP_INCLUDE

#include "rokz/rokz.h"



namespace rekz {

  // ---------------------------------------------------------------------------------------
  //                   
  // ---------------------------------------------------------------------------------------
  using namespace rokz;
  //


  // ---------------------------------------------------------------------------------------
  //                   
  // ---------------------------------------------------------------------------------------
  namespace HalfEdge {

    constexpr int32_t NullIndex = -1; 
    typedef   int32_t Index; 

    // --------------------------------------------------------------------
    struct Face {
      Index edgei;
    }; 
        
    // --------------------------------------------------------------------
    struct Edge {
      Index verti; 
      Index pairi; 
      Index facei; 
      Index nexti; 
    }; 
      
    // --------------------------------------------------------------------
    typedef Vec<Edge> Edges;
    typedef Vec<Face> Faces;

    // --------------------------------------------------------------------
    struct BRep {
      // edgei[i] -> geometry.verts[i] 
      Vec<Index> edgei;
      // edge store
      Edges edges;
      // face store
      Faces faces;
    }; 

  } // HalfEdge

  //
  // brep is associated with a geom
  template<typename Ty>
  inline HalfEdge::BRep& BuildBoundaryRep (HalfEdge::BRep& brep, const TriMesh<Ty>& geom) {

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

#endif
