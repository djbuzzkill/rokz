

#ifndef ROKZ_IMAGE_H
#define ROKZ_IMAGE_H


#include "common.h"
#include "shared_types.h"


namespace rokz {


  struct Image {

    VkImage              handle;
    VkImageCreateInfo    ci;
    VkDeviceMemory       mem; 
    VkMemoryAllocateInfo alloc_info;
  
  }; 


  struct ImageView {

    VkImageView           handle;
    VkImageViewCreateInfo ci; 
  };
  
  
 VkImageCreateInfo& Init_2D_device ( VkImageCreateInfo& ci, uint32_t wd, uint32_t ht);

 bool CreateImage (Image& image, const VkImageCreateInfo& ci, const VkDevice& device, const VkPhysicalDevice& phsydev); 
 bool AllocateImageMemory (Image& image, const VkImageCreateInfo& ci, const VkPhysicalDevice& physdev, const VkDevice& device);

 bool CreateImageView (ImageView& iv, const VkImageViewCreateInfo& ci, const VkDevice& device); 


 VkImageViewCreateInfo& Init (VkImageViewCreateInfo& ci, const Image& image); 
 
 void Destroy (Image& image, const VkDevice& device);
 void Destroy (ImageView& image, const VkDevice& device);
 
  
}
#endif
