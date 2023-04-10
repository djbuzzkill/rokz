//
#include "pipeline.h"
#include "shader.h"
#include "utility.h"
#include <vulkan/vulkan_core.h>

// ------------------------------------------------------------------------------------------
//                      
// ------------------------------------------------------------------------------------------
std::vector<VkDynamicState>& rokz::DynamicState_default (std::vector<VkDynamicState>& dynamic_states) {
  // DYNAMIC STATE
  dynamic_states.clear ();
  dynamic_states.push_back (VK_DYNAMIC_STATE_VIEWPORT);
  dynamic_states.push_back (VK_DYNAMIC_STATE_SCISSOR);
  dynamic_states.push_back (VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE);
  dynamic_states.push_back (VK_DYNAMIC_STATE_DEPTH_COMPARE_OP);
  //dynamic_states.push_back (VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT);

  // ?! wtf
  //dynamic_states.push_back (VK_DYNAMIC_STATE_POLYGON_MODE_EXT);
  return dynamic_states; 
}

// ------------------------------------------------------------------------------------------
//                      
// ------------------------------------------------------------------------------------------
rokz::ViewportState& rokz::ViewportState_default (rokz::ViewportState& vps, const VkRect2D& rect, float fdepth) {

  vps.vps.resize (1);
  vps.vps[0].scissor = rect;
  rokz::Viewport (vps.vps[0].viewport, rect.offset, rect.extent, fdepth);  

  return vps;
}

// -----------------------------------------------------------------------------------------
//                                             
// -----------------------------------------------------------------------------------------

// void rekz::SetupViewportState (rokz::ViewportState & vps, const VkExtent2D& swapchain_extent) {

//   const VkOffset2D offs0 {0, 0};

//   vps.viewports.resize (1);
//   vps.scissors.resize (1);
  
//   vps.scissors[0] = { offs0, swapchain_extent };
//   rokz::ViewportState_default (vps, vps.scissors[0], 1.0f); 

// }

// -----------------------------------------------------------------------------------------
// dynamics state
// -----------------------------------------------------------------------------------------
VkPipelineDynamicStateCreateInfo& rokz::CreateInfo (VkPipelineDynamicStateCreateInfo& ci, const std::vector<VkDynamicState>& dynamics) {

  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  ci.pNext = nullptr;
  ci.dynamicStateCount = static_cast<uint32_t>(dynamics.size());
  ci.pDynamicStates = &dynamics[0];

  return ci;
}

// -----------------------------------------------------------------------------------------
// color blend 
// -----------------------------------------------------------------------------------------
bool rokz::ColorBlendState_default (VkPipelineColorBlendAttachmentState& color_blend_attachment_state) {
  // COLOR BLENDING
  color_blend_attachment_state = {};
  
  color_blend_attachment_state.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

  color_blend_attachment_state.blendEnable = VK_TRUE;
  
  color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; 
  color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; 

  color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; 
  color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

  color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD; 
  color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD; 

  return true;
}

// -----------------------------------------------------------------------------------------
// color blend 
// -----------------------------------------------------------------------------------------
bool rokz::ColorBlendState_test  (VkPipelineColorBlendAttachmentState& color_blend_attachment_state) {
  // COLOR BLENDING
  color_blend_attachment_state = {};
  
  color_blend_attachment_state.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

  color_blend_attachment_state.blendEnable = VK_TRUE;
  
  color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; 
  color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD; 

  color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA; 
  color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; ;
  color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD; 

  return true;
}
// -----------------------------------------------------------------------------------------
// colorblend blend
// -----------------------------------------------------------------------------------------
VkPipelineColorBlendStateCreateInfo& rokz::CreateInfo ( VkPipelineColorBlendStateCreateInfo& color_blending_create_info,
                                                        const VkPipelineColorBlendAttachmentState& colorblend) {   // Create Info
  color_blending_create_info = {};
  color_blending_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blending_create_info.pNext = nullptr;
  color_blending_create_info.logicOpEnable = VK_FALSE;
  color_blending_create_info.logicOp = VK_LOGIC_OP_COPY; 
  color_blending_create_info.attachmentCount = 1;
  color_blending_create_info.pAttachments = &colorblend;
  color_blending_create_info.blendConstants[0] = 0.0f; 
  color_blending_create_info.blendConstants[1] = 0.0f; 
  color_blending_create_info.blendConstants[2] = 0.0f; 
  color_blending_create_info.blendConstants[3] = 0.0f; 

  return color_blending_create_info ;
}

