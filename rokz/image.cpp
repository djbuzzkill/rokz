

#include "image.h"
#include "utility.h"



// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateImage (Image& image, const VkDevice& device) {
  printf ("%s\n", __FUNCTION__);  

  if (vkCreateImage (device, &image.ci, nullptr, &image.handle) != VK_SUCCESS) {
    printf ("[FAILED] %s create image\n", __FUNCTION__);
    return false;
  }
  
  return true; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkMemoryAllocateInfo& rokz::AllocInfo (VkMemoryAllocateInfo& alloc_info, VkMemoryPropertyFlags prop_flags, const VkImage& image, const VkDevice& device, const VkPhysicalDevice& physdev) {

  VkMemoryRequirements mem_reqs;
  vkGetImageMemoryRequirements(device, image, &mem_reqs);
 
  alloc_info = {};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.pNext = nullptr;
  alloc_info.allocationSize = mem_reqs.size;
  rokz::FindMemoryType (alloc_info.memoryTypeIndex, mem_reqs.memoryTypeBits,
                        prop_flags, physdev);

  return alloc_info; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::AllocateImageMemory (rokz::Image& image, const VkDevice& device) {
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


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateImageView (ImageView& iv, const VkImageViewCreateInfo& ci, const VkDevice& device) {

  //printf ("%s\n", __FUNCTION__);  

  if (vkCreateImageView (device, &ci, nullptr, &iv.handle) != VK_SUCCESS) {
    printf ("[FAILED] %s  create image view!", __FUNCTION__);
    return false;
  }
  
  return true; 
}

// ---------------------------------------------------------------------
// CreateInfo for images from swapchain
// ---------------------------------------------------------------------
VkImageCreateInfo& rokz::CreateInfo (VkImageCreateInfo& ci, const VkSwapchainCreateInfoKHR& swapchain_ci) {
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

// ---------------------------------------------------------------------
// CreateINFO for imageview from image 
// ---------------------------------------------------------------------
VkImageViewCreateInfo& rokz::CreateInfo (VkImageViewCreateInfo& ci, VkImageAspectFlags aspect_flags, const Image& image) {

  // printf ("%s\n", __FUNCTION__);  

  ci = {}; 
  ci.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  ci.pNext = nullptr;
  ci.image    = image.handle;
  ci.viewType = VK_IMAGE_VIEW_TYPE_2D;
  ci.format   = image.ci.format; 
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

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateImageViews (std::vector<ImageView>&   imageviews,
                            const std::vector<Image>&  images,
                            const Device&              device) {

  printf ("%s\n", __FUNCTION__); 
  
  imageviews.resize (images.size()); 
  
  for (size_t i = 0; i < images.size(); i++) {
    // CREATEINFO for imageviews from swapchain images
    CreateInfo (imageviews[i].ci, VK_IMAGE_ASPECT_COLOR_BIT, images[i]); 
    
    if (!CreateImageView (imageviews[i], imageviews[i].ci, device.handle)) {
       printf ("[FAILED] %s create imageview \n", __FUNCTION__); 
    }

  }

  printf ("BAI %s\n", __FUNCTION__); 
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


// ---------------------------------------------------------------------
// VMA
// ---------------------------------------------------------------------
bool rokz::CreateImage (Image& image, VmaAllocator const& allocator) {
  image.alloc_ci = {};
  image.alloc_ci.usage = VMA_MEMORY_USAGE_AUTO;

  printf ("%s VMA\n", __FUNCTION__); 
  if( VK_SUCCESS != vmaCreateImage (allocator, &image.ci, &image.alloc_ci, &image.handle, &image.allocation, &image.alloc_info_)) {
    printf ("[FAILED] %s vmaCreateImage\n", __FUNCTION__); 
    return false; 
  }
  
  return true; 
}

// ---------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------
void rokz::Destroy (Image& image, VmaAllocator const& allocator) {
  vmaDestroyImage (allocator, image.handle, image.allocation); 
}


// ---------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------
VkImageCreateInfo& rokz::CreateInfo_2D (VkImageCreateInfo&    ci,
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




