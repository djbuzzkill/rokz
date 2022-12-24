
#ifndef ROKZ_DEFAULTS_INCLUDE
#define ROKZ_DEFAULTS_INCLUDE

#include "common.h"


namespace rokz {





  VkPipelineVertexInputStateCreateInfo& VertexInputState (VkPipelineVertexInputStateCreateInfo& create_info,
                                                          const VkVertexInputBindingDescription& binding_desc, 
                                                          const std::vector<VkVertexInputAttributeDescription>& attrib_desc); 


// ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  VkRect2D&                               Rect2D (VkRect2D& rect, const VkOffset2D& offs, const VkExtent2D& ext); 
  //  VkViewport&                             Viewport (VkViewport& vp, float wd, float  ht, float dp); 


  VkPipelineMultisampleStateCreateInfo&   CreateInfo (VkPipelineMultisampleStateCreateInfo& ci, VkSampleCountFlagBits msaa_samples = VK_SAMPLE_COUNT_1_BIT);
  VkPipelineInputAssemblyStateCreateInfo& CreateInfo  (VkPipelineInputAssemblyStateCreateInfo& ci, VkPrimitiveTopology prim = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST); 


  VkPipelineShaderStageCreateInfo&        Init (VkPipelineShaderStageCreateInfo& ci,
                                                VkShaderStageFlagBits stage_flags,
                                                const VkShaderModule& module); 

  VkPipelineRasterizationStateCreateInfo& CreateInfo (VkPipelineRasterizationStateCreateInfo& ci);

  VkPipelineDepthStencilStateCreateInfo&  CreateInfo (VkPipelineDepthStencilStateCreateInfo& ci);
  VkDescriptorSetLayoutBinding&           Init (VkDescriptorSetLayoutBinding& ds, uint32_t index, VkDescriptorType desc_type, VkShaderStageFlagBits stage_flags); 

  
  VkViewport&                             Viewport (VkViewport& vp, float x, float y, float wd, float ht, float dp); 
  VkPipelineViewportStateCreateInfo&      CreateInfo (VkPipelineViewportStateCreateInfo& ci, const VkViewport& vp, const VkRect2D& scissor); 

  VkDescriptorSetLayoutCreateInfo&        Init (VkDescriptorSetLayoutCreateInfo& ci);

  

  
}                                                                                 
#endif
