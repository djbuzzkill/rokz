
//
#ifndef ROKZ_DESCRIPTOR_H 
#define ROKZ_DESCRIPTOR_H


#include  "common.h"
#include  "shared_types.h"


namespace rokz {

  // ---------------------------------------------------------------------
  struct DescriptorPool {

    VkDescriptorPool                  handle;
    std::vector<VkDescriptorPoolSize> sizes;
    VkDescriptorPoolCreateInfo        ci;
  };

  // ---------------------------------------------------------------------
  struct DescriptorSetLayout { 
    VkDescriptorSetLayout                     handle;    
    VkDescriptorSetLayoutCreateInfo           ci;
    std::vector<VkDescriptorSetLayoutBinding> bindings; 
    
  }; 

  // ---------------------------------------------------------------------
  struct DescriptorGroup {
    
    std::vector<VkDescriptorSet>   desc_sets;
    rokz::DescriptorSetLayout      set_layout;
    VkDescriptorSetAllocateInfo    alloc_info;

  }; 

  // ---------------------------------------------------------------------
  VkDescriptorSetLayoutBinding&
  DescriptorSetLayoutBinding (VkDescriptorSetLayoutBinding& out, uint32_t binding,
                             VkDescriptorType desc_type, VkShaderStageFlagBits stage_flags, 
                             const VkSampler* pImmutableSamplers = nullptr)  ; 
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

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  bool AllocateDescriptorSets (std::vector<VkDescriptorSet>& desc_sets,
                               VkDescriptorSetAllocateInfo& alloc_info,
                               uint32_t num_sets,
                               const VkDevice &device); 


  void Destroy (DescriptorPool& dsl, const VkDevice& device) ;
  void Destroy (DescriptorSetLayout& dsl, const VkDevice& device) ;
  void Destroy (DescriptorGroup& dg, const VkDevice& device) ;
  
}


#endif
