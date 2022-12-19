

#ifndef ROKZ_IMAGE_H
#define ROKZ_IMAGE_H


#include "common.h"
#include "shared_types.h"


namespace rokz {

  constexpr VkImageUsageFlags kSamplingUsage     = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
  constexpr VkImageUsageFlags kDepthStencilUsage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; 
  constexpr VkImageUsageFlags kColorTargetUsage  = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; 
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
  VkImageCreateInfo& CreateInfo_2D (VkImageCreateInfo&    ci,
                                    VkFormat              format,
                                    VkImageUsageFlags     usage_flags, 
                                    VkSampleCountFlagBits num_samples,
                                    uint32_t wd, uint32_t ht);

  // 
  //
  inline VkImageCreateInfo&CreateInfo_2D_color_sampling (VkImageCreateInfo& ci,
                                                         VkSampleCountFlagBits num_samples,
                                                         uint32_t wd, uint32_t ht) {
    return CreateInfo_2D (ci, VK_FORMAT_B8G8R8A8_SRGB,  kSamplingUsage, num_samples, wd, ht); 
  }
  
  // 
  //
  inline VkImageCreateInfo& CreateInfo_2D_depthstencil (VkImageCreateInfo& ci,
                                                       VkFormat format, 
                                                       VkSampleCountFlagBits num_samples,
                                                        uint32_t wd, uint32_t ht) {
    return CreateInfo_2D (ci, format, kDepthStencilUsage, num_samples, wd, ht); 
  }

  // 
  //
  inline VkImageCreateInfo& CreateInfo_2D_color_target (VkImageCreateInfo& ci,
                                                        VkFormat format, 
                                                        VkSampleCountFlagBits num_samples,
                                                        uint32_t wd, uint32_t ht) {
    return CreateInfo_2D (ci, format, kColorTargetUsage, num_samples, wd, ht); 
  }
                                                 
  // 
  //
  bool                CreateImage   (Image& image, VmaAllocator const& allocator);
  // 
  //
  void                Destroy       (Image& image, VmaAllocator const& allocator); 

  // <----------- VMA
}

#endif
