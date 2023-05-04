
#ifndef REKZ_MILKSHAKE_INCLUDE
#define REKZ_MILKSHAKE_INCLUDE

//
#include "rekz.h"
#include "rokz/input_state.hpp"
#include <vulkan/vulkan_core.h>

//
namespace milkshake {

  using namespace rokz;

  using rokz::Display; 

  // ---------------------------------------------------------------------------------------
  enum { kMaxFramesInFlight = 2 }; 

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
    SwapchainSupportInfo   swapchain_support_info;
    // DYNAMIC RENDERING, u shal not renderpass
    RenderingInfoGroup     rendering_info_group;
    // struct Display
    Display                display; //
    //AttachmentProps { 
    VkFormat               depth_format;  
    VkSampleCountFlagBits  msaa_samples;
    VkFormat               swapchain_format; 
    //
    SwapchainResetter::Ref swapchain_resetter; // swchresetter
  };

  // ---------------------------------------------------------------------------------------
  struct rendersem  {
    //
    // may need a new syncgroup type
    VkSemaphore image_available;  // signaled when avaiable
    VkSemaphore gbuffers;         // sig'd when geom finished
    VkSemaphore lightpass;        // sig'd when final draw
  };

  struct per_frame_set {
    VkCommandBuffer commandbuf;  
    rendersem       sem;
    VkFence         inflight;      // flag bit set when present que done
  }; 

  enum AttachmentTypes {

    ATT_POSITION = 0, ATT_NORMAL, ATT_ALBEDO, // AT_SPECULAR,

    NUM_ATTACHMENT_TYPES
  }; 
  // ---------------------------------------------------------------------------------------
  struct Glob : public Basically {


    Glob () : Basically {} {
    }



    // attachement set
    rc::SwapchainGroup      swapchain_group;

    rc::Image::Ref          depthimage;          //
    rc::ImageView::Ref      depthview;      //

    Vec<rc::Image::Ref>     colortargets;     //  
    Vec<rc::ImageView::Ref> colorviews; //

    // uniformbundle
    Vec<rc::Buffer::Ref>   global_bu;    // vma_shared_uniforms;

    DescriptorGroup        grid_de;     
    DescriptorSetLayout    grid_dslo;          // global r 'shared global' descr's

    DescriptorGroup        dorito_de;     
    DescriptorSetLayout    dorito_dslo;          // global r 'shared global' descr's

    //FrameSyncGroup       framesyncgroup;
    //DescriptorSetLayout  landscape_dslo;       // global r 'shared global' descr's
    // descriptors sets
    //DescriptorGroup      landscape_de; //
    Arr<per_frame_set, kMaxFramesInFlight>  sync; 

    rekz::transform        viewer; // camera view matrix
    // DrawSequence stuff
    DrawSequence::Globals  shared;               
    //std::array<DrawSequence::DescriptorMap, kMaxFramesInFlight> descriptormaps;
    //DrawSequence::DescriptorLayoutMap  dslomap;
    // 
    // GRID
    struct Grid { 
      PipelineLayout    plo;
      Pipeline          pipe;
      DrawSequence::Ref draw;
      rc::Buffer::Ref   buff;
    } grid; 
    
    PipelineLayout    dorito_plo;
    Pipeline          dorito_pipe;

    DrawSequence::Ref draw_doritos;

  }; 
  // ---------------------------------------------------------------------------------------
  int run (const Vec<std::string>& args); 
  
  }



#endif
