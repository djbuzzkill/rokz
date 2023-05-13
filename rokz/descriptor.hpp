
//
#ifndef ROKZ_DESCRIPTOR_H 
#define ROKZ_DESCRIPTOR_H


#include  "common.hpp"
#include  "rokz_types.hpp"


namespace rokz {

  namespace cx { 
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
                                               const DescriptorPool& descrpool); 
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

    void FreeDescriptorSets (std::vector<VkDescriptorSet>& descriptorsets, const DescriptorPool& descrpool, const VkDevice& device); 


    void Destroy (DescriptorPool& dsl, const Device& device) ;
    void Destroy (DescriptorSetLayout& dsl, const Device& device) ;

    //void Destroy (DescriptorGroup& dg, const VkDevice& device) ;
    //void Destroy (DescriptorGroup& dg, const DescriptorPool& dsl, const VkDevice& device); 


  } // cx



  typedef Vec<VkDescriptorSetLayoutBinding> DescriptorSetLayoutBindings; 
  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  bool DefineDescriptorSetLayout (DescriptorSetLayout&                             dslo,
                                const std::vector<VkDescriptorSetLayoutBinding>& bindings,
                                const Device&                              device);

  bool MakeDescriptorPool      (DescriptorPool& dp, uint32_t mul, 
                                const Vec<VkDescriptorSetLayoutBinding>& bindings,
                                const Device& device); 
  
  bool MakeDescriptorSets (std::vector<VkDescriptorSet>& dss, VkDescriptorSetAllocateInfo& alloc_info, uint32_t num_sets,
                           VkDescriptorSetLayout dslo, const DescriptorPool& pool, Device& device);



} // rokz


#endif
