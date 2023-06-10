
#ifndef DARKROOT_GLOB_INCLUDE
#define DARKROOT_GLOB_INCLUDE


#include "rekz.h"
#include "polyobdata.h"
#include "onscreen_data.h"
#include "onscreen_draw.h"
#include "drawpolygon.h"

#include "rokz/display.hpp"
#include "rokz/input_state.hpp"
#include <vulkan/vulkan.h>


namespace darkroot {

  using namespace rokz;
  using namespace rekz;
  
  // ---------------------------------------------------------------------------------------
  struct Glob {

    enum { MaxFramesInFlight = 2
    }; 

    Glob();
    // input 
    rokz::InputState             input_state;
    glm::ivec2                   mouse_prev; 
    int                          prev_inside;
    // system
    Instance                     instance;
    Device                       device;
    rc::SwapchainGroup           swapchain_group;
    SwapchainInfo                swapchain_info;
    FrameSyncGroup               framesyncgroup;
    // DYNAMIC RENDERING, no use renderpass
    rokz::RenderingInfoGroup     rendering_info_group;
    // struct Display
    rokz::Display                display; //
    // device props - framebuffer properties
    //AttachmentProps { 
    VkFormat                     depth_format;  
    //VkSampleCountFlagBits        msaa_samples;
    VkFormat                     color_format;

    //}  
    // attachement set
    rc::Image::Ref         depthimage;          
    rc::ImageView::Ref     depthimageview;      
  
    rc::Image::Ref         msaacolorimage;      
    rc::ImageView::Ref     msaacolorimageview;  
  
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

  // * unify vb/ib
  // texture per obj
  //

  
  // 
} // darkroot

#endif
