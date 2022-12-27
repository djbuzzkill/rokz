#include "pipeline.h"


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkPipelineVertexInputStateCreateInfo& rokz::CreateInfo ( 
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
VkPipelineShaderStageCreateInfo& rokz::CreateInfo (VkPipelineShaderStageCreateInfo& ci,
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
bool rokz::CreateGraphicsPipelineLayout (
    VkPipelineLayout&            pipeline_layout,
    VkPipelineLayoutCreateInfo&  create_info, 
    const VkDescriptorSetLayout& desc_set_layout, 
    const VkDevice&              device)
{
  // PIPELINE LAYOUT CREATE INFO << mostly empty for now
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  create_info.pNext = nullptr;

  create_info.setLayoutCount         = 1;            
  create_info.pSetLayouts            = &desc_set_layout;         
  create_info.setLayoutCount         = 1;            
  create_info.pSetLayouts            = &desc_set_layout;         
  //printf ("NOTE: %s [Descriptor Set Layout INACTIVE]\n", __FUNCTION__); 

  create_info.pushConstantRangeCount = 0;    
  create_info.pPushConstantRanges = nullptr; 

  if (vkCreatePipelineLayout (device, &create_info, nullptr, &pipeline_layout) != VK_SUCCESS) {
    printf("FAILED _create pipeline layout_\n");
    return false;
  }

  return true;
}
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateGraphicsPipeline (
    rokz::Pipeline&                                    pipeline,
    const VkRenderPass&                                render_pass,
    const std::vector<VkPipelineShaderStageCreateInfo>& ci_shader_stages, 
    const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
    const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
    const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
    const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
    const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
    const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
    const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
    const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state, 
    const VkDevice&                                    device)
{
  pipeline.ci.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline.ci.pNext               = nullptr;
  pipeline.ci.flags               = 0x0; 
  pipeline.ci.stageCount          = ci_shader_stages.size();
  pipeline.ci.pStages             = &ci_shader_stages[0]; 
  pipeline.ci.pVertexInputState   = ci_vertex_input_state; ;
  pipeline.ci.pInputAssemblyState = ci_input_assembly;
  pipeline.ci.pViewportState      = ci_viewport_state;
  pipeline.ci.pRasterizationState = ci_rasterizer;
  pipeline.ci.pMultisampleState   = ci_multisampling;
  pipeline.ci.pDepthStencilState  = ci_depthstencil; 
  pipeline.ci.pColorBlendState    = ci_colorblend; 
  pipeline.ci.pDynamicState       = ci_dynamic_state; 
  pipeline.ci.layout              = pipeline.layout.handle; 
  pipeline.ci.renderPass          = render_pass;
  pipeline.ci.subpass             = 0;
  pipeline.ci.basePipelineHandle  = VK_NULL_HANDLE; 
  pipeline.ci.basePipelineIndex   = -1;              

  if (vkCreateGraphicsPipelines (device, VK_NULL_HANDLE, 1, &pipeline.ci, nullptr, &pipeline.handle) != VK_SUCCESS) {
    printf("failed to create graphics pipeline!");
    return false;
  }

  return true; 
  
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkGraphicsPipelineCreateInfo& rokz::CreateInfo (VkGraphicsPipelineCreateInfo&                      ci, 
                                                const VkPipelineLayout&                            pipeline_layout,
                                                const VkRenderPass&                                render_pass,
                                                const std::vector<VkPipelineShaderStageCreateInfo>& ci_shader_stages, 
                                                const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
                                                const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
                                                const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
                                                const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
                                                const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
                                                const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
                                                const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
                                                const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state)
{
  ci = {}; 
  ci.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  ci.pNext               = nullptr;
  ci.flags               = 0x0; 
  ci.stageCount          = ci_shader_stages.size();
  ci.pStages             = &ci_shader_stages[0]; 
  ci.pVertexInputState   = ci_vertex_input_state; ;
  ci.pInputAssemblyState = ci_input_assembly;
  ci.pViewportState      = ci_viewport_state;
  ci.pRasterizationState = ci_rasterizer;
  ci.pMultisampleState   = ci_multisampling;
  ci.pDepthStencilState  = ci_depthstencil; 
  ci.pColorBlendState    = ci_colorblend; 
  ci.pDynamicState       = ci_dynamic_state; 
  ci.layout              = pipeline_layout; 
  ci.renderPass          = render_pass;
  ci.subpass             = 0;
  ci.basePipelineHandle  = VK_NULL_HANDLE; 
  ci.basePipelineIndex   = -1;              

  return ci; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateGraphicsPipeline (rokz::Pipeline& pipeline, const VkDevice device) {

  if (vkCreateGraphicsPipelines (device, VK_NULL_HANDLE, 1, &pipeline.ci, nullptr, &pipeline.handle) != VK_SUCCESS) {
    printf("failed to create graphics pipeline!");
    return false;
  }

  return true; 

  }



