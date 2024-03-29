

#include "image.hpp"
#include "buffer.hpp"
#include "rc_image.hpp"
#include "utility.hpp"
#include "allocation.hpp"
#include "command.hpp"
#include <vulkan/vulkan_core.h>


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::cx::CreateImage (Image& image, const VkDevice& device) {
  printf ("%s\n", __FUNCTION__);  

  if (vkCreateImage (device, &image.ci, nullptr, &image.handle) != VK_SUCCESS) {
    printf ("[FAILED] %s create image\n", __FUNCTION__);
    return false;
  }
  
  return true; 
}

#ifdef ROKZ_HIDE_CX_ALLOCINFO
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkMemoryAllocateInfo& rokz::cx::AllocInfo (VkMemoryAllocateInfo& alloc_info, VkMemoryPropertyFlags prop_flags, const VkImage& image, const VkDevice& device, const VkPhysicalDevice& physdev) {

  VkMemoryRequirements mem_reqs;
  vkGetImageMemoryRequirements(device, image, &mem_reqs);
 
  alloc_info = {};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.pNext = nullptr;
  alloc_info.allocationSize = mem_reqs.size;
  ut::FindMemoryType (alloc_info.memoryTypeIndex, mem_reqs.memoryTypeBits, prop_flags, physdev);
  return alloc_info; 
}
#endif

#ifdef ROKZ_HIDE_CX_ALLOCATEIMAGEMEMORY
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::cx::AllocateImageMemory (rokz::Image& image, const VkDevice& device) {
  printf ("%s\n", __FUNCTION__);  

  if (vkAllocateMemory (device, &image.alloc_info, nullptr, &image.mem) != VK_SUCCESS) {
    printf ("[FAILED] %s allocate memory\n", __FUNCTION__);
    return false; 
  }

  if (VK_SUCCESS != vkBindImageMemory (device, image.handle, image.mem, 0)) {

      printf ("[FAILED] %s bind memory\n", __FUNCTION__);
      return false; 
    }
  
  return true; 
}
#endif

// ------------------------------------------------------------------------------------------
//                      
// ------------------------------------------------------------------------------------------
bool rokz::cx::CreateImageView (VkImageView& iv, const VkImageViewCreateInfo& ci, const VkDevice& device) {

  //printf ("%s\n", __FUNCTION__);  

  if (vkCreateImageView (device, &ci, nullptr, &iv) != VK_SUCCESS) {
    printf ("[FAILED] %s  create image view!", __FUNCTION__);
    return false;
  }
  
  return true; 
}

// ------------------------------------------------------------------------------------------
//                      
// ------------------------------------------------------------------------------------------
VkImageCreateInfo& rokz::cx::CreateInfo (VkImageCreateInfo& ci, const VkSwapchainCreateInfoKHR& swapchain_ci) {
  // printf ("%s VMA\n", __FUNCTION__); 
  ci = {}; 
  ci.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  ci.pNext = nullptr;
  ci.imageType     = VK_IMAGE_TYPE_2D;
  ci.extent.width  = swapchain_ci.imageExtent.width;
  ci.extent.height = swapchain_ci.imageExtent.height;
  ci.extent.depth  = 1;
  ci.mipLevels     = 1;
  ci.arrayLayers   = 1;
  ci.format        = swapchain_ci.imageFormat; 
  ci.tiling        = VK_IMAGE_TILING_OPTIMAL;
  ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  ci.usage         = swapchain_ci.imageUsage;
  ci.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;  
  ci.samples       = VK_SAMPLE_COUNT_1_BIT; // ??is it  always 1 ??
  ci.flags         = 0; 

  return ci;
}



// ------------------------------------------------------------------------------------------
//                      
// ------------------------------------------------------------------------------------------
VkImageViewCreateInfo& rokz::cx::CreateInfo (VkImageViewCreateInfo& ci, VkFormat format, VkImageAspectFlagBits aspect_flags, const VkImage& image) { 

  ci = {}; 
  ci.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  ci.pNext = nullptr;
  ci.image    = image;
  ci.viewType = VK_IMAGE_VIEW_TYPE_2D;
  ci.format   = format;
    
  ci.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  ci.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  ci.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  ci.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

  ci.subresourceRange.aspectMask = aspect_flags; 
  ci.subresourceRange.baseMipLevel = 0;
  ci.subresourceRange.levelCount = 1;
  ci.subresourceRange.baseArrayLayer = 0;
  ci.subresourceRange.layerCount = 1;
  return ci; 
}


