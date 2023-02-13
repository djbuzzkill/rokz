//
#ifndef DARK_OBJ_PIPELINE
#define DARK_OBJ_PIPELINE

#include "darkrootgarden.h"

namespace darkroot {


  // ?? who owns the descriptor pool. pipeline should not manage descriptor resources
  bool SetupObjDescriptorPool (rokz::DescriptorPool& dp, const rokz::Device& device);
  
  // this should be done by the DrawSequence
  // DrawSequence::UpdateDescriptors ()
  bool SetupObjectDescriptorSets (rokz::DescriptorGroup& descgr,

                                  const std::vector<rokz::Buffer>& vma_uniform_buffs,
                                  const std::vector<rokz::Buffer>& vma_objparam_buffs,

                                  const rokz::ImageView& texture_imageview, 
                                  const rokz::Sampler&   sampler, 

                                  const rokz::DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                  const rokz::Device&         device); 


  //
  // SetupObjectDescriptorLayout should be called DefObjectDescriptorLayout
  // rename (SetupObjectDescriptorLayout) -> DefineObjectDescriptorLayout
  bool DefineObjectDescriptorLayout  (rokz::DescriptorSetLayout& descrg, const rokz::Device& device); 
  //bool SetupObjectDescriptorLayout (const rokz::DescriptorSetLayout& dslo, const rokz::Device& device); 

  
  
  bool SetupObjectShaderModules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device);
  
  
  bool SetupObjectPipeline (rokz::Pipeline& pipeline,
                                    rokz::PipelineLayout& layout,
                            const rokz::DescriptorSetLayout& dslo, 
                            const std::filesystem::path& fspath,
                            const VkExtent2D& viewport_extent, //const rokz::Swapchain& swapchain,
                            VkSampleCountFlagBits msaa_samples,
                            VkFormat color_format,
                            VkFormat depth_format,
                            const rokz::Device& device); 


}



#endif
