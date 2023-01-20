
#ifndef MARS_TYPES_INCLUDE
#define MARS_TYPES_INCLUDE

#include "rokz/rokz.h"


namespace mars {

  // --------------------------------------------------------------------
  // Geometry
  // --------------------------------------------------------------------
  struct PushConstants {
    glm::ivec4 objectIDs;
    // x: object index
    // y: unused
    // z: unused
    // w: unused
  };

  // --------------------------------------------------------------------
  // Geometry
  // --------------------------------------------------------------------
  struct ViewParams {

    alignas(16) glm::vec3 view_pos;
    alignas(16) glm::vec3 view_dir;
    alignas(16) glm::vec3 view_up;

    float field_of_view; 
    float aspect;
    float near_dist;
    float far_dist;
    
  };

  // --------------------------------------------------------------------
  //    3.14159265f;
  // --------------------------------------------------------------------
  struct PatchParams { // UBO
    glm::mat4 model;
  }; 

  // --------------------------------------------------------------------
  //    3.14159265f;
  // --------------------------------------------------------------------
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
  //
  // --------------------------------------------------------------------
  struct Vertex_pos_nrm_txc_col {
    glm::vec3 pos; 
    glm::vec3 nrm; 
    glm::vec3 col; 
    glm::vec2 txc0; 
  };

  typedef Vertex_pos_nrm_txc_col MarsVert; 
  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  typedef TriMesh<MarsVert> MarsMesh;

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  struct Glob {


    enum { MaxFramesInFlight = 2 }; 

    rokz::Instance               instance;
    rokz::PhysicalDevice         physical_device;
    rokz::Device                 device;
  
    struct { VkQueue graphics; 
             VkQueue present; }  queues;

    VmaAllocator                 allocator;
    rokz::SwapchainGroup         frame_group;
    rokz::FrameSequencing        frame_sequence;

    rokz::SwapchainSupportInfo   swapchain_support_info;

    rokz::CommandPool            command_pool;
    rokz::DescriptorPool         descriptor_pool;
    // rokz::DescriptorGroup        descrgroup; 
    // rokz::Pipeline               pipeline; 
    PipelineGroup                terrain_pipeline;
    PipelineGroup                grid_pipeline;

    rokz::Image                  depth_image;
    rokz::ImageView              depth_imageview; 

    rokz::Image                  multisamp_color_image;
    rokz::ImageView              multisamp_color_imageview; 
  
    VkSampleCountFlagBits        msaa_samples; //  = VK_SAMPLE_COUNT_1_BIT;

    rokz::Buffer                 vma_ib_device;
    rokz::Buffer                 vma_vb_device;

    rokz::RenderPass             render_pass; 
    // image/texture
    rokz::Image                 texture_image; 
    rokz::ImageView             texture_imageview; 
    rokz::Sampler               sampler;

    std::vector<rokz::Buffer>   vma_uniform_buffs;
    std::vector<rokz::Buffer>   vma_objparam_buffs;
  
    rokz::Window                window;
    VkSurfaceKHR                surface; // 
    bool                        fb_resize; 

    float                       queue_priority;
    double                      sim_time; 
    float                       dt;

  };
  // --------------------------------------------------------------------
  
} // mars

#endif
