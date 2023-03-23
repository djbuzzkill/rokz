
#ifndef MARZ_INCLUDE
#define MARZ_INCLUDE

#include "rekz.h"

#include "rokz/display.h"
#include "rokz/input_state.h"
#include "rokz/global_descriptor.h"
#include "rokz/rc_types.h"

#include "marzdata.h"

namespace marz {

  using namespace rokz;
  
  enum { kMaxFramesInFlight = 2 }; 

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  struct Glob {
    // input 
    InputState             input_state;
    glm::ivec2             mouse_prev; 
    int                    prev_inside;
    // system
    Instance               instance;
    Device                 device;
    SwapchainSupportInfo   swapchain_support_info;
    FrameSyncGroup         framesyncgroup;
    // DYNAMIC RENDERING, no use renderpass
    RenderingInfoGroup     rendering_info_group;
    // struct Display
    rokz::Display          display;             //
    // 
    VkFormat               depth_format;        //
    VkSampleCountFlagBits  msaa_samples;        // = VK_SAMPLE_COUNT_1_BIT;
    // attachement set
    rc::SwapchainGroup     swapchain_group;
    rc::Image::Ref         depth_image;          //
    rc::ImageView::Ref     depth_imageview;      //
    rc::Image::Ref         msaa_color_image;     //  
    rc::ImageView::Ref     msaa_color_imageview; //

    SwapchainResetter::Ref swapchain_resetter; // swchresetter
    //Vec<Buffer>            landscape_bu; //
    DescriptorSetLayout    global_dslo;          // global r 'shared global' descr's
    DescriptorSetLayout    landscape_dslo;       // global r 'shared global' descr's
    // uniformbundle
    Vec<rc::Buffer::Ref>   global_uniform_bu;    // vma_shared_uniforms;
    // descriptors sets
    DescriptorGroup              landscape_de; //
    DescriptorGroup              global_uniform_de;

    // DrawSequence stuff
    DrawSequence::Globals                                       shared;               
    std::array<DrawSequence::DescriptorMap, kMaxFramesInFlight> descriptormaps;
    DrawSequence::DescriptorLayoutMap                           dslomap;
    // 
    // GRID
    struct Grid { 
      PipelineLayout    plo;
      Pipeline          pipe;
      DrawSequence::Ref draw;
      rc::Buffer::Ref   buff;
    } grid; 
    // 
    // LANDSCAPE
    struct Landscape { 
      PipelineLayout    plo;
      Pipeline          pipe;
      DrawSequence::Ref draw;
      marz::Data        data;
    } scape; 

  }; 


  //Glob::Landscape;

}

#endif
