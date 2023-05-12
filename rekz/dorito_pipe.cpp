
#include "dorito_pipe.h"


using namespace rokz;
// -------------------------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------------------------
const VkShaderStageFlags k_descriptor_stages =
  VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT; 

namespace dorito { 
  //
  const Vec<VkDescriptorSetLayoutBinding> kDescriptorBindings = {
    { UBO_BINDINGI,                 VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         0, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // height
    { POSITION_ATTACHMENT_BINDINGI, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // 
    { NORMAL_ATTACHMENT_BINDINGI,   VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // 
    { ALBEDO_ATTACHMENT_BINDINGI,   VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 3, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // 
  };

  // ------------------------------------------------------------------------------------
  const VkVertexInputBindingDescription&        GBuff::kVertexInputBindingDesc = kPNTx_InputBindingDesc;
  const Vec<VkVertexInputAttributeDescription>& GBuff::kVertexInputAttributeDesc = kPNTx_InputAttributeDesc;

  const VkVertexInputBindingDescription&        LComp::kVertexInputBindingDesc = kPTx_InputBindingDesc; 
  const Vec<VkVertexInputAttributeDescription>& LComp::kVertexInputAttributeDesc = kPTx_InputAttributeDesc;
}

// -------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------
bool dorito::InitPipeline (Pipeline&                    pipeline,
                           PipelineLayout&              plo,
                           Vec<VkDescriptorSetLayout>&  dslos,
                           VkSampleCountFlagBits        msaa_samples,
                           VkFormat                     color_format,
                           VkFormat                     depth_format,
                           const std::filesystem::path& pipe_path,
                           const VkExtent2D&            displayextent,
                           const Device&                device)
{
  HERE("HI");

  assert (false); 

  const std::string lcomp_vs_file = pipe_path/"dorito/dorito_comp_vert.vert";
  const std::string lcomp_fs_file = pipe_path/"dorito/dorito_comp_frag.frag";

  const std::string gbuff_vs_file = pipe_path/"dorito/dorito_gbuf_vert.vert";
  const std::string gbuff_fs_file = pipe_path/"dorito/dorito_gbuf_frag.frag";

  return true;
}
  
    // ----------------------------------------------------------------------------------------
    // set 0= Global  descriptors ,  set 1= landscape descriptors
    // ----------------------------------------------------------------------------------------
bool dorito::BindDescriptorResources (Vec<VkDescriptorSet>&      dss,
                                    const DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                    const Device&              device) {
  HERE("HI");




  
  assert (false); 
  return true; 
}