// -----------------------------------------------------------------------------------------
// colorblend blend 
// -----------------------------------------------------------------------------------------
VkPipelineColorBlendStateCreateInfo& rokz::CreateInfo ( VkPipelineColorBlendStateCreateInfo& color_blending_create_info,
                                                        const std::vector<VkPipelineColorBlendAttachmentState>& colorblends) {   // Create Info
  color_blending_create_info = {};
  color_blending_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blending_create_info.pNext = nullptr;
  color_blending_create_info.logicOpEnable = VK_FALSE;
  color_blending_create_info.logicOp = VK_LOGIC_OP_COPY; 
  color_blending_create_info.attachmentCount = colorblends.size();
  color_blending_create_info.pAttachments = &colorblends[0];
  color_blending_create_info.blendConstants[0] = 0.0f; 
  color_blending_create_info.blendConstants[1] = 0.0f; 
  color_blending_create_info.blendConstants[2] = 0.0f; 
  color_blending_create_info.blendConstants[3] = 0.0f; 

  return color_blending_create_info ;
}

// -----------------------------------------------------------------------------------------
// vertex input 
// -----------------------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------------------
// pipeline multisample msaa 
// -----------------------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------------------
// depth stencil pipeline
// -----------------------------------------------------------------------------------------
VkPipelineDepthStencilStateCreateInfo& rokz::CreateInfo (VkPipelineDepthStencilStateCreateInfo& ci) {
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  ci.pNext = nullptr;
  ci.depthTestEnable = VK_FALSE;
  ci.depthWriteEnable = VK_TRUE;
  ci.depthCompareOp = VK_COMPARE_OP_LESS;  // VK_COMPARE_OP_ALWAYS
  ci.depthBoundsTestEnable = VK_FALSE;
  ci.stencilTestEnable = VK_FALSE;
  return ci; 
}

// -----------------------------------------------------------------------------------------
// input assembly pipeline
// -----------------------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------------------
//  viewport state viewportstate
// -----------------------------------------------------------------------------------------
VkPipelineViewportStateCreateInfo& rokz::CreateInfo (VkPipelineViewportStateCreateInfo& ci, const ViewportState& vps) {
  // VkPipelineViewportStateCreateInfo
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  ci.pNext = nullptr;
  ci.viewportCount = vps.vps.size ();
  ci.pViewports    = &vps.vps[0].viewport; 
  ci.scissorCount  = vps.vps.size();
  ci.pScissors     = &vps.vps[0].scissor;

  return ci; 
}

// -----------------------------------------------------------------------------------------
// viewport state viewportstate
// -----------------------------------------------------------------------------------------
VkPipelineViewportStateCreateInfo& rokz::CreateInfo (VkPipelineViewportStateCreateInfo& ci,
                                                     const std::vector<VkViewport>& vps,
                                                     const std::vector<VkRect2D>& scissors) {

  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  ci.pNext = nullptr;
  ci.viewportCount = vps.size();
  ci.pViewports = &vps[0]; 

  ci.scissorCount = scissors.size();
  ci.pScissors = &scissors[0];
  return ci;
}
// -----------------------------------------------------------------------------------------
// rasterization
// -----------------------------------------------------------------------------------------
VkPipelineRasterizationStateCreateInfo & rokz::CreateInfo (VkPipelineRasterizationStateCreateInfo& ci) {
  //printf ("%s -> VkPipelineRasterizationStateCreateInfo& \n", __FUNCTION__); 
  // RASTERIZATION STATE .. VkPipelineRasterizationStateCreateInfo
  ci = {};
  ci.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  ci.pNext                   = nullptr;
  ci.depthClampEnable        = VK_FALSE;
  ci.rasterizerDiscardEnable = VK_FALSE;
  ci.polygonMode             = VK_POLYGON_MODE_FILL; // 
  ci.lineWidth               = 1.0f;
  ci.cullMode                = VK_CULL_MODE_BACK_BIT;
  ci.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE; // VK_FRONT_FACE_CLOCKWISE;
  ci.depthBiasEnable         = VK_FALSE;
  ci.depthBiasConstantFactor = 0.0f; 
  ci.depthBiasClamp          = 0.0f;          
  ci.depthBiasSlopeFactor    = 0.0f;    
  return ci;
}

