
#ifndef MARZ_INCLUDE
#define MARZ_INCLUDE

#include "rekz.h"

#include "rokz/rokz_types.h"
#include "rokz/utility.h"
#include "rokz/image.h"
#include "rokz/rokz.h"


namespace marz {


  enum { kMaxFramesInFlight = 2 }; 

  const VkExtent2D kWindowExtent { 800, 600}; 
  // ---------------------------------------------------------------------------------------
  //                   
  // ---------------------------------------------------------------------------------------
  typedef rekz::Vertex_pos_nrm_txc  kVert;

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  extern const std::vector<VkDescriptorSetLayoutBinding>      kDescriptorSetBindings;
  extern const VkVertexInputBindingDescription                kVertexInputBindingDesc;
  extern const std::vector<VkVertexInputAttributeDescription> kVertexInputBindingAttributeDesc;


  struct MarzDat {

    uint32_t      X_tile_dim;
    uint32_t      Z_tile_dim;

    uint32_t      X_tile_width;
    uint32_t      Z_tile_height;

    rokz::Buffer  vb_device;
    rokz::Buffer  ib_device;
    
    std::vector<rokz::Image>     colormaps;
    std::vector<rokz::ImageView> colorviews;

    std::vector<rokz::Image>     heightmaps;
    std::vector<rokz::ImageView> heightviews;


    std::vector<rokz::Image>     normalmaps;
    std::vector<rokz::ImageView> normalviews;
    
    rokz::Sampler nsampler;
    rokz::Sampler csampler;
    rokz::Sampler hsampler;
    
  };
  
  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  struct PatchData {
    glm::ivec4 dim; 
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
    rekz::InputState             input_state;
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
    std::vector<rokz::Buffer>    global_uniform_bu;    // vma_shared_uniforms;
    rokz::DescriptorGroup        global_uniform_de;
    std::array<rokz::DrawSequence::DescriptorMap, kMaxFramesInFlight> descriptormaps;
    rokz::DrawSequence::DescriptorLayoutMap                          dslomap;


    // 
    // GRID
    struct { 
      rokz::PipelineLayout    plo;
      rokz::Pipeline          pipe;
      rokz::DrawSequence::Ref draw;
      rekz::GridData          data;
    } grid; 

    
    // 
    // LANDSCAPE
    struct landscape { 
      rokz::PipelineLayout    plo;
      rokz::Pipeline          pipe;
      rokz::DrawSequence::Ref draw;
      MarzDat                 data;
    } scape; 

  }; 



}

#endif
