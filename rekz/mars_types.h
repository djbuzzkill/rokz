
#ifndef MARS_TYPES_INCLUDE
#define MARS_TYPES_INCLUDE

#include "rokz/rokz.h"
#include "rekz.h"


namespace mars {


  typedef rekz::RGBx<unsigned char>    RGBu8; 
  //
  typedef rekz::Vertex_pos_nrm_txc_col MarsVert; 
  typedef rekz::TriMesh<MarsVert>      MarsMesh;
  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  const float         kPi                =  glm::pi<float> ();  
  const VkExtent2D    kWinExtent         = { 800, 600 };
  const size_t        kPatchCount        = 100;
  const size_t        kMaxFramesInFlight = 2; 
  //

  // --------------------------------------------------------------------
  // Geometry
  // --------------------------------------------------------------------
  struct PushConstants {

    glm::ivec4 objectIDs;
    glm::vec4  color4f;
  };
  
 
  // --------------------------------------------------------------------
  // UBO
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
  //
  // --------------------------------------------------------------------
  struct Glob {

    enum { MaxFramesInFlight = 2 }; 


    rokz::Instance               instance;
    rokz::PhysicalDevice         physical_device;
    rokz::Device                 device;
    rokz::SwapchainGroup         swapchain_group;
    rokz::SwapchainSupportInfo   swapchain_support_info;
    rokz::FrameSync              frame_sync;
    // pipeline resource
    std::vector<rokz::Buffer>    vma_uniform_buffs;
    std::vector<rokz::Buffer>    vma_objparam_buffs;

    VkSampleCountFlagBits        msaa_samples; //  = VK_SAMPLE_COUNT_1_BIT;
    VkFormat                     depth_format;
    VkSurfaceFormatKHR           surface_format;

    rokz::DescriptorPool         descriptor_pool; 
    // rokz::DescriptorGroup        descrgroup; 
    // rokz::Pipeline               pipeline; 
    rekz::PipelineGroup          terrain_pipeline;
    rekz::PipelineGroup          grid_pipeline;

    // DYNAMIC RENDERING - no more renderpass
    rokz::RenderingInfoGroup     rendering_info_group;

    // ATTACHEMENT SET 
    rokz::Image                  msaa_depth_image;
    rokz::ImageView              msaa_depth_imageview; 
    rokz::Image                  msaa_color_image;
    rokz::ImageView              msaa_color_imageview; 

    // DATA 
    
    rokz::Buffer                 vma_ib_device;
    rokz::Buffer                 vma_vb_device;
    // image/texture
    // rokz::Image                 texture_image; 
    // rokz::ImageView             texture_imageview; 
    // rokz::Sampler               sampler;

  
    rokz::Window                window;
    VkSurfaceKHR                surface; // 


    rekz::InputState            input_state; // <-- use with window handlers

    double                      sim_time; 
    float                       dt;


    /////////////////////////////////////////////////////////////////////////
#ifndef MARS_GOING_AWAY_SOON 
    rokz::CommandPool            command_pool; //<-- now part of Device

    bool fb_resize;                            //<-- InputState::fb_resize
    rokz::RenderPass             render_pass;  //<-- no use 

    struct { VkQueue graphics;                 //<-- now part of Device
             VkQueue present; }  queues;       
    VmaAllocator                 allocator;    //<-- in device too
#endif
    /////////////////////////////////////////////////////////////////////////

    
  };


  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  glm::vec3& unit_angle_xz (glm::vec3& v, float theta); 
  
} // mars

#endif
