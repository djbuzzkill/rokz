
#ifndef ROKZ_SAMPLER_H
#define ROKZ_SAMPLER_H

#include "common.hpp"
#include "rokz_types.hpp"


namespace rokz {


  namespace cx { 
    VkSamplerCreateInfo& CreateInfo    (VkSamplerCreateInfo& ci, const VkPhysicalDeviceProperties& props); 
    VkSamplerCreateInfo& CreateInfo_height_sample (VkSamplerCreateInfo& ci, float max_sampler_anisotropy);
    // deprecate
    bool                 CreateSampler (Sampler& sampler, const VkDevice& device); 
    void                 Destroy       (Sampler& sampler, const VkDevice& device); 
    // pass native vk
    bool                 CreateSampler (VkSampler& sampler, const VkSamplerCreateInfo& ci, const VkDevice& device); 
    void                 Destroy       (VkSampler& sampler, const VkDevice& device);

  } // cx
} // rokz
    

#endif
