

#include "descriptor.h"
#include "utility.h"


// ---------------------------------------------------------------------
// DesciptorSetLayoutBinding
// ---------------------------------------------------------------------
VkDescriptorSetLayoutBinding& rokz::cx::DescriptorSetLayoutBinding (VkDescriptorSetLayoutBinding& out,
                                                               uint32_t                       binding,
                                                               VkDescriptorType               desc_type,
                                                               uint32_t                       descrcount, 
                                                               VkShaderStageFlags             stage_flags,
                                                               const VkSampler*               p_immu) {
  out = {};
  out.binding            = binding;
  out.descriptorType     = desc_type ; //VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
  out.descriptorCount    = descrcount; 
  out.stageFlags         = stage_flags; // VK_SHADER_STAGE_VERTEX_BIT,  VK_SHADER_STAGE_ALL_GRAPHICS
  out.pImmutableSamplers = p_immu;
  return out;
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
VkDescriptorPoolCreateInfo& rokz::cx::CreateInfo (VkDescriptorPoolCreateInfo& ci, uint32_t max_sets,  const std::vector<VkDescriptorPoolSize>& sizes) {
  // typedef struct VkDescriptorPoolCreateInfo {
  //   VkStructureType                sType;
  //   const void*                    pNext;
  //   VkDescriptorPoolCreateFlags    flags;
  //   uint32_t                       maxSets;
  //   uint32_t                       poolSizeCount;
  //   const VkDescriptorPoolSize*    pPoolSizes;
  // } VkDescriptorPoolCreateInfo;
  ci.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  ci.pNext         = nullptr;
  ci.flags         = 0;
  ci.maxSets       = max_sets;
  ci.poolSizeCount = sizes.size();
  ci.pPoolSizes    = &sizes[0];
  return ci;
}
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::cx::CreateDescriptorPool (DescriptorPool& desc_pool, const VkDevice &device) {
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
VkDescriptorSetAllocateInfo& rokz::cx::AllocateInfo (VkDescriptorSetAllocateInfo& alloc_info,
                                                     const std::vector<VkDescriptorSetLayout>& descrlos,
                                                     const rokz::DescriptorPool& descrpool) {


  alloc_info = {}; 
  alloc_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  alloc_info.pNext              = nullptr;
  alloc_info.descriptorPool     = descrpool.handle; 
  alloc_info.descriptorSetCount = descrlos.size ();  // num_sets;
  alloc_info.pSetLayouts        = &descrlos[0];

  return alloc_info;
}
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::cx::AllocateDescriptorSets (std::vector<VkDescriptorSet>& desc_sets,
                                   uint32_t num_sets,
                                   const VkDescriptorSetAllocateInfo& alloc_info,
                                   const VkDevice &device) {

  printf ("%s\n", __FUNCTION__);

  desc_sets.resize(num_sets);
  VkResult res = vkAllocateDescriptorSets (device, &alloc_info, &desc_sets[0]); 

  const char*   res_str = ut::VkResult_2_string (res); 

  switch (res)
    {
    case VK_SUCCESS:
      return true;
      break;
      
    default:
      printf (" --> [FAILED] %s \n", res_str);
      return false;
      break;
    }

  return false; 
}
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkDescriptorSetLayoutCreateInfo& rokz::cx::CreateInfo (VkDescriptorSetLayoutCreateInfo& ci,
                                             const std::vector<VkDescriptorSetLayoutBinding>& bindings) {

    ci = {};
    ci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    ci.pNext = nullptr;
    ci.bindingCount = bindings.size();
    ci.pBindings = &bindings[0];
    return ci;
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::cx::CreateDescriptorSetLayout (VkDescriptorSetLayout& dsl, const VkDescriptorSetLayoutCreateInfo& ci,
                                      const VkDevice& device) {

  if (vkCreateDescriptorSetLayout (device, &ci, nullptr, &dsl) != VK_SUCCESS) {
      printf ("[FAILED] %s create descriptor set layout", __FUNCTION__);
      return false; 
    }
    
    return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::cx::CreateDescriptorSetLayout (VkDescriptorSetLayout& dsl, VkDescriptorSetLayoutCreateInfo& ci,
                                        const std::vector<VkDescriptorSetLayoutBinding>& bindings,
                                        const VkDevice& device) {
    
  CreateInfo (ci,  bindings) ; 
    
    if (vkCreateDescriptorSetLayout (device, &ci, nullptr, &dsl) != VK_SUCCESS) {
      printf ("[FAILED] %s create descriptor set layout", __FUNCTION__);
      return false; 
    }
    
    return true;
}



void rokz::cx::Destroy (DescriptorPool& dsl, const VkDevice& device) {
  vkDestroyDescriptorPool(device, dsl.handle, nullptr);
  
}

void rokz::cx::Destroy (DescriptorSetLayout& dsl, const VkDevice& device) {

  vkDestroyDescriptorSetLayout (device, dsl.handle, nullptr);
}


void rokz::cx::Destroy (DescriptorGroup& dg, const VkDevice& device) {

  Destroy (dg.dslayout, device); 
}
  
