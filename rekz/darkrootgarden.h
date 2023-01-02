


#ifndef DARKROOT_GARDEN_INCLUDE
#define DARKROOT_GARDEN_INCLUDE

#include "rokz/common.h"



namespace darkroot {


  const float kPi = 3.14159265f;

  
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
    virtual auto Draw    (VkCommandBuffer commandbuffer) -> void = 0;
    
    virtual auto AllocRes (VmaAllocator& alloc) -> int = 0; 
    virtual auto FreeRes  (VmaAllocator& alloc) -> int = 0; 

  protected:
    Renderable () = default; 
  
  }; 

  // --------------------------------------------------------------------
  // 
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
      // edgei[i] -> geometry.verts[i] 
      std::vector<Index> edgei;
      // edge store
      Edges edges;
      // face store
      Faces faces;
    }; 

  } // HalfEdge


  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  struct DarkrootVert {
    glm::vec3 pos; 
    glm::vec3 nrm; 
    glm::vec3 col; 
    glm::vec2 txc0; 
  };

  //
  typedef TriMesh<DarkrootVert> DarkrootMesh;

  //
  struct SceneObjParam {
    glm::mat4 modelmat;
    //  glm::mat4 unused0;
  };

  //
  const size_t SizeOf_SceneObjParam = sizeof (SceneObjParam); 


  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  const DarkrootMesh& DarkOctohedron (); 

  
}
#endif
