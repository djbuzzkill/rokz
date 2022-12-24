#include "defaults.h"



using namespace rokz;

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkPipelineVertexInputStateCreateInfo& rokz::VertexInputState ( 
    VkPipelineVertexInputStateCreateInfo&                 create_info,
    const VkVertexInputBindingDescription&                binding_desc, 
    const std::vector<VkVertexInputAttributeDescription>& attrib_desc)
{
  // VERTEX INPUT
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  create_info.pNext = nullptr;
  create_info.vertexBindingDescriptionCount = 1;
  create_info.pVertexBindingDescriptions = &binding_desc; 
  create_info.vertexAttributeDescriptionCount = attrib_desc.size(); 
  create_info.pVertexAttributeDescriptions = &attrib_desc[0]; 

  return create_info; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkViewport& rokz::Viewport (VkViewport& vp, float x, float y, float wd, float ht, float dp) {
  // VIEWPORT 
  vp = {};
  vp.x = x;
  vp.y = y;
  vp.width = wd;
  vp.height = ht;
  vp.minDepth = 0.0f;
  vp.maxDepth = dp;

  return vp ;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkRect2D& rokz::Rect2D (VkRect2D& rect, const VkOffset2D& offs, const VkExtent2D& ext) { 
  // SCISSOR RECT
  rect = {}; 
  rect.offset = offs;
  rect.extent = ext;
  return rect; 
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
VkPipelineViewportStateCreateInfo& rokz::CreateInfo (VkPipelineViewportStateCreateInfo& ci, const VkViewport& vp, const VkRect2D& scissor) { 
  // VkPipelineViewportStateCreateInfo
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  ci.pNext = nullptr;
  ci.viewportCount = 1;
  ci.pViewports = &vp; 

  ci.scissorCount = 1;
  ci.pScissors = &scissor;

  return ci; 
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
VkPipelineRasterizationStateCreateInfo & rokz::CreateInfo (VkPipelineRasterizationStateCreateInfo& ci) {
  // RASTERIZATION STATE .. VkPipelineRasterizationStateCreateInfo
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  ci.pNext = nullptr;
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
  ;

VkPipelineMultisampleStateCreateInfo& rokz::CreateInfo (VkPipelineMultisampleStateCreateInfo& ci, VkSampleCountFlagBits  msaa_samples) { 
  // MULTI SAMPLING
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  ci.pNext = nullptr;
  ci.sampleShadingEnable = VK_FALSE;
  ci.rasterizationSamples = msaa_samples;
  ci.minSampleShading = 1.0f;          
  ci.pSampleMask = nullptr;            
  ci.alphaToCoverageEnable = VK_FALSE; 
  ci.alphaToOneEnable = VK_FALSE;      

  return ci; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkPipelineDepthStencilStateCreateInfo& rokz::CreateInfo (VkPipelineDepthStencilStateCreateInfo& ci) {
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  ci.pNext = nullptr;
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
VkPipelineInputAssemblyStateCreateInfo& rokz::CreateInfo (VkPipelineInputAssemblyStateCreateInfo& ci, VkPrimitiveTopology prim) { 
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
  ci.pNext = nullptr;
  ci.stage  = stage_flags; // VK_SHADER_STAGE_VERTEX_BIT;
  ci.module = module;
  ci.pSpecializationInfo = nullptr; 
  ci.pName = "main";
  return ci; 
}

// ---------------------------------------------------------------------
// DesciptorSetLayoutBinding
// ---------------------------------------------------------------------
// VkDescriptorSetLayoutBinding& rokz::Init (VkDescriptorSetLayoutBinding& ds, uint32_t binding, VkDescriptorType desc_type, VkShaderStageFlagBits stage_flags) {
//   ds =  {};
//   ds.binding            = binding;
//   ds.descriptorType     = desc_type ; //VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//   ds.descriptorCount    = 1; 
//   ds.stageFlags         = stage_flags; // VK_SHADER_STAGE_VERTEX_BIT,  VK_SHADER_STAGE_ALL_GRAPHICS
//   ds.pImmutableSamplers = nullptr; 
//   return ds;
// }

