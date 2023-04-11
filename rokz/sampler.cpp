
#include "sampler.hpp"
#include <vulkan/vulkan_core.h>


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::cx::CreateSampler (VkSampler& sampler, const VkSamplerCreateInfo& ci, const VkDevice& device) {
  
  if (vkCreateSampler (device, &ci, nullptr, &sampler) != VK_SUCCESS) {
    printf ("failed to create texture sampler!");
    return false;
  }
  return true;
}
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
void rokz::cx::Destroy (VkSampler& sampler, const VkDevice& device) {
  vkDestroySampler (device, sampler, nullptr);
  sampler = VK_NULL_HANDLE;
}

// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
bool rokz::cx::CreateSampler (rokz::Sampler& sampler, const VkDevice& device){
  return  CreateSampler (sampler.handle, sampler.ci, device); 
}

void rokz::cx::Destroy (rokz::Sampler& sampler, const VkDevice& device) {

  Destroy (sampler.handle, device);
}
  
// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
VkSamplerCreateInfo& rokz::cx::CreateInfo_height_sample (VkSamplerCreateInfo& ci,
                                                         float max_sampler_anisotropy) {
  ci =  {}; 
  ci.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  ci.pNext                   = nullptr;
  ci.flags                   = 0;

  ci.magFilter               = VK_FILTER_LINEAR; 
  ci.minFilter               = VK_FILTER_LINEAR; 
  
  ci.addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;  
  ci.addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  ci.addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;

  ci.anisotropyEnable        = VK_TRUE;
  ci.maxAnisotropy           = max_sampler_anisotropy; ;
  ci.borderColor             = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
  ci.unnormalizedCoordinates = VK_FALSE;
  ci.compareEnable           = VK_FALSE;
  ci.compareOp               = VK_COMPARE_OP_ALWAYS; 
  ci.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR; 
  ci.mipLodBias              = 0.0f;
  ci.minLod                  = 0.0f;
  ci.maxLod                  = 0.0f; 

  return ci;
}

VkSamplerCreateInfo& rokz::cx::CreateInfo (VkSamplerCreateInfo& ci, const VkPhysicalDeviceProperties& props) {
  printf ("%s SamplerCreateInfo \n", __FUNCTION__); 

  ci =  {}; 

  ci.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  ci.pNext                   = nullptr;
  ci.flags                   = 0;

  ci.magFilter               = VK_FILTER_LINEAR;
  ci.minFilter               = VK_FILTER_LINEAR;
  ci.addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE; // VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VK_SAMPLER_ADDRESS_MODE_REPEAT
  ci.addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  ci.addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
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

  return ci;
}

