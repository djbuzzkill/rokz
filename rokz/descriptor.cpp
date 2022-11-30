

#include "descriptor.h"

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateDescriptorPool (DescriptorPool& desc_pool, const VkDevice &device) {
  //
  if (vkCreateDescriptorPool(device, &desc_pool.ci, nullptr, &desc_pool.handle) != VK_SUCCESS) {
    printf ("[FAILED] %s ", __FUNCTION__);
    return false;
  }
  
  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::AllocateDescriptorSets (std::vector<VkDescriptorSet>& desc_sets,
                                   VkDescriptorSetAllocateInfo& alloc_info,
                                   uint32_t num_sets,
                                   const VkDevice &device) {

  desc_sets.resize(num_sets);

  if (vkAllocateDescriptorSets (device, &alloc_info, &desc_sets[0]) != VK_SUCCESS) {
    printf ("[FAILED] %s allocate descriptor sets", __FUNCTION__);
    return false;
  }

  return true; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateDescriptorSetLayout (VkDescriptorSetLayout& dsl, VkDescriptorSetLayoutCreateInfo& ci,
                                      const std::vector<VkDescriptorSetLayoutBinding>& bindings,
                                      const VkDevice& device) {
    
    ci = {};
    ci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    ci.bindingCount = bindings.size();
    ci.pBindings = &bindings[0];

    if (vkCreateDescriptorSetLayout (device, &ci, nullptr, &dsl) != VK_SUCCESS) {
      printf ("[FAILED] %s create descriptor set layout", __FUNCTION__);
      return false; 
    }
    
    return true;
}

// bool CreateDescriptorSetLayout (VkDescriptorSetLayout&              descriptor_set_layout,
//                                 VkDescriptorSetLayoutCreateInfo&    ci,
//                                 const VkDescriptorSetLayoutBinding& binding,
//                                 const VkDevice&                     device) {
