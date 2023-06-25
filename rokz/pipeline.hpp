//
#ifndef ROKZ_PIPELINE_H
#define ROKZ_PIPELINE_H

#include "common.hpp"
#include "rokz_types.hpp"
#include <vulkan/vulkan_core.h>


namespace rokz {

  inline VkViewport& Viewport (VkViewport& vp, float x, float y, float wd, float ht, float dp) {
    // VIEWPORT 
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
  inline VkViewport& Viewport (VkViewport& vp, const VkOffset2D& offs, const VkExtent2D& ext, float dp) {
    return Viewport (vp, offs.x, offs.y, ext.width, ext.height, dp); 
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
  std::vector<VkDynamicState>& DynamicState_default    (std::vector<VkDynamicState>& dynamic_states);
  bool                         ColorBlendState_default (VkPipelineColorBlendAttachmentState& color_blend_attachment_state); 
  ViewportState&               ViewportState_default   (ViewportState& vps, const VkRect2D& rect, float depth) ; 
  bool                         ColorBlendState_test (VkPipelineColorBlendAttachmentState& color_blend_attachment_state); 

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  bool CreateGraphicsPipelineLayout (VkPipelineLayout&            pipeline_layout,
                                     VkPipelineLayoutCreateInfo&  create_info, 
                                     const VkDescriptorSetLayout& desc_set_layout, 
                                     const VkDevice&              device); 

  bool DefineGraphicsPipelineLayout (VkPipelineLayout&            pipeline_layout,
                                     VkPipelineLayoutCreateInfo&  create_info,
                                     uint32_t                     push_constant_size,
                                     VkShaderStageFlags           stageflags, 
                                     const std::vector<VkDescriptorSetLayout>& desc_set_layouts, 
                                     const VkDevice&              device); 

  bool CreateGraphicsPipelineLayout (VkPipelineLayout&                 pipeline_layout,
                                     const VkPipelineLayoutCreateInfo& create_info,
                                     const VkDevice&                   device); 

  VkPipelineColorBlendStateCreateInfo&    CreateInfo (VkPipelineColorBlendStateCreateInfo& color_blending_create_info,
                                                      const VkPipelineColorBlendAttachmentState& colorblend);
  VkPipelineColorBlendStateCreateInfo&    CreateInfo (VkPipelineColorBlendStateCreateInfo& color_blending_create_info,
                                                      const std::vector<VkPipelineColorBlendAttachmentState>& colorblends);


  VkPipelineDynamicStateCreateInfo&       CreateInfo (VkPipelineDynamicStateCreateInfo& ci, const std::vector<VkDynamicState>& dynamics); 
  VkPipelineVertexInputStateCreateInfo&   CreateInfo (VkPipelineVertexInputStateCreateInfo& create_info,
                                                      const VkVertexInputBindingDescription& binding_desc,
                                                      const std::vector<VkVertexInputAttributeDescription>& attrib_desc); 
  VkPipelineMultisampleStateCreateInfo&   CreateInfo (VkPipelineMultisampleStateCreateInfo& ci, VkSampleCountFlagBits msaa_samples = VK_SAMPLE_COUNT_1_BIT);
  VkPipelineDepthStencilStateCreateInfo&  CreateInfo (VkPipelineDepthStencilStateCreateInfo& ci);
  VkPipelineInputAssemblyStateCreateInfo& CreateInfo (VkPipelineInputAssemblyStateCreateInfo& ci, VkPrimitiveTopology prim = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST); 

  VkPipelineViewportStateCreateInfo&      CreateInfo (VkPipelineViewportStateCreateInfo& ci, const ViewportState& vps); 
  VkPipelineViewportStateCreateInfo&      CreateInfo (VkPipelineViewportStateCreateInfo& ci,  const std::vector<VkViewport>& vps, const std::vector<VkRect2D>& scissors); 


  VkPipelineRasterizationStateCreateInfo& CreateInfo (VkPipelineRasterizationStateCreateInfo& ci);

  VkPipelineTessellationStateCreateInfo&  CreateInfo (VkPipelineTessellationStateCreateInfo& ci, uint32_t num_control_points); 

  
  VkPipelineLayoutCreateInfo&             CreateInfo (VkPipelineLayoutCreateInfo& ci,
                                                      const std::vector<VkDescriptorSetLayout>& dslos,
                                                      const std::vector<VkPushConstantRange>& pc = std::vector<VkPushConstantRange>(0)); 


  VkPipelineRenderingCreateInfoKHR& CreateInfo  (VkPipelineRenderingCreateInfoKHR& ci,
                                                 const std::vector<VkFormat>& color_formats,
                                                 VkFormat depth_format = VK_FORMAT_UNDEFINED,
                                                 VkFormat stencil_format = VK_FORMAT_UNDEFINED) ; 

  
  //
  VkGraphicsPipelineCreateInfo&           CreateInfo (VkGraphicsPipelineCreateInfo&                      ci, 
                                                      VkRenderPass                                       render_pass,
                                                      VkPipelineLayout                                   pipeline_layout,
                                                      const void*                                        next, 
                                                      const std::vector<VkPipelineShaderStageCreateInfo>& ci_shader_stages, 
                                                      const VkPipelineInputAssemblyStateCreateInfo*      ci_input_assembly, 
                                                      const VkPipelineVertexInputStateCreateInfo*        ci_vertex_input_state,
                                                      const VkPipelineViewportStateCreateInfo*           ci_viewport_state, 
                                                      const VkPipelineTessellationStateCreateInfo*       ci_tesselation, 
                                                      const VkPipelineRasterizationStateCreateInfo*      ci_rasterizer, 
                                                      const VkPipelineMultisampleStateCreateInfo*        ci_multisampling,
                                                      const VkPipelineDepthStencilStateCreateInfo*       ci_depthstencil, 
                                                      const VkPipelineColorBlendStateCreateInfo*         ci_colorblend, 
                                                      const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state); 


  //
  // VkGraphicsPipelineCreateInfo - similar to above except using dynamic render pass
  VkGraphicsPipelineCreateInfo&          CreateInfo (VkGraphicsPipelineCreateInfo&                      ci, 
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
                                                     const VkPipelineDynamicStateCreateInfo*            ci_dynamic_state); 



  ViewportState&  SetupViewportState (ViewportState & vps, const VkExtent2D& swapchain_extent);
  // ----------------------------------------------------------------------------------------
  //
  // ----------------------------------------------------------------------------------------

  PipelineState& PipelineState_tessellation (
   rokz::PipelineState&                                  ps,
   VkSampleCountFlagBits                                 msaa_samples,
   uint32                                                control_points,
   
   const std::vector<VkVertexInputAttributeDescription>& vert_input_attrb_desc,
   const VkVertexInputBindingDescription&                vert_bindiing_desc,
   const VkExtent2D&                                     vpext
   );


  PipelineState& PipelineState_default (rokz::PipelineState&                                  ps,
                                        VkPrimitiveTopology                                   prim,
                                        VkSampleCountFlagBits                                 msaa_samples,
                                        const std::vector<VkVertexInputAttributeDescription>& vert_input_attrb_desc,
                                        const VkVertexInputBindingDescription&                vert_bindiing_desc,
                                        const VkExtent2D&                                     vpext
                                        );

  PipelineState& PipelineState_alpha_test (rokz::PipelineState&                                  ps,
                                           VkPrimitiveTopology                                   prim,
                                           VkSampleCountFlagBits                                 msaa_samples,
                                           const std::vector<VkVertexInputAttributeDescription>& vert_input_attrb_desc,
                                           const VkVertexInputBindingDescription&                vert_bindiing_desc,
                                           const VkExtent2D&                                     vpext
                                           );
  bool SetupPipelinShaderStages (std::vector<VkPipelineShaderStageCreateInfo>& shader_stage_cis,
                                 Vec<rokz::ShaderModule>& shader_modules, 
                                 const Vec<ShaderStageDef>& stagedefs, const rokz::Device& device);
  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  bool CreateGraphicsPipeline (rokz::Pipeline& pipeline, const VkDevice device); 



  namespace rc {
  }  
}
  
#endif
