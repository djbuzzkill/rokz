
#ifndef ROKZ_DEFAULTS_INCLUDE
#define ROKZ_DEFAULTS_INCLUDE

#include "common.h"


namespace rokz {





  VkPipelineVertexInputStateCreateInfo& Init (VkPipelineVertexInputStateCreateInfo& create_info,
                                              const VkVertexInputBindingDescription& binding_desc, 
                                              const std::array<VkVertexInputAttributeDescription, 2>& attrib_desc); 


  
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
