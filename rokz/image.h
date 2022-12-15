

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
    
    // vmaCreateBuffer(glob.allocator, &buffer_info, &allocInfo, &buffer, &allocation, nullptr);
    VmaAllocationCreateInfo alloc_ci; 
    VmaAllocation           allocation;
    VmaAllocationInfo       alloc_info_;
  
  }; 

  // ------------------------------------------------------------------
  struct ImageView {

    VkImageView           handle;
    VkImageViewCreateInfo ci; 

    VmaAllocationCreateInfo alloc_ci; 
    VmaAllocation           allocation;
    VmaAllocationInfo       alloc_info_;
  };
  



  
  // ------------------------------------------------------------------
  VkImageCreateInfo& Init_2D_device (VkImageCreateInfo& ci, VkImageUsageFlags usage, VkSampleCountFlagBits num_samples, uint32_t wd, uint32_t ht);
  bool CreateImage (Image& image, const VkDevice& device); 
  
  VkMemoryAllocateInfo& Init (VkMemoryAllocateInfo& alloc_info,  VkMemoryPropertyFlags prop_flags, const VkImage& image, const VkDevice& device, const VkPhysicalDevice& physdev); 
  bool AllocateImageMemory (Image& image,  const VkDevice& device); 


  VkImageViewCreateInfo& Init (VkImageViewCreateInfo& ci, VkImageAspectFlags aspect_flags, const Image& image); 
  bool CreateImageView (ImageView& iv, const VkImageViewCreateInfo& ci, const VkDevice& device); 
 
  void Destroy (Image& image, const VkDevice& device);
  void Destroy (ImageView& image, const VkDevice& device);
  

  // VMA ----------->
  // VkImageCreateInfo&       Init_2D_dev (VkImageCreateInfo& ci, VkImageUsageFlags usage, VkSampleCountFlagBits num_samples, uint32_t wd, uint32_t ht);
  VkImageCreateInfo&       CreateInfo_2D_sample  (VkImageCreateInfo& ci,
                                                  VkSampleCountFlagBits num_samples,
                                                  uint32_t wd, uint32_t ht);

  VkImageCreateInfo&       CreateInfo_2D_depthstencil (VkImageCreateInfo& ci,
                                                       VkFormat format, 
                                                       VkSampleCountFlagBits num_samples,
                                                       uint32_t wd, uint32_t ht);


  VkImageCreateInfo&       CreateInfo_2D_color  (VkImageCreateInfo& ci,
                                                 VkFormat format, 
                                                 VkSampleCountFlagBits num_samples,
                                                 uint32_t wd, uint32_t ht);
                                                 
  
  //VmaAllocationCreateInfo& Init        (VmaAllocationCreateInfo& alloc_info); 
  bool                CreateImage (Image& image, VmaAllocator const& allocator);

  void                Destroy       (Image& image, VmaAllocator const& allocator); 

  // <----------- VMA
}

#endif
