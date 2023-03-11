
#ifndef MARZ_INCLUDE
#define MARZ_INCLUDE

#include "rekz.h"

#include "rokz/display.h"
#include "rokz/input_state.h"

#include "grid_pipeline.h"


namespace marz {

  using namespace rokz;
  
  enum { kMaxFramesInFlight = 2 }; 

  // ---------------------------------------------------------------------------------------
  //                   
  // ---------------------------------------------------------------------------------------
  typedef rekz::Vertex_pos_nrm_txc  kVert;

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  extern const Vec<VkDescriptorSetLayoutBinding>      kDescriptorSetBindings;
  extern const VkVertexInputBindingDescription        kVertexInputBindingDesc;
  extern const Vec<VkVertexInputAttributeDescription> kVertexInputBindingAttributeDesc;


  struct PatchPushConstant {
    glm::ivec4 cell;  // only x, y are used
    glm::ivec4 objIDs;// only x is used
  };

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  struct MarzDat {

    uint32_t      X_tile_dim;
    uint32_t      Z_tile_dim;

    uint32_t      X_tile_width;
    uint32_t      Z_tile_height;

    rokz::Buffer  vb_device;
    rokz::Buffer  ib_device;
    
    Vec<Image>     colormaps;
    Vec<ImageView> colorviews;

    Vec<Image>     heightmaps;
    Vec<ImageView> heightviews;

    Vec<Image>     normalmaps;
    Vec<ImageView> normalviews;
    
    Sampler        depthsampler;
    Sampler        colorsampler;
    Sampler        normalsampler;
    
  };
  
  

  void SetPatchUniforms ();
  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  bool InitMarzPipe (rokz::Pipeline&                           pipe,
                     rokz::PipelineLayout&                     plo,
                     const std::vector<VkDescriptorSetLayout>& dslos,
                     //0
                     const std::filesystem::path& fspath,
                     const VkExtent2D&            viewport_extent, //const rokz::Swapchain& swapchain,
                     VkSampleCountFlagBits        msaa_samples,
                     VkFormat                     color_format,
                     VkFormat                     depth_format,
                     const rokz::Device&          device); 

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  struct Glob {
    // input 
    rokz::InputState             input_state;
    glm::ivec2                   mouse_prev; 
    int                          prev_inside;
    // system
    rokz::Instance               instance;
    rokz::Device                 device;
    rokz::SwapchainGroup         swapchain_group;
    rokz::SwapchainSupportInfo   swapchain_support_info;
    rokz::FrameSyncGroup         framesyncgroup;
    // DYNAMIC RENDERING, no use renderpass
    rokz::RenderingInfoGroup     rendering_info_group;
    // struct Display
    rokz::Display                display;             //
    // device props
    VkFormat                     depth_format;        //
    VkSampleCountFlagBits        msaa_samples;        // = VK_SAMPLE_COUNT_1_BIT;

    //
    // attachement set
    rokz::Image                  depth_image;          //
    rokz::ImageView              depth_imageview;      //
    rokz::Image                  msaa_color_image;     //  
    rokz::ImageView              msaa_color_imageview; //

    rokz::ResetSwapchainCB::Ref  swapchain_reset_cb;

    //
    rokz::DrawSequence::Globals  shared;               // DrawSequence::shared_globals
    rokz::DescriptorSetLayout    global_dslo;          // global r 'shared global' descr's
    // UniformBundle
    Vec<rokz::Buffer>            global_uniform_bu;    // vma_shared_uniforms;
    rokz::DescriptorGroup        global_uniform_de;

    std::array<DrawSequence::DescriptorMap, kMaxFramesInFlight> descriptormaps;
    DrawSequence::DescriptorLayoutMap                          dslomap;

    // 
    // GRID
    struct { 
      PipelineLayout    plo;
      Pipeline          pipe;
      DrawSequence::Ref draw;
      rekz::GridData    data;
    } grid; 

    
    // 
    // LANDSCAPE
    struct landscape { 
      PipelineLayout    plo;
      Pipeline          pipe;
      DrawSequence::Ref draw;
      MarzDat           data;
    } scape; 

    Vec<Buffer>            poly_objects_bu; // polygons will make use of object descriptors
    DescriptorGroup        poly_objects_de; // ?!?! how r descriptors handled

    
  }; 



}

#endif
