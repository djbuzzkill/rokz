
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
  // PolygonData polyd resources the polygon pipeline will use
  // --------------------------------------------------------------------
  struct PolygonData { 

    rokz::Buffer          vb_device;
    rokz::Buffer          ib_device;
    rokz::DescriptorGroup descrgroup;
    // image/texture
    rokz::Image           texture;   // color texture
    rokz::ImageView       imageview; // 
    rokz::Sampler         sampler;   // 

    float obj_theta[2];     // scene objects 

    rekz::Polarf                view_orie;

    // pipeline resources
    std::vector<rokz::Buffer>    vma_poly_uniforms;

  } ;



  // --------------------------------------------------------------------
  struct GridData {
    // some shit like this
    rokz::Buffer          vb_device;
    rokz::Buffer          ib_device;
    rokz::DescriptorGroup descrgroup;
    
  };
  
  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  struct pipeline_assembly  {
    rokz::Pipeline&       pipeline;
    VkPipelineLayout      plo;
    VkDescriptorSet       descrset;
  }; 


  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  struct shared_globals {

    shared_globals () : viewport_ext (), dt(0.0f), sim_time(0.0f), current_frame (0) {
    }
    
    VkExtent2D     viewport_ext;
    float          dt;
    float          sim_time;
    uint32_t       current_frame;

  }; 
  // ---------------------------------------------------------------------
  // ?? a pipeline is tied to a drawlist.. no.
  // ?? a drawlist is tied to data..       ??
  // ?? a data is tied to a drawlist..     no
  // ?? a drawlist is tied to a pipeline.. ??

  // ---------------------------------------------------------------------
  // Intent is to encapsulate a list of draw commands
  // DrawLisst renamed to DrawSequence to avoid confusion
  // ---------------------------------------------------------------------
  struct DrawSequence : public rokz::ut::destructor {

    // do crap before recording ("UpdateDescriptors()", etc)
    virtual int Prep (const shared_globals& globals, const pipeline_assembly& pa, const rokz::Device& device) = 0; 
    //
    // the draw sequence recording, mebe rename to DrawSeq::Rec() 
    virtual int Exec (VkCommandBuffer comb, const pipeline_assembly& pa, const VkDescriptorSet& ds) = 0;

  protected:

    DrawSequence () {} // dont allow abstract

  }; 

  std::shared_ptr<DrawSequence> CreatePolygonDraw (const PolygonData& d); 
  std::shared_ptr<DrawSequence> CreatePolygonWireframe (const darkroot::PolygonData& d); 
  // std::shared_ptr<DrawSequence> CreatePolygonTextured 
  // std::shared_ptr<DrawSequence> CreateGridDraw 

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

    //#ifdef GLOB_COMMENT_OUT   
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
    rokz::RenderingInfoGroup     rendering_info_group;

    // struct Display
    rokz::Window                  window;
    VkSurfaceKHR                  surface; // 

    // device props
    VkFormat                      depth_format;
    VkSampleCountFlagBits         msaa_samples; //  = VK_SAMPLE_COUNT_1_BIT;

    // 
    rokz::Pipeline                polys_pl  ;
    rokz::PipelineLayout          polys_plo ;
    rokz::DescriptorSetLayout     polys_dslo;
    PolygonData                   polyd;
    std::shared_ptr<DrawSequence> drawpoly;
    // 
    // rokz::Pipeline                grid_pl  ;
    // rokz::PipelineLayout          grid_plo ;
    // rokz::DescriptorSetLayout     grid_dslo;
    // std::shared_ptr<DrawSequence> drawgrid;
    // GridData                      gridata;

    std::shared_ptr<darkroot::ResetSwapchainCB> swapchain_reset_cb;
    std::vector<rokz::Buffer>                   vma_shared_uniforms;
    shared_globals shared;

    // attachement set
    rokz::Image                  depth_image;
    rokz::ImageView              depth_imageview; 
    rokz::Image                  msaa_color_image;
    rokz::ImageView              msaa_color_imageview; 
  };
  // 
} // darkroot

#endif
