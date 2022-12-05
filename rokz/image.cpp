

#include "image.h"
#include "utility.h"


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateImage (Image& image, const VkImageCreateInfo& ci, const VkDevice& device) {
  
  if (vkCreateImage (device, &ci, nullptr, &image.handle) != VK_SUCCESS) {
    printf ("failed to create image!");
    return false;
  }

  return true; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateImageView (ImageView& iv, const VkImageViewCreateInfo& ci, const VkDevice& device) {

  if (vkCreateImageView (device, &ci, nullptr, &iv.handle) != VK_SUCCESS) {
    printf ("[FAILED] %s  create image view!", __FUNCTION__);
    return false;
  }
  
  return true; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkImageViewCreateInfo& rokz::Init (VkImageViewCreateInfo& ci, const Image& image, VkFormat format) {

  ci = {}; 
  ci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  ci.image = image.handle;
  ci.viewType = VK_IMAGE_VIEW_TYPE_2D;
  ci.format = format;
  ci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  ci.subresourceRange.baseMipLevel = 0;
  ci.subresourceRange.levelCount = 1;
  ci.subresourceRange.baseArrayLayer = 0;
  ci.subresourceRange.layerCount = 1;
  
  return ci; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::AllocateImageMemory (Image& image, const VkImageCreateInfo& ci, const VkPhysicalDevice& physdev, const VkDevice& device) {

  VkMemoryRequirements mem_reqs;
  vkGetImageMemoryRequirements(device, image.handle, &mem_reqs);

  VkMemoryAllocateInfo alloc_info {};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.allocationSize = mem_reqs.size;
  rokz::FindMemoryType (alloc_info.memoryTypeIndex,
                        mem_reqs.memoryTypeBits,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                        physdev);

  if (vkAllocateMemory(device, &alloc_info, nullptr, &image.mem) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate image memory!");
  }

  if (VK_SUCCESS != vkBindImageMemory (device, image.handle, image.mem, 0)) {

      printf ("[FAILED] %s bind memory\n", __FUNCTION__);
      return false; 
    }
  
  return true; 
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void rokz::Destroy (Image& image, const VkDevice& device) {

    vkDestroyImage (device, image.handle, nullptr);
    vkFreeMemory   (device, image.mem, nullptr);
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void rokz::Destroy (ImageView& iv, const VkDevice& device) {

    vkDestroyImageView(device, iv.handle, nullptr);

}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
// VkImageCreateInfo& Init (VkImageCreateInfo& ci, uint32_t wd, uint32_t ht, uint32_t dp){

//   ci = {};
//   ci.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
//   ci.imageType     = VK_IMAGE_TYPE_2D;
//   ci.extent.width  = wd;
//   ci.extent.height = ht;
//   ci.extent.depth  = dp;
//   ci.mipLevels     = 1;
//   ci.arrayLayers   = 1;

//   ci.format = VK_FORMAT_R8G8B8A8_SRGB;
//   ci.tiling = VK_IMAGE_TILING_OPTIMAL;
//   ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//   ci.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
//   ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//   ci.samples = VK_SAMPLE_COUNT_1_BIT;
//   ci.flags = 0; 
//   return ci; 
// }


// ---------------------------------------------------------------------
// default values for device image
// ---------------------------------------------------------------------
VkImageCreateInfo& rokz::Init_2D_device ( VkImageCreateInfo& ci, uint32_t wd, uint32_t ht) {

  ci.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  ci.imageType     = VK_IMAGE_TYPE_2D;
  ci.extent.width  = wd;
  ci.extent.height = ht;
  ci.extent.depth  = 1;
  ci.mipLevels     = 1;
  ci.arrayLayers   = 1;
  ci.format        = VK_FORMAT_R8G8B8A8_SRGB;
  ci.tiling        = VK_IMAGE_TILING_OPTIMAL;
  ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  ci.usage         = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
  ci.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;  
  ci.samples       = VK_SAMPLE_COUNT_1_BIT;
  ci.flags         = 0; 

  return ci; 
}
