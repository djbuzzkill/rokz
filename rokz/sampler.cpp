
#include "sampler.h"



// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateSampler (Sampler& sampler, const VkDevice& device){

  printf ("%s \n", __FUNCTION__); 
  
  if (vkCreateSampler (device, &sampler.ci, nullptr, &sampler.handle) != VK_SUCCESS) {
    printf ("failed to create texture sampler!");
    return false;
  }

  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
void rokz::Destroy (Sampler& sampler, const VkDevice& device) {

  vkDestroySampler(device, sampler.handle, nullptr);
}
  
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkSamplerCreateInfo& rokz::CreateInfo (VkSamplerCreateInfo& ci, const VkPhysicalDeviceProperties& props) {

  printf ("%s SamplerCreateInfo \n", __FUNCTION__); 

  ci =  {}; 

  ci.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  ci.pNext                   = nullptr;
  ci.flags                   = 0;

  ci.magFilter               = VK_FILTER_LINEAR;
  ci.minFilter               = VK_FILTER_LINEAR;
  ci.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  ci.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  ci.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  ci.anisotropyEnable        = VK_TRUE;
  ci.maxAnisotropy           = props.limits.maxSamplerAnisotropy;
  ci.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  ci.unnormalizedCoordinates = VK_FALSE;
  ci.compareEnable           = VK_FALSE;
  ci.compareOp               = VK_COMPARE_OP_ALWAYS; 
  ci.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  ci.mipLodBias              = 0.0f;
  ci.minLod                  = 0.0f;
  ci.maxLod                  = 0.0f; 

  ci.compareEnable = VK_FALSE;
  ci.compareOp = VK_COMPARE_OP_ALWAYS;

  ci.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  ci.mipLodBias = 0.0f;
  ci.minLod = 0.0f;
  ci.maxLod = 0.0f;

  return ci;
}

