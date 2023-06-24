//
#ifndef _PEPPER_INCLUDE_
#define _PEPPER_INCLUDE_

#include "rekz.h"
#include "polyobdata.h"
#include "onscreen_data.h"
#include "onscreen_draw.h"
#include "drawpolygon.h"
#include "rokz/rc_types.hpp"



namespace pepper {

  using namespace rokz; 

  const VkExtent2D kDefaultDimensions { 1024, 768 }; 

  struct PepperLoop; 
  // ---------------------------------------------------------------------------------------
  enum { kMaxFramesInFlight = 2 }; 

  // -- 
  enum { 
    ATTACH_POSITIONI = 0, 
    ATTACH_NORMALI,
    ATTACH_ALBEDOI, // AT_SPECULAR,
    NUM_COLOR_ATTACHMENTS, 
    ATTACH_DEPTHI = NUM_COLOR_ATTACHMENTS, 
    NUM_TOTAL_ATTACHMENTS
  }; 

    
  // -- semsphores in group
  struct nacho_sem  {
    // may need a new syncgroup type
    VkSemaphore image_available;  // signaled when avaiable
    VkSemaphore gbuffers;         // sig'd when geom finished
    VkSemaphore lightpass;        // sig'd when final draw
  };

  // -- stuff per frame --
  struct per_frame_set {
    // DEF - represent 2-pass rendering sequence geombuf, lightpass
    VkCommandBuffer commandbuf;  
    nacho_sem       sem;
    VkFence         inflight;      // flag bit set when present que done
  }; 


  using namespace rokz;
  using namespace rekz;
  
  using rokz::Display; 
  // ---------------------------------------------------------------------------------------
  struct Basically {
    // input 
    InputState             input_state;
    glm::ivec2             mouse_prev; 
    int                    prev_inside;
    // system
    Instance               instance;
    Device                 device;

    rc::SwapchainGroup     swapchain_group;
    SwapchainInfo          swapchain_info;

    rc::Attachment         depth; 

    // DYNAMIC RENDERING
    RenderingInfoGroup     rendering_info_group;
    // not yet

    VkRenderPassBeginInfo renderPassInfo {};
    // renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    // renderPassInfo.renderPass = renderPass;
    // renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];

    // PerFrame
    rc::RenderPass::Ref         renderpass;
    Vec<rc::Framebuffer::Ref>   framebuffers; 

    // struct Display
    Display                display; //
    //AttachmentProps { 
    //VkSampleCountFlagBits  msaa_samples;
    //VkFormat               swapchain_format; 

    //VkFormat                     depth_format;  
    //VkFormat                     color_format;

    //
    SwapchainResetter::Ref swapchain_resetter; // swchresetter
  };

  // ---------------------------------------------------------------------------------------
  struct Glob : public Basically {

    FrameSyncGroup               framesyncgroup;
    // DYNAMIC RENDERING, no use renderpass
    rokz::RenderingInfoGroup     rendering_info_group;
    // struct Display
    rokz::Display                display; //
    // device props - framebuffer properties
    //AttachmentProps { 

    //}  
    // attachement set
    rc::Attachment         depth; 
    // rc::Image::Ref         depthimage;          
    // rc::ImageView::Ref     depthimageview;      
  
    rc::Attachment         msaacolor;
    // rc::Image::Ref         msaacolorimage;      
    // rc::ImageView::Ref     msaacolorimageview;  

    DrawSequence::Globals  shared;             
    DescriptorSetLayout    grid_dslo;        
    DescriptorSetLayout    object_dslo;        
    DescriptorSetLayout    osd_dslo;        

    // UniformBundle
    Vec<rc::Buffer::Ref>   global_rc_uniform_bu;    
    //DescriptorGroup        global_uniform_de;

    // POLYGONS
    Pipeline               polys_pl;
    PipelineLayout         polys_plo;
    // ? this belongs with the polygons
    PolygonData            polyd;
    DrawSequence::Ref      drawpoly;
    Vec<rc::Buffer::Ref>   poly_objects_bu; //

    // 
    // GRID
    Pipeline               grid_pl;
    PipelineLayout         grid_plo;
    DrawSequence::Ref      drawgrid;
    rc::Buffer::Ref        gridbuff;

    // 
    Pipeline               osd_pl;
    PipelineLayout         osd_plo;
    DrawSequence::Ref      osdraw;
    onscreen::Data         osdata;

    DescriptorGroup        poly_objects_de; //
    DescriptorGroup        grid_de; 
    DescriptorGroup        osd_de; //
    

    struct DRAWSEQSTRUC {
      DrawSequence::RenderEnv  re;
      DrawSequence::Ref        draw;
    }; 
   
    Vec<DRAWSEQSTRUC>      drawtasks; 

    // 
    //std::array<rokz::DrawSequence::DescriptorMap, MaxFramesInFlight> descriptormaps;
    rokz::DrawSequence::DescriptorLayoutMap                          dslomap;

    SwapchainResetter::Ref  swapchain_resetter;
  };

  // -- -------------------------------------------------------------------
  int run (const Vec<std::string>& args); 
  void cleanup ( Glob& g); 


  bool setup_renderpasses ( Glob& g);
  bool setup_framebuffers ( Glob& g);
  
  
}







#endif
