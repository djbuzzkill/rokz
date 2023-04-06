
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

  struct transform {
    glm::vec3 position;
    glm::vec3 rotation_eu;
  }; 

  inline glm::mat4& ViewerMatrix (glm::mat4& outm, const glm::vec3 vpos, const glm::vec3& vrot_eu) {

    glm::mat4 xrot = glm::rotate (glm::mat4(1), vrot_eu.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 yrot = glm::rotate (glm::mat4(1), vrot_eu.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 zrot = glm::rotate (glm::mat4(1), vrot_eu.z, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotm =  zrot  * yrot  * xrot;
    // glm::mat4 viewmatrix = glm::translate (glm::mat4(1.0f), vpos) * rotation;
    // outm = glm::inverse (viewmatrix); 
    outm = glm::inverse (glm::translate (glm::mat4(1.0f), vpos) * rotm); 
    
    return outm;
  }
  
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
    DescriptorSetLayout    grid_dslo;          // global r 'shared global' descr's
    DescriptorSetLayout    landscape_dslo;       // global r 'shared global' descr's
    // uniformbundle
    Vec<rc::Buffer::Ref>   global_bu;    // vma_shared_uniforms;

    
    // descriptors sets
    DescriptorGroup        landscape_de; //
    //DescriptorGroup        global_de;
    DescriptorGroup        grid_de;

    transform              viewer;

    
    // DrawSequence stuff
    DrawSequence::Globals              shared;               
    //std::array<DrawSequence::DescriptorMap, kMaxFramesInFlight> descriptormaps;
    DrawSequence::DescriptorLayoutMap  dslomap;
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
