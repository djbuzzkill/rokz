

#include "onscreen_pipe.h"
#include <vulkan/vulkan_core.h>



using namespace rokz;


const VkVertexInputBindingDescription&        rekz::onscreen::kVertexInputBindingDesc   = rokz::kPTx_InputBindingDesc;
const Vec<VkVertexInputAttributeDescription>& rekz::onscreen::kVertexInputAttributeDesc = rokz::kPTx_InputAttributeDesc; 

const DescriptorSetLayoutBindings rekz::onscreen::kDescriptorBindings = {
  { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER        , rekz::onscreen::kMaxCount, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // array of structs per obj
  { 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, rekz::onscreen::kMaxCount, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // array of textures per obj
};
