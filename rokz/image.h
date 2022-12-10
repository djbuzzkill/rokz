

#ifndef ROKZ_IMAGE_H
#define ROKZ_IMAGE_H


#include "common.h"
#include "shared_types.h"


namespace rokz {

  // ------------------------------------------------------------------
  struct Image {

    VkImage              handle;
    VkImageCreateInfo    ci;
    VkDeviceMemory       mem; 
    VkMemoryAllocateInfo alloc_info;
  
  }; 

  // ------------------------------------------------------------------
  struct ImageView {

    VkImageView           handle;
    VkImageViewCreateInfo ci; 
  };
  
  
  // ------------------------------------------------------------------
  VkImageCreateInfo& Init_2D_device (VkImageCreateInfo& ci, VkImageUsageFlags usage, uint32_t wd, uint32_t ht);
  bool CreateImage (Image& image, const VkDevice& device); 

  VkMemoryAllocateInfo& Init (VkMemoryAllocateInfo& alloc_info,  VkMemoryPropertyFlags prop_flags, const VkImage& image, const VkDevice& device, const VkPhysicalDevice& physdev); 
  bool AllocateImageMemory (Image& image,  const VkDevice& device); 


  VkImageViewCreateInfo& Init (VkImageViewCreateInfo& ci, VkImageAspectFlags aspect_flags, const Image& image); 
  bool CreateImageView (ImageView& iv, const VkImageViewCreateInfo& ci, const VkDevice& device); 
 
  void Destroy (Image& image, const VkDevice& device);
  void Destroy (ImageView& image, const VkDevice& device);
  
}

#endif
