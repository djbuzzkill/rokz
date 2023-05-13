
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
    SwapchainInfo   swapchain_info;
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

    // attachement set
    rc::SwapchainGroup      swapchain_group;

    struct { 
      rc::Framebuffer::Ref framebuffer; 
      rc::RenderPass::Ref  renderpass ;
      struct {
        rc::Attachment position, normal, albedo, depth; } attachment;
    } gbuff; 

    struct {
      Vec<rc::Framebuffer::Ref> framebuffers;
      rc::RenderPass::Ref       renderpass;
      struct {
        rc::Attachment depth; } attachment; 
    } lcomp; 
    
    
    // uniformbundle
    Vec<rc::Buffer::Ref>   global_bu;        // vma_shared_uniforms;

    DescriptorGroup        grid_de;     
    DescriptorSetLayout    grid_dslo;        // global r 'shared global' descr's

    DescriptorGroup        dorito_de;     
    DescriptorSetLayout    dorito_dslo;      // global r 'shared global' descr's

    // -- is  this correct
    Arr<per_frame_set, kMaxFramesInFlight>  sync; 

    rekz::transform        viewer; // camera view matrix
    // DrawSequence stuff
    DrawSequence::Globals  shared;               
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
