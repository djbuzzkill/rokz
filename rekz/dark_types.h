
#ifndef DARKROOT_GLOB_INCLUDE
#define DARKROOT_GLOB_INCLUDE

#include "rokz/utility.h"
#include "rokz/image.h"
#include "rokz/rokz.h"
#include "rekz.h"
#include "rokz/rokz_types.h"
#include <vulkan/vulkan_core.h>

namespace darkroot {

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  typedef rekz::Vertex_pos_nrm_txc_col DarkVert;

  // --------------------------------------------------------------------
  typedef rekz::TriMesh<DarkVert>  DarkMesh;

  // ---------------------------------------------------------------------------------------
  //                   
  // ---------------------------------------------------------------------------------------
  // struct ResetSwapchainCB {

  // public:
  
  //   virtual bool ResetSwapchain  (const rokz::Window& win, const rokz::Allocator& allocator, const rokz::Device& device) = 0;

  // protected:
  
  //   ResetSwapchainCB () {}
  // };

  // ---------------------------------------------------------------------------------------
  //                   
  // ---------------------------------------------------------------------------------------
  struct Glob {
    
    enum { MaxFramesInFlight = 2 }; 

    Glob();
    // input 
    rekz::InputState              input_state;
    int                           prev_x;
    int                           prev_y; 
    int                           prev_inside;
    // system
    rokz::Instance                instance;
    rokz::Device                  device;
    rokz::SwapchainGroup          swapchain_group;
    rokz::SwapchainSupportInfo    swapchain_support_info;
    rokz::FrameSyncGroup          framesyncgroup;
    // DYNAMIC RENDERING, no use renderpass
    rokz::RenderingInfoGroup      rendering_info_group;
    rokz::DrawSequence::Globals   shared;                  //DrawSequence::shared_globals
    // struct Display
    rokz::Display                 display;
    // device props
    VkFormat                      depth_format;
    VkSampleCountFlagBits         msaa_samples;            // = VK_SAMPLE_COUNT_1_BIT;

    // 
    rokz::Pipeline               polys_pl  ;
    rokz::PipelineLayout         polys_plo ;
    //rokz::DescriptorSetLayout    polys_dslo; <-- dslo's r separate  from pipelines 
    rekz::PolygonData                  polyd;
    rokz::DrawSequence::Ref      drawpoly;
    // 
    rokz::Pipeline               grid_pl  ;
    rokz::PipelineLayout         grid_plo ;
    // rokz::DescriptorSetLayout grid_dlso; <++ dslo's r separate  from pipelines 
    rokz::DrawSequence::Ref      drawgrid;
    rekz::GridData               gridata;

    rokz::DescriptorSetLayout    global_dslo; 
    rokz::DescriptorSetLayout    objres_dslo;

    rokz::ResetSwapchainCB::Ref  swapchain_reset_cb;


    std::vector<rokz::Buffer>    global_uniform_bu; // vma_shared_uniforms;
    rokz::DescriptorGroup        global_uniform_de;

    // pipeline resources
    std::vector<rokz::Buffer>    objres_uniform_bu;
    rokz::DescriptorGroup        objres_uniform_de;
    



    // attachement set
    rokz::Image                  depth_image;
    rokz::ImageView              depth_imageview; 
    rokz::Image                  msaa_color_image;
    rokz::ImageView              msaa_color_imageview; 
  };


  

  // 
} // darkroot

#endif
