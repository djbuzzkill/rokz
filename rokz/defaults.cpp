#include "defaults.h"



using namespace rokz;

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkPipelineVertexInputStateCreateInfo& rokz::Init ( 
    VkPipelineVertexInputStateCreateInfo&                 create_info,
    const VkVertexInputBindingDescription&                binding_desc, 
    const std::vector<VkVertexInputAttributeDescription>& attrib_desc)
{
  // VERTEX INPUT
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  create_info.vertexBindingDescriptionCount = 1;
  create_info.pVertexBindingDescriptions = &binding_desc; 
  create_info.vertexAttributeDescriptionCount = attrib_desc.size(); 
  create_info.pVertexAttributeDescriptions = &attrib_desc[0]; 

  return create_info; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkViewport& rokz::Init (VkViewport& vp, float wd, float ht, float dp) {
  // VIEWPORT 
  vp = {};
  vp.x = 0.0f;
  vp.y = 0.0f;
  vp.width = wd;
  vp.height = ht;
  vp.minDepth = 0.0f;
  vp.maxDepth = dp;

  return vp ;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkRect2D& rokz::Init (VkRect2D& rect, const VkOffset2D& offs, const VkExtent2D& ext) { 
  // SCISSOR RECT
  rect = {}; 
  rect.offset = offs;
  rect.extent = ext;
  return rect; 
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
VkPipelineViewportStateCreateInfo& rokz::Init (VkPipelineViewportStateCreateInfo& ci, const VkViewport& vp, const VkRect2D& scissor) { 
  // VkPipelineViewportStateCreateInfo
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

  ci.viewportCount = 1;
  ci.pViewports = &vp; 

  ci.scissorCount = 1;
  ci.pScissors = &scissor;

  return ci; 
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
VkPipelineRasterizationStateCreateInfo & rokz::Init (VkPipelineRasterizationStateCreateInfo& ci) {
  // RASTERIZATION STATE .. VkPipelineRasterizationStateCreateInfo
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  ci.depthClampEnable = VK_FALSE;
  ci.rasterizerDiscardEnable = VK_FALSE;
  ci.polygonMode = VK_POLYGON_MODE_FILL;
  ci.lineWidth = 1.0f;
  ci.cullMode = VK_CULL_MODE_BACK_BIT;
  ci.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; // VK_FRONT_FACE_CLOCKWISE;
  ci.depthBiasEnable = VK_FALSE;
  ci.depthBiasConstantFactor = 0.0f; 
  ci.depthBiasClamp = 0.0f;          
  ci.depthBiasSlopeFactor = 0.0f;    

  //ci.cullMode = VK_CULL_MODE_BACK_BIT;

  
  return ci;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------

VkPipelineMultisampleStateCreateInfo& rokz::Init (VkPipelineMultisampleStateCreateInfo& ci) { 
  // MULTI SAMPLING
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  ci.sampleShadingEnable = VK_FALSE;
  ci.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  ci.minSampleShading = 1.0f;          
  ci.pSampleMask = nullptr;            
  ci.alphaToCoverageEnable = VK_FALSE; 
  ci.alphaToOneEnable = VK_FALSE;      

  return ci; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkPipelineDepthStencilStateCreateInfo& rokz::Init (VkPipelineDepthStencilStateCreateInfo& ci) {
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  ci.depthTestEnable = VK_TRUE;
  ci.depthWriteEnable = VK_TRUE;
  ci.depthCompareOp = VK_COMPARE_OP_LESS;
  ci.depthBoundsTestEnable = VK_FALSE;
  ci.stencilTestEnable = VK_FALSE;
  return ci; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkPipelineInputAssemblyStateCreateInfo& rokz::Init (VkPipelineInputAssemblyStateCreateInfo& ci, VkPrimitiveTopology prim) { 
  // INPUT ASSEMBLY STATE
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  ci.pNext = nullptr; 
  ci.flags = 0x0;
  ci.topology = prim ; //VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  ci.primitiveRestartEnable = VK_FALSE;
  return ci; 

}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkPipelineShaderStageCreateInfo& rokz::Init (VkPipelineShaderStageCreateInfo& ci,
                                             VkShaderStageFlagBits stage_flags,
                                             const VkShaderModule& module)
{
  ci = {};   
  ci.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  ci.stage  = stage_flags; // VK_SHADER_STAGE_VERTEX_BIT;
  ci.module = module;
  ci.pSpecializationInfo = nullptr; 
  ci.pName = "main";
  return ci; 
}
                                             
                                             
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkDescriptorSetLayoutBinding& rokz::Init (VkDescriptorSetLayoutBinding& ds, uint32_t index, VkDescriptorType desc_type, VkShaderStageFlagBits stage_flags) {
  ds =  {};
  ds.binding = index;
  ds.descriptorType = desc_type ;
  ds.descriptorCount = 1;
  ds.stageFlags = stage_flags; // VK_SHADER_STAGE_VERTEX_BIT,  VK_SHADER_STAGE_ALL_GRAPHICS
  ds.pImmutableSamplers = nullptr; 
  return ds;
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