// rokz::cx::
// ------------------------------------------------------------------------------------------
//                      
// ------------------------------------------------------------------------------------------
VkImageViewCreateInfo& rokz::cx::CreateInfo_2D_array (VkImageViewCreateInfo& ci,
                                                      VkFormat              format,
                                                      rokz::uint32           layercount, 
                                                      const VkImage&         image) {

  ci.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  ci.pNext    = nullptr;
  ci.image    = image;
  ci.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
  ci.format   = format;
    
  ci.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  ci.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  ci.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  ci.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

  ci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; 
  ci.subresourceRange.baseMipLevel = 0;
  ci.subresourceRange.levelCount = 1;
  ci.subresourceRange.baseArrayLayer = 0;
  ci.subresourceRange.layerCount = layercount;
  return ci; 

}




// ------------------------------------------------------------------------------------------
//                      
// ------------------------------------------------------------------------------------------
VkImageViewCreateInfo& rokz::cx::CreateInfo (VkImageViewCreateInfo& ci, VkImageAspectFlagBits aspect_flags, const Image& image) {

  return CreateInfo (ci, image.ci.format, aspect_flags, image.handle);
}
// ------------------------------------------------------------------------------------------
//                      
// ------------------------------------------------------------------------------------------
void rokz::cx::Destroy (VkImageView& iv, const VkDevice& device) {
  vkDestroyImageView (device, iv, nullptr);
  iv = VK_NULL_HANDLE;
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void rokz::cx::Destroy (ImageView& iv, const VkDevice& device) {
  Destroy (iv, device);
}


// ---------------------------------------------------------------------------------------------
// VMA
// ---------------------------------------------------------------------------------------------
bool CreateImage (VkImage&                   image,
                  VmaAllocationCreateInfo&   alloc_ci,
                  VmaAllocationInfo          alloc_info, 
                  VmaAllocation&             allocation,
                  const VkImageCreateInfo&   ci,
                  VmaAllocator const&        allocator) {
  
  if( VK_SUCCESS != vmaCreateImage (allocator, &ci, &alloc_ci, &image, &allocation, &alloc_info)) {
    printf ("[FAILED] %s vmaCreateImage\n", __FUNCTION__); 
    return false; 
  }
  
  return true; 
}

// ---------------------------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------------------------
bool rokz::cx::CreateImage (Image& image, VmaAllocator const& allocator) {
  return  CreateImage (image.handle, image.alloc_ci, image.alloc_info, image.allocation, image.ci, allocator);
}




// ---------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------
void rokz::cx::Destroy (VkImage& image, VmaAllocation allocation,  VmaAllocator allocator) {

  vmaDestroyImage (allocator, image, allocation); 
  image = VK_NULL_HANDLE;
}

// ---------------------------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------------------------
void rokz::cx::Destroy (Image& image, VmaAllocator allocator) {

  cx::Destroy (image.handle, image.allocation, allocator);
}




// ---------------------------------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------------------------------
VkImageCreateInfo& rokz::cx::CreateInfo_2D_array (VkImageCreateInfo&    ci,
                                        VkFormat              format,
                                        uint32                layercount, 
                                        VkImageUsageFlags     usage_flags, 
                                        uint32_t wd, uint32_t ht) {
  printf ("%s VMA\n", __FUNCTION__); 
  ci = {}; 
  ci.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  ci.pNext = nullptr;
  ci.imageType     = VK_IMAGE_TYPE_2D;
  ci.extent        = { wd, ht, 1 };
  ci.mipLevels     = 1;
  ci.arrayLayers   = layercount;
  ci.format        = format ; // 
  ci.tiling        = VK_IMAGE_TILING_OPTIMAL;
  ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  ci.usage         = usage_flags;  // kSamplingUsage =>  VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST
  ci.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;  
  ci.samples       = VK_SAMPLE_COUNT_1_BIT; 
  ci.flags         = 0; 

  return ci;
}


// ---------------------------------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------------------------------
VkImageCreateInfo& rokz::cx::CreateInfo_2D (VkImageCreateInfo&    ci,
                                        VkFormat              format,
                                        VkImageUsageFlags     usage_flags, 
                                        VkSampleCountFlagBits num_samples,
                                        uint32_t wd, uint32_t ht) {
  printf ("%s VMA\n", __FUNCTION__); 
  ci = {}; 
  ci.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  ci.pNext = nullptr;
  ci.imageType     = VK_IMAGE_TYPE_2D;
  ci.extent.width  = wd;
  ci.extent.height = ht;
  ci.extent.depth  = 1;
  ci.mipLevels     = 1;
  ci.arrayLayers   = 1;
  ci.format        = format ; // 
  ci.tiling        = VK_IMAGE_TILING_OPTIMAL;
  ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  ci.usage         = usage_flags; 
  ci.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;  
  ci.samples       = num_samples;
  ci.flags         = 0; 

  return ci;
}

