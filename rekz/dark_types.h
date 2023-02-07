
#ifndef DARKROOT_GLOB_INCLUDE
#define DARKROOT_GLOB_INCLUDE

#include "rokz/rokz.h"
#include "rekz.h"

namespace darkroot {

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  typedef rekz::Vertex_pos_nrm_txc_col DarkVert;

  // --------------------------------------------------------------------
  typedef rekz::TriMesh<DarkVert> DarkMesh;

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  struct PushConstants {

    glm::ivec4 drawIDs; 
    // x: object index
    // y: unused
    // z: unused
    // w: unused
  }; 
  
  // --------------------------------------------------------------------
  // UBO
  // --------------------------------------------------------------------
  struct SceneObjParam {
    glm::mat4 modelmat;
    //  glm::mat4 unused0;
  };


  template<typename Ty>  struct Trait {

    int    Fn ();
    float  Fl ();
    Ty     Ft ();
    
  };


  // ---------------------------------------------------------------------
  // 
  // ---------------------------------------------------------------------
  struct ResetSwapchainCB {

  public:
  
    virtual bool ResetSwapchain  (const rokz::Window& win, const VmaAllocator& allocator, const rokz::Device& device) = 0;

  protected:
  
    ResetSwapchainCB () {}
  };

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  struct Glob {

    rekz::InputState input_state;
    //#ifdef GLOB_COMMENT_OUT   
    enum { MaxFramesInFlight = 2 }; 
    Glob();

    // device props
    VkFormat                     depth_format;
    //VkSurfaceFormatKHR           surface_format;
    VkSampleCountFlagBits        msaa_samples; //  = VK_SAMPLE_COUNT_1_BIT;

    // system
    rokz::Instance               instance;
    rokz::PhysicalDevice         physical_device;
    rokz::Device                 device;
    rokz::SwapchainGroup         swapchain_group;
    rokz::SwapchainSupportInfo   swapchain_support_info;
    rokz::FrameSyncGroup         framesyncgroup;

    // pipeline resources
    std::vector<rokz::Buffer>    vma_uniform_buffs;
    std::vector<rokz::Buffer>    vma_objparam_buffs;
    // global descriptor pool
    rokz::DescriptorPool         descr_pool;
    // pipelines
    rekz::PipelineGroup          obj_pipeline;

    rekz::PipelineGroup          grid_pipeline;
    // DYNAMIC RENDERING
    rokz::RenderingInfoGroup     rendering_info_group;
    
    // attachement set
    rokz::Image                  depth_image;
    rokz::ImageView              depth_imageview; 
    rokz::Image                  msaa_color_image;
    rokz::ImageView              msaa_color_imageview; 
  
    // DATA
    rokz::Buffer                vma_ib_device;
    rokz::Buffer                vma_vb_device;

    // image/texture
    rokz::Image                 texture_image; 
    rokz::ImageView             texture_imageview; 
    rokz::Sampler               sampler;

    float obj_theta[2];     // scene objects 

    rekz::Polarf                view_orie;
    int                         prev_x;
    int                         prev_y; 
    int                         prev_inside;
    
    // window
    rokz::Window                window;
    VkSurfaceKHR                surface; // 
    //bool                        fb_resize; 

    //VkViewport                  viewport;
    //VkRect2D                    scissor_rect; 
 
    std::shared_ptr<darkroot::ResetSwapchainCB> swapchain_reset_cb;



    
    double                      sim_time; 
    float                       dt;

  };
  // --------------------------------------------------------------------


  
  
  
#ifdef DARkROOT_ENABLE_RENDERABLE
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
#endif
} // darkroot

#endif
