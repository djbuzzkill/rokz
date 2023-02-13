
#ifndef DARKROOT_GLOB_INCLUDE
#define DARKROOT_GLOB_INCLUDE

#include "rokz/rokz.h"
#include "rekz.h"
#include <vulkan/vulkan_core.h>

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
  // ?? is a pipeline  tied to a drawsequence?  
  // --  DrawPolygons is part of obj_pipeline 
  // ---------------------------------------------------------------------
  struct DrawSequence {

    virtual int Exec (VkCommandBuffer comb, const rokz::Pipeline& pl, const VkDescriptorSet* ds) = 0;


    // ?? maybe u update is fn (pipeline) -> DrawSequence<Pipeline>:: UpdateDescriptors ()
    //
    //       -> DrawSequence<PipelineA> :: UpdateDescriptors (data)
    //       -> DrawSequence<PipelineA> :: Exec (data)
    //
    //       -> DrawSequence<PipelineB> :: UpdateDescriptors (data)
    //       -> DrawSequence<PipelineB> :: Exec (data)

    virtual int UpdateDescriptors (const rokz::DescriptorPool& descpool, const rokz::Device& device) = 0; 
    // ?? who owns the descriptor set, should it be allocated with the drawseq or the pipeline
    
  protected:

    DrawSequence () {}

  }; 


  typedef std::vector<DrawSequence> DrawSequences;


  // ---------------------------------------------------------------------
  // 
  // ---------------------------------------------------------------------
  struct ResetSwapchainCB {

  public:
  
    virtual bool ResetSwapchain  (const rokz::Window& win, const rokz::Allocator& allocator, const rokz::Device& device) = 0;

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

    // global descriptor pool
    //rokz::DescriptorPool         descr_pool;

    // pipelines
    rokz::Pipeline               pipeline_objs;
    rokz::Pipeline               pipeline_grid;

    rekz::PipelineDef            pipeline_def_obj; 
    rekz::PipelineDef            pipeline_def_grid;
    
    rokz::DescriptorGroup        descrgroup_objs;
    rokz::DescriptorGroup        descrgroup_grid;
    

    
    // DYNAMIC RENDERING
    rokz::RenderingInfoGroup     rendering_info_group;
    
    // attachement set
    rokz::Image                  depth_image;
    rokz::ImageView              depth_imageview; 
    rokz::Image                  msaa_color_image;
    rokz::ImageView              msaa_color_imageview; 
  
    // pipeline resources
    std::vector<rokz::Buffer>    vma_uniform_buffs;
    std::vector<rokz::Buffer>    vma_objparam_buffs;

    // DATA     <-- moved to DrawPolygon
    // rokz::Buffer                vma_ib_device;
    // rokz::Buffer                vma_vb_device;

    // image/texture
    // rokz::Image                 texture_image; 
    // rokz::ImageView             texture_imageview; 
    // rokz::Sampler               sampler;

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

    std::shared_ptr<darkroot::DrawSequence>     polygons; 
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
