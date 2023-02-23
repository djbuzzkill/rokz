//
#ifndef DARK_OBJ_PIPELINE
#define DARK_OBJ_PIPELINE

#include "darkrootgarden.h"

namespace darkroot {


  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  extern const std::vector<VkDescriptorSetLayoutBinding>      kObjDescriptorBindings;

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  extern const VkVertexInputBindingDescription                kVertexInputBindingDesc;

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  extern const std::vector<VkVertexInputAttributeDescription> kVertexInputBindingAttributeDesc;

  // ?? who owns the descriptor pool. pipeline should not manage descriptor resources

  // bool SetupObjDescriptorPool (rokz::DescriptorPool& dp, const rokz::Device& device);
  
  // bool DarkObjDescriptorPool (rokz::DescriptorPool& dp, const rokz::Device& device);


  //const std::vector<VkDescriptorSetLayoutBinding>& DarkObjDescriptorBindings (); 
  // this should be done by the DrawSequence
  // DrawSequence::UpdateDescriptors ()
  // bool SetupObjectDescriptorSets (rokz::DescriptorGroup& descgr,

  //                                 const std::vector<rokz::Buffer>& vma_uniform_buffs,
  //                                 const std::vector<rokz::Buffer>& vma_objparam_buffs,

  //                                 const rokz::ImageView& texture_imageview, 
  //                                 const rokz::Sampler&   sampler, 

  //                                 const rokz::DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
  //                                 const rokz::Device&         device); 

  //  SetupObjectDescriptorSets does too many things. it:
  //  - initializes DescriptorGroup::vector<vkDescriptorSets>
  //  - allocates from descriptor pool, but relies on externally initialized DescriptorPool
  //  - binds resources to the descriptors vkUpdaateDescriptorSets

  //bool BindDarkObjDescriptorSetResources (VkDescriptorSet ds); 

  
  //
  // SetupObjectDescriptorLayout should be called DefObjectDescriptorLayout
  // rename (SetupObjectDescriptorLayout) -> DefineObjectDescriptorLayout
  // bool DeclObjectDescriptorLayout  (rokz::DescriptorSetLayout& descrg, const rokz::Device& device); 
  //bool SetupObjectDescriptorLayout (const rokz::DescriptorSetLayout& dslo, const rokz::Device& device); 
  //  bool DarkObjDescriptorLayout (rokz::DescriptorSetLayout& dslo, const std::vector<VkDescriptorSetLayoutBinding>& bindings, const rokz::Device& device); 

  //bool SetupObjectShaderModules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device);

  //
  // bool SetupObjectPipeline (rokz::Pipeline& pipeline,
  //                           rokz::PipelineLayout& layout,
  //                           const rokz::DescriptorSetLayout& dslo, 
  //                           const std::filesystem::path& fspath,
  //                           const VkExtent2D& viewport_extent, //const rokz::Swapchain& swapchain,
  //                           VkSampleCountFlagBits msaa_samples,
  //                           VkFormat color_format,
  //                           VkFormat depth_format,
  //                           const rokz::Device& device); 

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  //bool MakePipelineLayout  (rokz::PipelineLayout& layout, const rokz::DescriptorSetLayout& dslo, const rokz::Device& device);


  // ----------------------------------------------------------------------------------------------
  // sorta does same thing as  SetupObjectPipeline
  // ----------------------------------------------------------------------------------------------
  bool InitObjPipeline     (rokz::Pipeline&              pipeline,
                            rokz::PipelineLayout&        plo,
                            rokz::DescriptorSetLayout&   dslo,
                            //0
                            const std::filesystem::path& fspath,
                            const VkExtent2D&            viewport_extent, //const rokz::Swapchain& swapchain,
                            VkSampleCountFlagBits        msaa_samples,
                            VkFormat                     color_format,
                            VkFormat                     depth_format,
                            const rokz::Device&          device); 


  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  bool BindObjectDescriptorSets (std::vector<VkDescriptorSet>&    dss ,
                                 const std::vector<rokz::Buffer>& vma_uniform_buffs,
                                 const std::vector<rokz::Buffer>& vma_objparam_buffs,

                                 const rokz::ImageView&           texture_imageview, 
                                 const rokz::Sampler&             sampler, 
                                 const rokz::DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                 const rokz::Device&              device);

}



#endif
