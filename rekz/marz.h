
#ifndef MARZ_INCLUDE
#define MARZ_INCLUDE

#include "rekz.h"

#include "rokz/display.h"
#include "rokz/input_state.h"
#include "rokz/global_descriptor.h"


namespace marz {

  using namespace rokz;
  
  enum { kMaxFramesInFlight = 2 }; 

  // ---------------------------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------------------------
  typedef rekz::Vertex_pos_nrm_txc  kPatchVert;

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  extern const Vec<VkDescriptorSetLayoutBinding>      kDescriptorSetBindings;
  extern const VkVertexInputBindingDescription        kVertexInputBindingDesc;
  extern const Vec<VkVertexInputAttributeDescription> kVertexInputBindingAttributeDesc;

 
  struct PatchPushConstant {
    glm::uvec4 cell;  // only x, y are used
    glm::uvec4 objIDs;// only x is used
  };

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  struct MarsDat {

    const uint32 x_tile_dim   = 1024;
    const uint32 z_tile_dim   = 1024;
    const uint32 x_tile_count = 6;
    const uint32 z_tile_count = 16;

    Buffer         vb_device;
    Buffer         ib_device;
    
    Vec<Image>     colormaps;
    Vec<ImageView> colorviews;

    Vec<Image>     heightmaps;
    Vec<ImageView> heightviews;

    Vec<Image>     normalmaps;
    Vec<ImageView> normalviews;
    
    Sampler        heightsampler;
    Sampler        colorsampler;
    Sampler        normalsampler;
    
  };


  bool SetupData   (MarsDat& dat);
  void CleanupData (MarsDat& dat, rokz::Device& device);
  
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
    SwapchainGroup         swapchain_group;
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
    Image                  depth_image;          //
    ImageView              depth_imageview;      //
    Image                  msaa_color_image;     //  
    ImageView              msaa_color_imageview; //

    ResetSwapchainCB::Ref  swapchain_reset_cb;
    //Vec<Buffer>            landscape_bu; //
    DescriptorSetLayout    global_dslo;          // global r 'shared global' descr's
    DescriptorSetLayout    landscape_dslo;       // global r 'shared global' descr's
    // uniformbundle
    Vec<Buffer>                  global_uniform_bu;    // vma_shared_uniforms;
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
      rekz::GridData    data;
    } grid; 
    // 
    // LANDSCAPE
    struct Landscape { 
      PipelineLayout    plo;
      Pipeline          pipe;
      DrawSequence::Ref draw;
      MarsDat           data;
    } scape; 

  }; 


  //Glob::Landscape;

}

#endif
