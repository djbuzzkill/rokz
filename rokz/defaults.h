
#ifndef ROKZ_DEFAULTS_INCLUDE
#define ROKZ_DEFAULTS_INCLUDE

#include "common.h"


namespace rokz {





  VkPipelineVertexInputStateCreateInfo& Init (VkPipelineVertexInputStateCreateInfo& create_info,
                                              const VkVertexInputBindingDescription& binding_desc, 
                                              const std::vector<VkVertexInputAttributeDescription>& attrib_desc); 


// ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  VkRect2D&                               Init (VkRect2D& rect, const VkOffset2D& offs, const VkExtent2D& ext); 
  VkViewport&                             Init (VkViewport& vp, float wd, float  ht, float dp); 
  VkPipelineMultisampleStateCreateInfo&   Init (VkPipelineMultisampleStateCreateInfo& ci);
  VkPipelineInputAssemblyStateCreateInfo& Init (VkPipelineInputAssemblyStateCreateInfo& ci, VkPrimitiveTopology prim = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST); 
  VkPipelineRasterizationStateCreateInfo& Init (VkPipelineRasterizationStateCreateInfo& ci);
  VkPipelineDepthStencilStateCreateInfo&  Init (VkPipelineDepthStencilStateCreateInfo& ci);
  VkDescriptorSetLayoutBinding&           Init (VkDescriptorSetLayoutBinding& ds, uint32_t index, VkDescriptorType desc_type, VkShaderStageFlagBits stage_flags); 

  
  VkPipelineViewportStateCreateInfo&      Init (VkPipelineViewportStateCreateInfo& ci, const VkViewport& vp, const VkRect2D& scissor); 

  VkDescriptorSetLayoutCreateInfo&        Init (VkDescriptorSetLayoutCreateInfo& ci);

  
  
  VkInstanceCreateInfo&                    Default (VkInstanceCreateInfo& create_info); // 
  VkDeviceCreateInfo&                      Default (VkDeviceCreateInfo& create_info);
  VkDeviceQueueCreateInfo&                 Default (VkDeviceQueueCreateInfo& create_info);    
  VkSwapchainCreateInfoKHR&                Default (VkSwapchainCreateInfoKHR& create_info);  
  VkImageViewCreateInfo&                   Default (VkImageViewCreateInfo& create_info);  
  VkRenderPassCreateInfo&                  Default (VkRenderPassCreateInfo& create_info); 
  VkGraphicsPipelineCreateInfo&            Default (VkGraphicsPipelineCreateInfo& create_info); 
  VkPipelineInputAssemblyStateCreateInfo&  Default (VkPipelineInputAssemblyStateCreateInfo& create_info);       //      ateInfo input_assembly{};
  VkPipelineDynamicStateCreateInfo&        Default (VkPipelineDynamicStateCreateInfo& create_info); //         dynamic_state_create_info {};
  VkPipelineViewportStateCreateInfo&       Default (VkPipelineViewportStateCreateInfo& create_info);    //    viewport_state_create_info{};
  VkPipelineRasterizationStateCreateInfo&  Default (VkPipelineRasterizationStateCreateInfo& create_info);//  rasterizer{};
  VkPipelineMultisampleStateCreateInfo&    Default (VkPipelineMultisampleStateCreateInfo& create_info);  //   multisampling{};
  VkPipelineDepthStencilStateCreateInfo&   Default (VkPipelineDepthStencilStateCreateInfo& create_info);  //    pipeline_depth_stencil_create_info {};
  VkPipelineColorBlendStateCreateInfo&     Default (VkPipelineColorBlendStateCreateInfo& create_info);    //    color_blending_create_info{};
  VkPipelineLayoutCreateInfo&              Default (VkPipelineLayoutCreateInfo& create_info);
  VkPipelineShaderStageCreateInfo&         Default (VkPipelineShaderStageCreateInfo& create_info); //      vert_shader_stage_info {};

  
}                                                                                 
#endif
