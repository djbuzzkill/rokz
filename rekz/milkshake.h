
#ifndef REKZ_MILKSHAKE_INCLUDE
#define REKZ_MILKSHAKE_INCLUDE

//
#include "rekz.h"
#include "rokz/input_state.hpp"
#include <vulkan/vulkan_core.h>

//
namespace rekz { namespace nacho {

    using namespace rokz;

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


    //
    // may need a new syncgroup type
    struct MilkSync {
      VkSemaphore sem_image_available;  // signaled when avaiable
      VkSemaphore sem_gbuffers;         // sig'd when geom finished
      VkSemaphore sem_lightpass;        // sig'd when final draw
      VkFence     fence_in_flight;      // flag bit set when present que done
    } ;

    struct PerFrame {
      VkCommandBuffer command_buffer;  
      MilkSync        sync;
    }; 
      
    // ---------------------------------------------------------------------------------------
    struct Glob : public Basically {

      enum { kMaxFramesInFlight = 2
      }; 

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


      std::array<PerFrame, kMaxFramesInFlight>          perframe; 

      //FrameSyncGroup         framesyncgroup;

      //DescriptorSetLayout    landscape_dslo;       // global r 'shared global' descr's
      // descriptors sets
      //DescriptorGroup        landscape_de; //
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
    }; 
    // ---------------------------------------------------------------------------------------
    
  }}



#endif
