
#ifndef DARKROOT_GLOB_INCLUDE
#define DARKROOT_GLOB_INCLUDE


#include "rekz.h"

#include "rokz/display.h"
#include "rokz/input_state.h"

namespace darkroot {

  using namespace rokz;
  using namespace rekz;
  
  // ---------------------------------------------------------------------------------------
  struct Glob {

    enum { MaxFramesInFlight = 2 }; 

    Glob();

    // input 
    rokz::InputState             input_state;
    glm::ivec2                   mouse_prev; 
    int                          prev_inside;
    // system
    Instance               instance;
    Device                 device;
    rc::SwapchainGroup     swapchain_group;
    SwapchainSupportInfo   swapchain_support_info;
    FrameSyncGroup         framesyncgroup;
    // DYNAMIC RENDERING, no use renderpass
    rokz::RenderingInfoGroup     rendering_info_group;
    // struct Display
    rokz::Display                display;             //


    // device props

    // framebuffer properties
    VkFormat                     depth_format;        //
    VkSampleCountFlagBits        msaa_samples;        // = VK_SAMPLE_COUNT_1_BIT;
    // attachement set

    rc::Image::Ref         depthimage      ; //Image                depth_image;          //
    rc::ImageView::Ref     depthimageview  ; // depth_imageview;      //b
    rc::Image::Ref         msaacolorimage  ;     //  msaa_color_image
    rc::ImageView::Ref     msaacolorimageview; // msaa_color_imageview

    DrawSequence::Globals  shared;               // DrawSequence::shared_globals
    DescriptorSetLayout    global_dslo;          // global r 'shared global' descr's
    DescriptorSetLayout    object_dslo;          // object r 'generic', but belong to instance

    // UniformBundle
    //Vec<Buffer>            global_uniform_bu;    // vma_shared_uniforms;
    Vec<rc::Buffer::Ref>   global_rc_uniform_bu;    // vma_shared_uniforms;
    DescriptorGroup        global_uniform_de;

    // POLYGONS
    Pipeline               polys_pl;
    PipelineLayout         polys_plo;
    // ? this belongs with the polygons
    PolygonData            polyd;
    DrawSequence::Ref      drawpoly;
    Vec<Buffer>            poly_objects_bu; // polygons will make use of object descriptors
    DescriptorGroup        poly_objects_de; // ?!?! how r descriptors handled

    // 
    // GRID
    Pipeline               grid_pl;
    PipelineLayout         grid_plo;
// rokz::DescriptorSetLayout grid_dlso; <++ dslo's r separate from pipelines 
    DrawSequence::Ref      drawgrid;
    rc::Buffer::Ref        gridbuff;

    std::array<rokz::DrawSequence::DescriptorMap, MaxFramesInFlight> descriptormaps;
    rokz::DrawSequence::DescriptorLayoutMap                          dslomap;

    SwapchainResetter::Ref  swapchain_resetter;

  };

  // * unify vb/ib
  // texture per obj
  //

  
  // 
} // darkroot

#endif
