//
#ifndef ROKZ_PIPELINE_H
#define ROKZ_PIPELINE_H

#include "common.h"
#include "shared_types.h"


namespace rokz {

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  inline VkViewport& Viewport (VkViewport& vp, float x, float y, float wd, float ht, float dp) {
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
  inline VkRect2D& Rect2D (VkRect2D& rect, const VkOffset2D& offs, const VkExtent2D& ext) { 
    // SCISSOR RECT
    rect = {}; 
    rect.offset = offs;
    rect.extent = ext;
    return rect; 
  }

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  bool CreateGraphicsPipelineLayout  (VkPipelineLayout&            pipeline_layout,
                                     VkPipelineLayoutCreateInfo&  create_info, 
                                     const VkDescriptorSetLayout& desc_set_layout, 
                                     const VkDevice&              device); 

  VkPipelineVertexInputStateCreateInfo&   CreateInfo (VkPipelineVertexInputStateCreateInfo& create_info,
                                                      const VkVertexInputBindingDescription& binding_desc,
                                                      const std::vector<VkVertexInputAttributeDescription>& attrib_desc); 
  VkPipelineMultisampleStateCreateInfo&   CreateInfo (VkPipelineMultisampleStateCreateInfo& ci, VkSampleCountFlagBits msaa_samples = VK_SAMPLE_COUNT_1_BIT);
  VkPipelineDepthStencilStateCreateInfo&  CreateInfo (VkPipelineDepthStencilStateCreateInfo& ci);
  VkPipelineInputAssemblyStateCreateInfo& CreateInfo (VkPipelineInputAssemblyStateCreateInfo& ci, VkPrimitiveTopology prim = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST); 
  VkPipelineShaderStageCreateInfo&        CreateInfo (VkPipelineShaderStageCreateInfo& ci, VkShaderStageFlagBits stage_flags, const VkShaderModule& module); 
  VkPipelineViewportStateCreateInfo&      CreateInfo (VkPipelineViewportStateCreateInfo& ci, const VkViewport& vp, const VkRect2D& scissor); 
  VkPipelineRasterizationStateCreateInfo& CreateInfo (VkPipelineRasterizationStateCreateInfo& ci);

  VkPipelineLayoutCreateInfo&             CreateInfo (VkPipelineLayoutCreateInfo& ci, const std::vector<VkDescriptorSetLayout>& dslos, const std::vector<VkPushConstantRange>& pc = std::vector<VkPushConstantRange>(0)); 

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  VkGraphicsPipelineCreateInfo&           CreateInfo (VkGraphicsPipelineCreateInfo&                      ci, 
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
                                                      const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state); 

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  bool CreateGraphicsPipeline (
    rokz::Pipeline&          pipeline,
    const VkDevice           device); 

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  bool CreateGraphicsPipeline (
    rokz::Pipeline&             pipeline,
    //const PipelineLayout&       pipeline_layout,
    const VkRenderPass&         render_pass,
    const std::vector<VkPipelineShaderStageCreateInfo>& ci_shader_stages, 
    const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
    const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
    const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
    const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
    const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
    const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
    const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
    const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state, 
    const VkDevice&                                     device); 
  
}
  
#endif