// ---------------------------------------------------------------------
// VkPipelineTessellationStateCreateInfo
// ---------------------------------------------------------------------
// VkPipelineRasterizationStateCreateInfo & rokz::CreateInfo (VkPipelineRasterizationStateCreateInfo& ci, VkPolygonMode polymode, ) {
//   //printf ("%s -> VkPipelineRasterizationStateCreateInfo& \n", __FUNCTION__); 
//   // RASTERIZATION STATE .. VkPipelineRasterizationStateCreateInfo
//   ci = {};
//   ci.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//   ci.pNext                   = nullptr;
//   ci.depthClampEnable        = VK_FALSE;
//   ci.rasterizerDiscardEnable = VK_FALSE;
//   ci.polygonMode             = VK_POLYGON_MODE_FILL; // 
//   ci.lineWidth               = 1.0f;
//   ci.cullMode                = VK_CULL_MODE_BACK_BIT;
//   ci.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE; // VK_FRONT_FACE_CLOCKWISE;
//   ci.depthBiasEnable         = VK_FALSE;
//   ci.depthBiasConstantFactor = 0.0f; 
//   ci.depthBiasClamp          = 0.0f;          
//   ci.depthBiasSlopeFactor    = 0.0f;    
//   return ci;
// }

// ---------------------------------------------------------------------
// VkPipelineTessellationStateCreateInfo
// ---------------------------------------------------------------------
VkPipelineTessellationStateCreateInfo& rokz::CreateInfo (VkPipelineTessellationStateCreateInfo& ci, uint32_t num_control_points) { 
 
  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
  ci.pNext = nullptr;
  ci.flags = 0;
  ci.patchControlPoints = num_control_points;
  return ci;
}

// -----------------------------------------------------------------------------------------
// VkPipelineLayoutCreateInfo                                                              
// -----------------------------------------------------------------------------------------
VkPipelineLayoutCreateInfo& rokz::CreateInfo (VkPipelineLayoutCreateInfo& ci,
                                              const std::vector<VkDescriptorSetLayout>& dslos,
                                              const std::vector<VkPushConstantRange> & pc) {

  ci.sType                = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  ci.pNext                  = nullptr;
  ci.flags                  = 0; 

  ci.setLayoutCount         = dslos.size();            
  ci.pSetLayouts            = dslos.size() ? &dslos[0] : nullptr;

  ci.pushConstantRangeCount = pc.size();    
  ci.pPushConstantRanges    = pc.size() ? &pc[0] : nullptr;

  return ci;
}

