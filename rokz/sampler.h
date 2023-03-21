
#ifndef ROKZ_SAMPLER_H
#define ROKZ_SAMPLER_H

#include "common.h"
#include "rokz_types.h"


namespace rokz {


  namespace cx { 
    VkSamplerCreateInfo& CreateInfo    (VkSamplerCreateInfo& ci, const VkPhysicalDeviceProperties& props); 
    // deprecate
    bool                 CreateSampler (Sampler& sampler, const VkDevice& device); 
    void                 Destroy       (Sampler& sampler, const VkDevice& device); 
    // pass native vk
    bool                 CreateSampler (VkSampler& sampler, const VkSamplerCreateInfo& ci, const VkDevice& device); 
    void                 Destroy       (VkSampler& sampler, const VkDevice& device);

  } // cx
} // rokz
    

#endif
