//
#ifndef DARK_OBJ_PIPELINE
#define DARK_OBJ_PIPELINE

#include "darkrootgarden.h"

namespace darkroot {

  bool SetupObjDescriptorPool (Glob& glob); 

  bool SetupObjectDescriptorSets (rekz::PipelineGroup& pipelinegroup,

                                  const std::vector<rokz::Buffer>& vma_uniform_buffs,
                                  const std::vector<rokz::Buffer>& vma_objparam_buffs,

                                  const rokz::ImageView& texture_imageview, 
                                  const rokz::Sampler&   sampler, 

                                  const rokz::DescriptorPool& descpool,
                                  const rokz::Device&         device); 

  bool SetupObjectDescriptorLayout (rokz::DescriptorGroup& descrgroup, const rokz::Device& device); 

  bool SetupObjectShaderModules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device);
  
  
  bool SetupObjectPipeline (rekz::PipelineGroup& pipelinegroup,
                            const std::filesystem::path& fspath,
                            const VkExtent2D& viewport_extent, //const rokz::Swapchain& swapchain,
                            VkSampleCountFlagBits msaa_samples,
                            VkFormat color_format,
                            VkFormat depth_format,
                            const rokz::Device& device); 


}



#endif
