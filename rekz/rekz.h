
#ifndef ROCKZ_INCLUDE
#define ROCKZ_INCLUDE


#include <vector>
#include <iostream>

#include <string>
#include <chrono>
#include <cmath>

#include "rokz/rokz.h"
#include "rokz/defaults.h"
//#include "rokz/rokz_funcs.h"

namespace rekz {

  template<typename ElTy> 
  struct RGBx {
    ElTy r;
    ElTy g;
    ElTy b;
  };

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  template<typename Ty> struct PolarCoord {

    PolarCoord () : theta (0.0f), phi (0.0f) {
    }
    
    PolarCoord (Ty th, Ty ph) : theta (th), phi (ph) {
    }

    Ty theta; // "longitude" 0-360
    Ty phi;   // "latitude"  -90 - +90
  }; 

  typedef PolarCoord<float>  Polarf;

  typedef PolarCoord<double> Polard;
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
  //
  // --------------------------------------------------------------------
  template<typename VTy> 
  using TriMesh = rekz::GeometryData<VTy, uint16_t>; 

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  struct Vertex_pos_nrm_txc_col {
    glm::vec3 pos; 
    glm::vec3 nrm; 
    glm::vec3 col; 
    glm::vec2 txc0; 
  };

  struct Vertex_pos_col {

    glm::vec3 pos; 
    glm::vec3 col; 

  };

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  struct MouseState {

    MouseState () : inside (0), left_butt(0), right_butt(0), 
                    middle_butt(0), x_pos (0), y_pos(0) {
    }

    int inside; 
    int left_butt;   
    int right_butt;  
    int middle_butt; 
    
    int x_pos;
    int y_pos;
  }; 

  
  // --------------------------------------------------------------------
  // UserPointer data
  // --------------------------------------------------------------------
  struct InputState { 

    InputState () : keys (), mouse (), fb_resize (false) {
    }
    
    std::map<int, int> keys;
    MouseState         mouse;
    bool               fb_resize; 
  };


  // ---------------------------------------------------------------------------------------
  //                   
  // ---------------------------------------------------------------------------------------
  struct GridData {
    // some shit like this
    rokz::Buffer          vb_device;
    rokz::Buffer          ib_device;
    rokz::DescriptorGroup descrgroup;
    
  };
  
  bool SetupGridData   (GridData& gd, const rokz::Device& device);
  void CleanupGridData (GridData& gd, const rokz::Device& device);
  
  rokz::DrawSequence::Ref CreateDrawGrid (const GridData& dat);

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


  // ---------------------------------------------------------------------
  // image handling - should we keep
  // ---------------------------------------------------------------------
  struct DevILImageProps {

    int width          ;//= ilGetInteger (IL_IMAGE_WIDTH); 
    int height         ;//= ilGetInteger (IL_IMAGE_HEIGHT);
    int depth          ;//= ilGetInteger (IL_IMAGE_DEPTH);
    int bytes_per_pixel; //= ilGetInteger (IL_IMAGE_BYTES_PER_PIXEL); 
    int bpp            ;//= ilGetInteger (IL_IMAGE_BPP);
    int type           ;//= ilGetInteger (IL_IMAGE_TYPE);
    int format         ;//= ilGetInteger (IL_IMAGE_FORMAT); 
  };



  typedef int (*DevILOpenFileCB) (const unsigned char* dat, const DevILImageProps& props, void* up); 

  int OpenImageFile (const std::string& fqname, DevILOpenFileCB cb, void* up);



  // struct ImageCB {

  //   virtual int do_shit (const unsigned char* dat, const DevILImageProps&) = 0; 

  // protected:
  //   ImageCB () { }
  // } ;

  
  // int OpenImageFile (const std::string& fqname, ImageCB*);

  // ---------------------------------------------------------------------
  // load texture to device memory
  // ---------------------------------------------------------------------
  bool LoadTexture_color_sampling (rokz::Image&             image,
                                   VkFormat                 format,
                                   const VkExtent2D&        ext2d,
                                   const void*              srcimage,
                                   const VmaAllocator&      allocator, 
                                   const VkQueue&           queue, 
                                   const rokz::CommandPool& commandpool, 
                                   const rokz::Device&      device);
  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  uint32_t SizeOfComponents (VkFormat format) ;
  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  uint32_t NumberOfComponents (VkFormat format); 
  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  bool CreateDepthBufferTarget (rokz::Image&          depth_image,
                               rokz::ImageView&      depth_imageview,
                               //rokz::SwapchainGroup& scg,
                               VkSampleCountFlagBits msaa_samples, 
                               VkFormat              depth_format,
                               const rokz::CommandPool& command_pool,
                               const VkQueue&        queue, 
                               const VkExtent2D&     ext,
                               const VmaAllocator&   allocator,
                               const rokz::Device&   device); 

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  bool CreateMSAAColorTarget (rokz::Image&          color_image, 
                              rokz::ImageView&      color_imageview, 
                              VkSampleCountFlagBits msaa_samples,
                              VkFormat              image_format,
                              const VmaAllocator&   allocator, 
                              const rokz::CommandPool& command_pool, 
                              const VkQueue&        queue, 
                              const VkExtent2D&     ext,
                              const rokz::Device&   device); 
  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  namespace win_event { 
    void on_resize (GLFWwindow* window, int width, int height); 
    void on_keypress (GLFWwindow* window, int key, int scancode, int action, int mods); 
    void on_mouse_enter (GLFWwindow* window, int entered);
    void on_mouse_move (GLFWwindow* window, double xpos, double ypos); 
    void on_mouse_button (GLFWwindow* window, int button, int action, int mods); 
  }

  bool InitializeSwapchain (rokz::SwapchainGroup& scg,
                            const rokz::SwapchainSupportInfo& swapchain_support_info,
                            const VkSurfaceKHR& surface,
                            const VkExtent2D&   extent, 
                            const rokz::PhysicalDevice& physdev,
                            const rokz::Device& device) ; 

  
  //
  // actually Initize,Instance,SurfaceAndDevice ()
  // bool InitializeInstance (rokz::Instance& instance); 
  // bool InitializeDevice (rokz::Device& device, const rokz::PhysicalDevice& physical_device, const rokz::Instance& instance); 
  // bool InitializeDevice (rokz::Instance& instance, rokz::Device& device, rokz::Window& window,  VkSurfaceKHR& surface, rokz::PhysicalDevice& physical_device); 

  struct UniformSet
    {
  virtual void Update () = 0; // stuff like update buffer contents
  virtual void Bind   () = 0; // does vkUpdateDescriptorSets
    }; 

  
}



#endif
