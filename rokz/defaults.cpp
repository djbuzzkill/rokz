#include "defaults.h"



using namespace rokz;





VkPipelineVertexInputStateCreateInfo& rokz::Init (VkPipelineVertexInputStateCreateInfo& create_info,
                                                  const VkVertexInputBindingDescription& binding_desc, 
                                                  const std::array<VkVertexInputAttributeDescription, 2>& attrib_desc)
{
  // VERTEX INPUT
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  create_info.vertexBindingDescriptionCount = 1;
  create_info.pVertexBindingDescriptions = &binding_desc; // Optional
  create_info.vertexAttributeDescriptionCount = attrib_desc.size(); 
  create_info.pVertexAttributeDescriptions = &attrib_desc[0]; // Optional

  return create_info; 
}




VkInstanceCreateInfo& rokz::Default (VkInstanceCreateInfo& create_info) {
  return create_info;
}

VkDeviceCreateInfo& rokz::Default (VkDeviceCreateInfo& create_info) {
  return create_info;
}

VkDeviceQueueCreateInfo& rokz::Default (VkDeviceQueueCreateInfo& create_info) {
  return create_info;
}

VkSwapchainCreateInfoKHR& rokz::Default (VkSwapchainCreateInfoKHR& create_info) {
  return create_info;
}

VkImageViewCreateInfo& rokz::Default (VkImageViewCreateInfo& create_info) {
  return create_info;
} 

VkRenderPassCreateInfo& rokz::Default (VkRenderPassCreateInfo& create_info) {
  return create_info;
}

VkGraphicsPipelineCreateInfo& rokz::Default (VkGraphicsPipelineCreateInfo& create_info) {
  return create_info;
}

VkPipelineInputAssemblyStateCreateInfo&  rokz::Default (VkPipelineInputAssemblyStateCreateInfo& create_info) {
  return create_info;
}       //      ateInfo input_assembly{};

VkPipelineDynamicStateCreateInfo& rokz::Default (VkPipelineDynamicStateCreateInfo& create_info) {
  return create_info;
} //         dynamic_state_create_info {};

VkPipelineViewportStateCreateInfo& rokz::Default (VkPipelineViewportStateCreateInfo& create_info) {
  return create_info;
}    //    viewport_state_create_info{};

VkPipelineRasterizationStateCreateInfo& rokz::Default (VkPipelineRasterizationStateCreateInfo& create_info) {
  return create_info;
}//  rasterizer{};

VkPipelineMultisampleStateCreateInfo& rokz::Default (VkPipelineMultisampleStateCreateInfo& create_info)  {
  return create_info;
} //   multisampling{};

VkPipelineDepthStencilStateCreateInfo& rokz::Default (VkPipelineDepthStencilStateCreateInfo& create_info)  {
  return create_info;
} //    pipeline_depth_stencil_create_info {};

VkPipelineColorBlendStateCreateInfo& rokz::Default (VkPipelineColorBlendStateCreateInfo& create_info) {
  return create_info;
}   //    color_blending_create_info{};

VkPipelineLayoutCreateInfo&  rokz::Default (VkPipelineLayoutCreateInfo& create_info) {
  return create_info;
}

VkPipelineShaderStageCreateInfo& rokz::Default (VkPipelineShaderStageCreateInfo& create_info) {
  return create_info;
} //      vert_shader_stage_info {};


