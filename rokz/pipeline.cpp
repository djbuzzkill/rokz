#include "pipeline.h"




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
    rokz::Pipeline&             pipeline,
    //const PipelineLayout&       pipeline_layout,
    const VkRenderPass&         render_pass,
    const std::vector<VkPipelineShaderStageCreateInfo> ci_shader_stages, 
    const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
    const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
    const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
    const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
    const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
    const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
    const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
    const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state, 
    const VkDevice                                     device)
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

