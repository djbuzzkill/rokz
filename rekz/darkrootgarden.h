


#ifndef DARKROOT_GARDEN_INCLUDE
#define DARKROOT_GARDEN_INCLUDE

#include "rokz/common.h"

namespace darkroot {

  // --------------------------------------------------------------------
  // Geometry
  // --------------------------------------------------------------------
  template<typename VTy, typename IndTy>
    struct GeometryData {

      typedef  VTy         VertexType; 
      typedef  IndTy       IndexType; 

      enum { VertexSize = sizeof(VTy) }; 
      enum { IndexSize  = sizeof(IndTy) }; 
  
      std::vector<VTy> verts;
      std::vector<IndTy> indices;
    }; 


  // --------------------------------------------------------------------
  template<typename VTy> 
    using TriMesh = GeometryData<VTy, uint16_t>; 


  // --------------------------------------------------------------------
  // struct Renderable 
  // --------------------------------------------------------------------
  struct Renderable {

    virtual auto SetupRS (VkCommandBuffer commandbuffer) -> int = 0;
    virtual auto Draw    (VkCommandBuffer commandbuffer) -> int = 0;

    virtual auto AllocRes (VmaAllocator& alloc) -> int = 0; 
    virtual auto FreeRes  (VmaAllocator& alloc) -> int = 0; 

  protected:
    Renderable () = default; 
  
  }; 

  // --------------------------------------------------------------------
  // struct Renderable 
  // --------------------------------------------------------------------
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
    typedef std::vector<Edge> Edges;
    typedef std::vector<Face> Faces;

    // --------------------------------------------------------------------
    struct BRep {
      // geometry.verts[i] per edgei[i];
      std::vector<Index> edgei;
      // edge store
      Edges edges;
      // face store
      Faces faces;
    }; 

  } // HalfEdge

}
#endif