// ---------------------------------------------------------------------
// CreateGraphicsPipelineLayout renamed 
// ---------------------------------------------------------------------
bool rokz::DefineGraphicsPipelineLayout (
    VkPipelineLayout&            pipeline_layout,
    VkPipelineLayoutCreateInfo&  create_info,
    uint32_t                     push_constant_size,
    VkShaderStageFlags           pcstages, 
    const std::vector<VkDescriptorSetLayout>& desc_set_layouts, 
    const VkDevice&              device)
{
  // PIPELINE LAYOUT CREATE INFO << mostly empty for now
  create_info.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  create_info.pNext          = nullptr;
  create_info.flags          = 0; // ?? VK_PIPELINE_LAYOUT_CREATE_INDEPENDENT_SETS_BIT_EXT; 

  create_info.setLayoutCount = desc_set_layouts.size();            
  create_info.pSetLayouts    = &desc_set_layouts[0];         
  
  create_info.pushConstantRangeCount = 0;    
  create_info.pPushConstantRanges = nullptr; 

  VkPushConstantRange pcr;
  if (push_constant_size) {

    pcr.stageFlags = pcstages; 
    pcr.offset     = 0;
    pcr.size       = push_constant_size; 
    create_info.pushConstantRangeCount = 1;    
    create_info.pPushConstantRanges = &pcr; 
  }

  //
  if (vkCreatePipelineLayout (device, &create_info, nullptr, &pipeline_layout) != VK_SUCCESS) {
    printf("FAILED _create pipeline layout_\n");
    return false;
  }

  return true;
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
  create_info.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  create_info.pNext          = nullptr;
  create_info.flags          = 0; // ?? VK_PIPELINE_LAYOUT_CREATE_INDEPENDENT_SETS_BIT_EXT; 

  create_info.setLayoutCount = 1;            
  create_info.pSetLayouts    = &desc_set_layout;         

  create_info.pushConstantRangeCount = 0;    
  create_info.pPushConstantRanges = nullptr; 

  bool enable_push_constants = false;
  VkPushConstantRange pcr;
  if (enable_push_constants) {
    pcr.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    pcr.offset     = 0;
    pcr.size       = sizeof(glm::mat4) * 2; 
    create_info.pushConstantRangeCount = 1;    
    create_info.pPushConstantRanges = &pcr; 
  }

  //
  if (vkCreatePipelineLayout (device, &create_info, nullptr, &pipeline_layout) != VK_SUCCESS) {
    printf("FAILED _create pipeline layout_\n");
    return false;
  }

  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateGraphicsPipelineLayout (
    VkPipelineLayout&                 pipeline_layout,
    const VkPipelineLayoutCreateInfo& create_info,
    const VkDevice&                   device)
{
  //
  if (vkCreatePipelineLayout (device, &create_info, nullptr, &pipeline_layout) != VK_SUCCESS) {

    printf ("%s --> [FAILED ] create graphics pipeline layout\n", __FUNCTION__); 
    return false;
  }

  return true;
}

// ---------------------------------------------------------------------
// VkGraphicsPipelineCreateInfo 
// ---------------------------------------------------------------------
VkGraphicsPipelineCreateInfo& rokz::CreateInfo (VkGraphicsPipelineCreateInfo&                      ci, 
                                                const VkPipelineLayout&                            pipeline_layout,
                                                const VkRenderPass&                                render_pass,
                                                const std::vector<VkPipelineShaderStageCreateInfo>& ci_shader_stages, 
                                                const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
                                                const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
                                                const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
                                                const VkPipelineTessellationStateCreateInfo*       ci_tesselation, 
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
  ci.pTessellationState  = ci_tesselation; 
  ci.pInputAssemblyState = ci_input_assembly;
  ci.pViewportState      = ci_viewport_state;
  ci.pRasterizationState = ci_rasterizer;
  ci.pMultisampleState   = ci_multisampling;
  ci.pDepthStencilState  = ci_depthstencil; 
  ci.pColorBlendState    = ci_colorblend; 
  ci.pDynamicState       = ci_dynamic_state; 
  ci.layout              = pipeline_layout; 
  ci.renderPass          = render_pass;              // a pipeline is bound a render pass
  ci.subpass             = 0;                        // and also to a specific subpass
  ci.basePipelineHandle  = VK_NULL_HANDLE; 
  ci.basePipelineIndex   = -1;              

  return ci; 
}




// ---------------------------------------------------------------------
// VkGraphicsPipelineCreateInfo 
// ---------------------------------------------------------------------


VkPipelineRenderingCreateInfoKHR& rokz::CreateInfo  (VkPipelineRenderingCreateInfoKHR& ci,

                                                     const std::vector<VkFormat>& color_formats,
                                                     VkFormat depth_format,
                                                     VkFormat stencil_format) {
  // typedef struct VkPipelineRenderingCreateInfo {
  //     VkStructureType    sType;
  //     const void*        pNext;
  //     uint32_t           viewMask;
  //     uint32_t           colorAttachmentCount;
  //     const VkFormat*    pColorAttachmentFormats;
  //     VkFormat           depthAttachmentFormat;
  //     VkFormat           stencilAttachmentFormat;
  // } VkPipelineRenderingCreateInfo;

  ci.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO; 
  ci.pNext = nullptr;
  ci.viewMask = 0;
  ci.colorAttachmentCount    = color_formats.size ();
  ci.pColorAttachmentFormats = &color_formats[0]; 
  ci.depthAttachmentFormat   = depth_format;
  ci.stencilAttachmentFormat = stencil_format; 

  return ci; 
}
                                               

// ---------------------------------------------------------------------
// VkGraphicsPipelineCreateInfo 
// ---------------------------------------------------------------------
VkGraphicsPipelineCreateInfo& rokz::CreateInfo (VkGraphicsPipelineCreateInfo&                      ci, 
                                                const VkPipelineLayout&                            pipeline_layout,
                                                const VkPipelineRenderingCreateInfoKHR*            ci_pipeline_rendering, 
                                                const std::vector<VkPipelineShaderStageCreateInfo>& ci_shader_stages, 
                                                const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
                                                const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
                                                const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
                                                const VkPipelineTessellationStateCreateInfo*       ci_tesselation, 
                                                const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
                                                const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
                                                const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
                                                const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
                                                const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state)
{
  ci = {}; 
  ci.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  // VkPipelineRenderingCreateInfoKHR pipeline_rendering_create_info {
  //   .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR
  //   .colorAttachmentCount = 1,
  //   .pColorAttachmentFormats = &swapchain_image_format_,
  // };
  ci.pNext               = ci_pipeline_rendering;
  ci.flags               = 0x0; 
  ci.stageCount          = ci_shader_stages.size();
  ci.pStages             = &ci_shader_stages[0]; 
  ci.pVertexInputState   = ci_vertex_input_state; ;
  ci.pTessellationState  = ci_tesselation; 
  ci.pInputAssemblyState = ci_input_assembly;
  ci.pViewportState      = ci_viewport_state;
  ci.pRasterizationState = ci_rasterizer;
  ci.pMultisampleState   = ci_multisampling;
  ci.pDepthStencilState  = ci_depthstencil; 
  ci.pColorBlendState    = ci_colorblend; 
  ci.pDynamicState       = ci_dynamic_state; 
  ci.layout              = pipeline_layout; 
  ci.renderPass          = nullptr;              // a pipeline is bound a render pass
  ci.subpass             = 0;                        // and also to a specific subpass
  ci.basePipelineHandle  = VK_NULL_HANDLE; 
  ci.basePipelineIndex   = -1;              

  return ci; 
}
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateGraphicsPipeline (rokz::Pipeline& pipeline, const VkDevice device) {

  if (vkCreateGraphicsPipelines (device, VK_NULL_HANDLE, 1, &pipeline.ci, nullptr, &pipeline.handle) != VK_SUCCESS) {
    HERE("FAILED");
    return false;
  }
  return true; 

}



 
// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
rokz::PipelineState& rokz::PipelineState_alpha_test (rokz::PipelineState&                                  ps,
                                                     VkPrimitiveTopology                                   prim,
                                                     VkSampleCountFlagBits                                 msaa_samples,
                                                     const std::vector<VkVertexInputAttributeDescription>& vert_input_attrib_desc,
                                                     const VkVertexInputBindingDescription&                vert_bindiing_desc,
                                                     const VkExtent2D&                                     vpext) {
  
  SetupViewportState (ps.viewport, vpext); 

  ps.colorblend_attachments.resize (1);
  ColorBlendState_test (ps.colorblend_attachments[0]); 

  DynamicState_default (ps.dynamics); 
  //
  PipelineStateCreateInfo& psci = ps.ci;
  CreateInfo (psci.tesselation, 4); 
  CreateInfo (psci.dynamicstate, ps.dynamics); 
  CreateInfo (psci.vertexinputstate, vert_bindiing_desc, vert_input_attrib_desc); 
  CreateInfo (psci.viewport_state, ps.viewport);
  CreateInfo (psci.input_assembly, prim);  // VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST | VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP); 
  CreateInfo (psci.rasterizer); 
  CreateInfo (psci.colorblendstate, ps.colorblend_attachments); 
  CreateInfo (psci.multisampling, msaa_samples); 
  CreateInfo (psci.depthstencilstate); 

  return ps;
  
}
// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
rokz::PipelineState& rokz::PipelineState_default (rokz::PipelineState&                                  ps,
                                                  VkPrimitiveTopology                                   prim,
                                                  VkSampleCountFlagBits                                 msaa_samples,
                                                  const std::vector<VkVertexInputAttributeDescription>& vert_input_attrib_desc,
                                                  const VkVertexInputBindingDescription&                vert_bindiing_desc,
                                                  const VkExtent2D&                                     vpext) {
  
  SetupViewportState (ps.viewport, vpext); 

  ps.colorblend_attachments.resize (1);

  ColorBlendState_default (ps.colorblend_attachments[0]); 

  DynamicState_default (ps.dynamics); 
  //
  PipelineStateCreateInfo& psci = ps.ci;
  CreateInfo (psci.tesselation, 4); 
  CreateInfo (psci.dynamicstate, ps.dynamics); 
  CreateInfo (psci.vertexinputstate, vert_bindiing_desc, vert_input_attrib_desc); 
  CreateInfo (psci.viewport_state, ps.viewport);
  CreateInfo (psci.input_assembly, prim);  // VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST | VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP); 
  CreateInfo (psci.rasterizer); 
  CreateInfo (psci.colorblendstate, ps.colorblend_attachments); 
  CreateInfo (psci.multisampling, msaa_samples); 
  CreateInfo (psci.depthstencilstate); 

  return ps;
  
}
// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
rokz::PipelineState& rokz::PipelineState_tessellation (rokz::PipelineState&                                 ps,
                                                      VkSampleCountFlagBits                                 msaa_samples,
                                                      uint32                                                controlpoints, 
                                                      const std::vector<VkVertexInputAttributeDescription>& vert_input_attrib_desc,
                                                      const VkVertexInputBindingDescription&                vert_bindiing_desc,
                                                      const VkExtent2D&                                     vpext) {

  SetupViewportState (ps.viewport, vpext); 

  ps.colorblend_attachments.resize (1);

  ColorBlendState_default (ps.colorblend_attachments[0]); 

  DynamicState_default (ps.dynamics); 
  //
  PipelineStateCreateInfo& psci = ps.ci;
  CreateInfo (psci.tesselation, controlpoints); 
  CreateInfo (psci.dynamicstate, ps.dynamics); 
  CreateInfo (psci.vertexinputstate, vert_bindiing_desc, vert_input_attrib_desc); 
  CreateInfo (psci.viewport_state, ps.viewport);
  CreateInfo (psci.input_assembly, VK_PRIMITIVE_TOPOLOGY_PATCH_LIST); 
  CreateInfo (psci.rasterizer); 
  CreateInfo (psci.colorblendstate, ps.colorblend_attachments); 
  CreateInfo (psci.multisampling, msaa_samples); 
  CreateInfo (psci.depthstencilstate); 

  return ps;
  
}

// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
rokz::ViewportState& rokz::SetupViewportState (rokz::ViewportState & vps, const VkExtent2D& swapchain_extent) {

  const VkOffset2D offs0 {0, 0};

  vps.vps.resize (1);
  
  vps.vps[0].scissor = { offs0, swapchain_extent };
  rokz::ViewportState_default (vps, vps.vps[0].scissor, 1.0f); 
  return vps;
  
}

// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
bool rokz::SetupPipelinShaderStages (std::vector<VkPipelineShaderStageCreateInfo>& shader_stage_cis,
                                     Vec<rokz::ShaderModule>& shader_modules, 
                                     const Vec<ShaderStageDef>& stagedefs, const rokz::Device& device) {

  shader_modules.resize (stagedefs.size());
  shader_stage_cis.resize (stagedefs.size());
  
  for (size_t istage = 0; istage < stagedefs.size(); ++istage) { 
    // FRAG SHADER
    if (!rokz::CompileThisShader_file (shader_modules[istage].spv, stagedefs[istage].stage, stagedefs[istage].fqsource)) 
      return false; 

    CreateInfo (shader_modules[istage].ci, shader_modules[istage].spv); 
    if (!rokz::CreateShaderModule_spv (shader_modules[istage], device.handle))
      return false; 

    // for pipeline state ci
    rokz::CreateInfo (shader_stage_cis[istage], stagedefs[istage].stage, stagedefs[istage].entrypoint, shader_modules[istage].handle); 
  }

  return true;
}
