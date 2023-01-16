
#ifndef DARKROOT_GLOB_INCLUDE
#define DARKROOT_GLOB_INCLUDE

#include "rokz/rokz.h"

#define DARKROOT_DYNAMIC_RENDER_ENABLE 1

namespace darkroot {

  struct PushConstants {

    glm::ivec4 drawIDs; 
    // x: object index
    // y: unused
    // z: unused
    // w: unused

  }; 

  //    3.14159265f;
  struct PipelineGroup { 

    rokz::Pipeline        pipeline;
    rokz::DescriptorGroup descrgroup;

    
  };
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

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  typedef TriMesh<DarkrootVert> DarkrootMesh;

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  struct DarkRenderable : public Renderable {

    DarkRenderable () : num_verts (0), num_inds (0), vb_dev() , ib_dev () {
    }

    DarkRenderable (uint32_t v, uint32_t i) : num_verts (v), num_inds (i), vb_dev() , ib_dev () {
    }

    virtual auto SetupRS (VkCommandBuffer commandbuffer) -> int;
    virtual auto Draw (VkCommandBuffer commandbuffer) -> void;
    virtual auto AllocRes (VmaAllocator& allocator) -> int;
    virtual auto FreeRes (VmaAllocator& alloc) -> int;

    uint32_t num_verts;
    uint32_t num_inds;
  
    rokz::Buffer vb_dev;
    rokz::Buffer ib_dev;

    glm::vec3 pos;
    glm::quat qrot;
  
    friend DarkRenderable& Friendly ( DarkRenderable& dr); 

  protected:
  };

#ifdef DARKROOT_DYNAMIC_RENDER_ENABLE
  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  struct RenderingInfoGroup {

    VkRenderingInfo ri;
  
    VkRenderingInfo                        rendering_info;
    std::vector<VkRenderingAttachmentInfo> color_attachment_infos;
    VkRenderingAttachmentInfo              depth_attachment_info;

    std::vector<VkClearValue>              clear_colors;  // = {{0.0f, 0.0f, 0.0f, 1.0f}};
    VkClearValue                           clear_depth; //  = {{

    VkRect2D                               render_area; 
  };
#endif
  

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  struct Glob {

    //#ifdef GLOB_COMMENT_OUT   
    enum { MaxFramesInFlight = 2 }; 
    Glob();
  
    VkFormat                         depth_format;
    VkSurfaceFormatKHR               surface_format;

    rokz::Instance                   instance;
    rokz::PhysicalDevice             physical_device;
    rokz::Device                     device;
  
    struct { VkQueue graphics; VkQueue present; } queues;

    VmaAllocator                     allocator;

    rokz::SwapchainGroup         swapchain_group;
    rokz::FrameSequencing        frame_sequence;
    rokz::SwapchainSupportInfo   swapchain_support_info;

    // global pools
    rokz::CommandPool            command_pool;
    rokz::DescriptorPool         descr_pool;

    // 
    PipelineGroup                obj_pipeline;
    PipelineGroup                grid_pipeline;

    // DYNAMIC RENDERING
#ifdef  DARKROOT_DYNAMIC_RENDER_ENABLE
    RenderingInfoGroup           rendering_info_group;
#endif
    
    rokz::Image                  depth_image;
    rokz::ImageView              depth_imageview; 

    rokz::Image                  msaa_color_image;
    rokz::ImageView              msaa_color_imageview; 
  
    VkSampleCountFlagBits        msaa_samples; //  = VK_SAMPLE_COUNT_1_BIT;

    rokz::Buffer                 vma_ib_device;
    rokz::Buffer                 vma_vb_device;



    // image/texture
    rokz::Image                 texture_image; 
    rokz::ImageView             texture_imageview; 
    rokz::Sampler               sampler;

    std::vector<rokz::Buffer>   vma_uniform_buffs;
    //  std::vector<void*>          uniform_mapped_pointers; 

    std::vector<rokz::Buffer>   vma_objparam_buffs;
    //  std::vector<void*>          uniform_param_pointers; 

  
    rokz::Window                window;
    VkSurfaceKHR                surface; // 
    bool                        fb_resize; 

    //VkViewport                  viewport;
    //VkRect2D                    scissor_rect; 

    float                       queue_priority;
    double                      sim_time; 
    float                       dt;
  
    // DarkrootMesh                darkmesh;
    // DarkRenderable             darkobj;
    // HalfEdge::BRep             darkboundary;
    // std::vector<Renderable*>   renderables;

  };
  // --------------------------------------------------------------------
  
} // darkroot

#endif
