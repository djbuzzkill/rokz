//
#ifndef ROKZ_PIPELINE_H
#define ROKZ_PIPELINE_H

#include "common.h"
#include "shared_types.h"


namespace rokz {


  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  bool CreateGraphicsPipelineLayout  (VkPipelineLayout&            pipeline_layout,
                                     VkPipelineLayoutCreateInfo&  create_info, 
                                     const VkDescriptorSetLayout& desc_set_layout, 
                                     const VkDevice&              device); 


  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  VkGraphicsPipelineCreateInfo& CreateInfo (VkGraphicsPipelineCreateInfo&                      ci, 
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
    const std::vector<VkPipelineShaderStageCreateInfo> ci_shader_stages, 
    const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
    const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
    const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
    const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
    const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
    const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
    const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
    const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state, 
    const VkDevice                                     device); 


  
}
  
#endif
