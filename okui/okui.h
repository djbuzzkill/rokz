//
#ifndef OKUI_INCLUDE
#define OKUI_INCLUDE

#include "rokz/rokz.hpp"
#include <vulkan/vulkan.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

namespace okui {

  using namespace rokz;
  using rokz::Display; 

  const VkExtent2D kDefaultDimensions { 1024, 768 }; 
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

    // DYNAMIC RENDERING, u shal not renderpass
    RenderingInfoGroup     rendering_info_group;
    // struct Display
    Display                display; //
    //AttachmentProps { 
    VkSampleCountFlagBits  msaa_samples;
    VkFormat               swapchain_format; 
    //
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

    // ---------------------------------------------------------------------------------------
  struct Glob : public Basically {

    Glob () : Basically {} {
    }

    rokz::Device device; 
    

    
  };

  
  int run (const Vec<std::string>& args); 

  // imgui_demo
  int imgui_test (int, char**); 

}



#endif
