

#include "descriptor.h"
#include "utility.h"
#include <vulkan/vulkan_core.h>


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
VkDescriptorPoolCreateInfo& rokz::cx::CreateInfo (VkDescriptorPoolCreateInfo& ci, uint32_t max_sets,
                                                  const std::vector<VkDescriptorPoolSize>& sizes) {
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

// ----------------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------------
VkDescriptorSetLayoutCreateInfo& rokz::cx::CreateInfo (VkDescriptorSetLayoutCreateInfo& ci,
                                                       const std::vector<VkDescriptorSetLayoutBinding>& bindings) {

    ci = {};
    ci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    ci.pNext = nullptr;
    ci.bindingCount = bindings.size();
    ci.pBindings = &bindings[0];
    return ci;
}


// ----------------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------------
bool rokz::cx::CreateDescriptorSetLayout (VkDescriptorSetLayout& dslo,
                                          const VkDescriptorSetLayoutCreateInfo& ci,
                                          const VkDevice& device) {

  if (vkCreateDescriptorSetLayout (device, &ci, nullptr, &dslo) != VK_SUCCESS) {
      printf ("[FAILED] %s create descriptor set layout", __FUNCTION__);
      return false; 
    }
    
    return true;
}

// ----------------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------------
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

// ----------------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------------

void rokz::cx::Destroy (DescriptorPool& dsl, const Device& device) {
  vkDestroyDescriptorPool(device.handle, dsl.handle, nullptr);
  dsl.handle = VK_NULL_HANDLE;
  
}

void rokz::cx::Destroy (DescriptorSetLayout& dsl, const Device& device) {

  vkDestroyDescriptorSetLayout (device.handle, dsl.handle, nullptr);
  dsl.handle = VK_NULL_HANDLE; 
}

void rokz::cx::FreeDescriptorSets (std::vector<VkDescriptorSet>& descriptorsets, const DescriptorPool& descrpool, const VkDevice& device) {

  vkFreeDescriptorSets (device, descrpool.handle, descriptorsets.size(), &descriptorsets[0] ); 

  //Destroy (dg.dslayout, device); 
}

// ----------------------------------------------------------------------------------------------
// rokz::CreateDescriptorSetLayout
// ----------------------------------------------------------------------------------------------
bool rokz::DefineDescriptorSetLayout (DescriptorSetLayout& dslo, const std::vector<VkDescriptorSetLayoutBinding>& bindings,  const rokz::Device& device) {

  printf ("%s", __FUNCTION__); 

  if (!rokz::cx::CreateDescriptorSetLayout (dslo.handle, dslo.ci, bindings, device.handle)) {
    printf (" --> [FAILED] \n"); 
    return false;
  }

  printf (" --> true\n"); 
  return true; 
}

// -------------------------------------------------------------------------------------------
//  
// -------------------------------------------------------------------------------------------
bool rokz::MakeDescriptorPool (rokz::DescriptorPool& dp, uint32_t mul, 
                            const std::vector<VkDescriptorSetLayoutBinding>& bindings,
                            const rokz::Device& device) {

  printf ("bindings.size:%zu\n", bindings.size() );
  dp.sizes.resize (bindings.size()); 
  for (size_t i = 0; i < bindings.size(); ++i) {
    printf ("mul:%u | bindings[%zu].descriptorCount:%u\n", mul, i,  bindings[i].descriptorCount );
    dp.sizes[i] = { bindings[i].descriptorType, mul * bindings[i].descriptorCount };
  }
  
  rokz::cx::CreateInfo ( dp.ci, mul, dp.sizes); 
  
  if (!rokz::cx::CreateDescriptorPool (dp, device.handle)) {
    printf ("[FAILED] %s", __FUNCTION__);
    return false; 
  }

 return true; 

}

// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
bool rokz::MakeDescriptorSets (std::vector<VkDescriptorSet>& dss, VkDescriptorSetAllocateInfo& alloc_info,
                         uint32_t num_sets, VkDescriptorSetLayout dslo, const rokz::DescriptorPool& pool, rokz::Device& device) {
  //
  printf ("[%i] %s\n", __LINE__, __FUNCTION__);
  //  SetupObjectDescriptorSets does too many things. it:
  //  - initializes DescriptorGroup::vector<vkDescriptorSets>
  //  - allocates from descriptor pool, but relies on externally initialized DescriptorPool
  //  - binds resources to the descriptors vkUpdaateDescriptorSets

  // use same layout for both allocations
  std::vector<VkDescriptorSetLayout> dslos (num_sets, dslo);
  // could have also said: 
  //    VkDescriptorSetLayout[]  desc_layouts = { dg.set_layout.handle, dg.diff_set_layout.handle }; 
  // but that wouldnt work
  rokz::cx::AllocateInfo (alloc_info , dslos, pool);
  
  if (!rokz::cx::AllocateDescriptorSets (dss, num_sets, alloc_info, device.handle)) {
    printf ("[FAILED] alloc desc sets %s\n", __FUNCTION__);
    return false;
  }

  return true;
}
