
#ifndef ROKZ_SAMPLER_H
#define ROKZ_SAMPLER_H

#include "common.h"
#include "shared_types.h"


namespace rokz {


  namespace cx { 
    VkSamplerCreateInfo& CreateInfo (VkSamplerCreateInfo& ci, const VkPhysicalDeviceProperties& props); 
    bool                 CreateSampler (Sampler& sampler, const VkDevice& device); 
    void                 Destroy (Sampler& sampler, const VkDevice& device); 
  } // cx
} // rokz
    

#endif
