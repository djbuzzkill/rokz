

#ifndef LUMEN_PIPE_INCLUDE
#define LUMEN_PIPE_INCLUDE


#include "rekz.h"
#include <vulkan/vulkan_core.h>

namespace lumen {

  // ----------------------------------------------------------------------------------------
  //
  // ----------------------------------------------------------------------------------------
  using namespace rokz; 
    
  const uint32 kMaxCount = 128;
  extern const Vec<VkDescriptorSetLayoutBinding>       kDescriptorBindings;

  const uint32 UBO_BINDINGI = 0; 
  const uint32 POSITION_ATTACHMENT_BINDINGI = 1; 
  const uint32 NORMAL_ATTACHMENT_BINDINGI   = 2; 
  const uint32 ALBEDO_ATTACHMENT_BINDINGI   = 3; 

// ----------------------------------------------------------------------------------------
  // Geom buff 
  // ----------------------------------------------------------------------------------------
  namespace gbuff {
    //
    typedef rokz::PNTx_Vert Vert;

    const VkShaderStageFlags kPC_stages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    extern const VkVertexInputBindingDescription& kVertexInputBindingDesc; //  = kPNTx_InputBindingDesc;
    extern const Vec<VkVertexInputAttributeDescription>& kVertexInputAttributeDesc; //  = kPNTx_InputAttributeDesc;

    struct PushConstant {
      glm::vec4 position;
      glm::vec4 scale;
      uint32 resource_id;
      uint32 _unused3_id;
      uint32 _unused4_id;
      uint32 _unused5_id;
    };
    static_assert (sizeof(PushConstant) <= 128, "exceeding max push constant"); 
  }

  // ----------------------------------------------------------------------------------------
  // Light Composition
  // ----------------------------------------------------------------------------------------
  namespace lcomp {

    typedef rokz::PTx_Vert Vert;
    const VkShaderStageFlags kPC_stages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    extern const VkVertexInputBindingDescription& kVertexInputBindingDesc; // = kPTx_InputBindingDesc; 
    extern const Vec<VkVertexInputAttributeDescription>& kVertexInputAttributeDesc; //  = kPTx_InputAttributeDesc;

    struct PushConstant {
      glm::vec4 position;
      glm::vec4 scale;
      uint32 resource_id;
      uint32 _unused3_id;
      uint32 _unused4_id;
      uint32 _unused5_id;
    };
    static_assert (sizeof(PushConstant) <= 128, "exceeding max push constant"); 

  }
  
  // ----------------------------------------------------------------------------------------
  //
  // ----------------------------------------------------------------------------------------
  bool InitPipeline (Pipeline&                    pipeline,
                     PipelineLayout&              plo,
                     VkRenderPass                 renderpass, 
                     const Vec<VkDescriptorSetLayout>&  dslos,
                     //VkSampleCountFlagBits        msaa_samples,
                     VkFormat                     color_format,
                     VkFormat                     depth_format,
                     const std::filesystem::path& pipe_path,
                     const VkExtent2D&            displayextent,
                     const Device&                device);
  
  // ----------------------------------------------------------------------------------------
  // set 0= Global  descriptors ,  set 1= landscape descriptors
  // ----------------------------------------------------------------------------------------
  bool BindDescriptorResources (Vec<VkDescriptorSet>&           dss,
                                // const Vec<rc::Buffer::Ref>&     globalubs, 

                                // const rc::Sampler::Ref&         colorsamp,
                                // const Vec<rc::ImageView::Ref>&  colorviews,

                                // const rc::Sampler::Ref&         heightsamp, 
                                // const Vec<rc::ImageView::Ref>&  heightviews,

                                // const rc::Sampler::Ref&         normalsamp, 
                                // const Vec<rc::ImageView::Ref>&  normalviews,

                                const DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                const Device&              device);


}


  
#endif
