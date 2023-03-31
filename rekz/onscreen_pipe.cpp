

#include "onscreen_pipe.h"
#include <vulkan/vulkan_core.h>



using namespace rokz;


const VkVertexInputBindingDescription&        rekz::onscreen::kVertexInputBindingDesc   = rokz::kPTx_InputBindingDesc;
const Vec<VkVertexInputAttributeDescription>& rekz::onscreen::kVertexInputAttributeDesc = rokz::kPTx_InputAttributeDesc; 

const DescriptorSetLayoutBindings rekz::onscreen::kDescriptorBindings = {
  { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER        , rekz::onscreen::kMaxCount, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // array of structs per obj
  { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, rekz::onscreen::kMaxCount, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // array of textures per obj
};


// ----------------------------------------------------------------------------------------------
// this function is sus, it is setting up a global buffer
// ----------------------------------------------------------------------------------------------
bool rekz::onscreen::InitObjPipeline (Pipeline&                         pipeline,
                                      PipelineLayout&                   plo,
                                      const Vec<VkDescriptorSetLayout>& dslos,
                                      //0
                                      const systempath&                 fspath,
                                      const VkExtent2D&                 viewport_extent, //const rokz::Swapchain& swapchain,
                                      VkSampleCountFlagBits             msaa_samples,
                                      VkFormat                          color_format,
                                      VkFormat                          depth_format,
                                      const Device&                     device)
{

  return false;
}

// ----------------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------------
bool rekz::onscreen::SetupObjectUniforms (Vec<rc::Buffer::Ref>& uniform_buffs, uint32_t num_sets, const Device& device) {

  return false; 
}
// ----------------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------------
bool rekz::onscreen::BindObjectDescriptorResources (Vec<VkDescriptorSet>&       dss ,
                                                    const Vec<rc::Buffer::Ref>& objbuffs,
                                                    const rc::ImageView::Ref    imageviews,  
                                                    const rc::Sampler::Ref      sampler, 
                                                    const DescriptorSetLayout&  dslayout, 
                                                    const Device&               device)  {


  return false; 
}

