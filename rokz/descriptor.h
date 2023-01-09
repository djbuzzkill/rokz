
//
#ifndef ROKZ_DESCRIPTOR_H 
#define ROKZ_DESCRIPTOR_H


#include  "common.h"
#include  "shared_types.h"


namespace rokz {


  // ---------------------------------------------------------------------
  VkDescriptorSetLayoutBinding&
  DescriptorSetLayoutBinding (VkDescriptorSetLayoutBinding& out, uint32_t binding,
                              VkDescriptorType desc_type, uint32_t descrcount, 
                              VkShaderStageFlags stage_flags, 
                              const VkSampler* pImmutableSamplers = nullptr)  ; 


  VkDescriptorPoolCreateInfo& CreateInfo (VkDescriptorPoolCreateInfo& ci, uint32_t max_sets,
                                          const std::vector<VkDescriptorPoolSize>& sizes); 

  
  VkDescriptorSetLayoutCreateInfo& CreateInfo (VkDescriptorSetLayoutCreateInfo& ci,
                                               const std::vector<VkDescriptorSetLayoutBinding>& bindings) ; 

  VkDescriptorSetAllocateInfo& AllocateInfo (VkDescriptorSetAllocateInfo& alloc_info,
                                             const std::vector<VkDescriptorSetLayout>& descrlos,
                                             const rokz::DescriptorPool& descrpool); 
  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  bool CreateDescriptorPool (DescriptorPool& desc_pool, const VkDevice &device); 

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  bool CreateDescriptorSetLayout (VkDescriptorSetLayout&           dsl,
                                  VkDescriptorSetLayoutCreateInfo& ci,
                                  const std::vector<VkDescriptorSetLayoutBinding>& bindings,
                                  const VkDevice& device);

  bool CreateDescriptorSetLayout (VkDescriptorSetLayout& dsl,
                                  const VkDescriptorSetLayoutCreateInfo& ci,
                                  const VkDevice& device) ; 

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  bool AllocateDescriptorSets (std::vector<VkDescriptorSet>& desc_sets,
                               uint32_t num_sets,
                               const VkDescriptorSetAllocateInfo& alloc_info,
                               const VkDevice &device); 


  void Destroy (DescriptorPool& dsl, const VkDevice& device) ;
  void Destroy (DescriptorSetLayout& dsl, const VkDevice& device) ;
  void Destroy (DescriptorGroup& dg, const VkDevice& device) ;
  
}


#endif
