

#ifndef ROKZ_IMAGE_H
#define ROKZ_IMAGE_H


#include "common.h"
#include "rokz_types.h"
#include "rc_image.h"
#include <vulkan/vulkan_core.h>


namespace rokz {

  constexpr VkImageUsageFlags kSamplingUsage     = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
  constexpr VkImageUsageFlags kDepthStencilUsage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; 
  constexpr VkImageUsageFlags kColorTargetUsage  = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; 



  namespace cx {

    VkImageViewCreateInfo& CreateInfo (VkImageViewCreateInfo& ci, VkFormat format, VkImageAspectFlagBits aspect_flags, const VkImage& image);
    VkImageViewCreateInfo& CreateInfo (VkImageViewCreateInfo& ci, VkImageAspectFlagBits aspect_flags, const Image& image);
    
    bool CreateImageView (VkImageView& imageview, const VkImageViewCreateInfo& ci, const VkDevice& device); 
    //bool CreateImageViews (std::vector<ImageView>& imageviews, const std::vector<Image>& images, const Device& device); 


    bool CreateImageViews (std::vector<rc::ImageView::Ref>&   imageviews,
                           const std::vector<rc::Image::Ref>& images, VkFormat format,
                           VkImageAspectFlagBits aspectflags, const Device& device);
    
    // ------------------------------------------------------------------
    bool CreateImage (Image& image, const VkDevice& device); 
    void Destroy (Image& image, const VkDevice& device);
    void Destroy (ImageView& image, const VkDevice& device);
    void Destroy (VkImageView& image, const VkDevice& device);
    // alloc old fashioned way

    // VkMemoryAllocateInfo& AllocInfo (VkMemoryAllocateInfo& alloc_info,  VkMemoryPropertyFlags prop_flags, const VkImage& image, const VkDevice& device, const VkPhysicalDevice& physdev); 
    // bool AllocateImageMemory (Image& image,  const VkDevice& device); 

    // CI for image from swapchain 
    VkImageCreateInfo& CreateInfo (VkImageCreateInfo& ci, const VkSwapchainCreateInfoKHR& swapchain_ci);
    // a 'generic' ci 
    VkImageCreateInfo& CreateInfo_2D (VkImageCreateInfo& ci, VkFormat format, VkImageUsageFlags usage_flags, 
                                      VkSampleCountFlagBits num_samples, uint32_t wd, uint32_t ht);

    // for color textures
    inline VkImageCreateInfo& CreateInfo_2D_color_sampling (VkImageCreateInfo& ci,
                                                            VkSampleCountFlagBits  num_samples,
                                                            uint32_t wd, uint32_t ht) {

      return CreateInfo_2D (ci, VK_FORMAT_B8G8R8A8_SRGB,  kSamplingUsage, num_samples, wd, ht); 
    }
  
    // 
    // depth attachments
    inline VkImageCreateInfo& CreateInfo_2D_depthstencil (VkImageCreateInfo& ci,
                                                          VkFormat format, 
                                                          VkSampleCountFlagBits num_samples,
                                                          uint32_t wd, uint32_t ht) {
      return CreateInfo_2D (ci, format, kDepthStencilUsage, num_samples, wd, ht); 
    }

    // for color attachments?
    inline VkImageCreateInfo& CreateInfo_2D_color_target (VkImageCreateInfo& ci,
                                                          VkFormat format, 
                                                          VkSampleCountFlagBits num_samples,
                                                          uint32_t wd, uint32_t ht) {
      return CreateInfo_2D (ci, format, kColorTargetUsage, num_samples, wd, ht); 
    }
                                                 


    
    struct mappedimage_cb {
      virtual int on_mapped  (void* mappedmemory, size_t maxsize, const VkExtent2D& ext2d) = 0;

      typedef std::shared_ptr<mappedimage_cb> Ref;
    };

    int TransferToDeviceImage (VkImage& dstb, size_t sizemem, VkFormat targetformat, 
                               const VkExtent2D& ext2d, mappedimage_cb*, const rokz::Device& device);

    inline int TransferToDeviceImage (VkImage& dstb, size_t sizemem, VkFormat targetformat,
                                      const VkExtent2D& ext2d, mappedimage_cb::Ref cb, const rokz::Device& device) {
      return TransferToDeviceImage (dstb, sizemem, targetformat, ext2d, cb.get (), device); 
    }


    // 
    // VMA -------------------------------------------------------------------------->
    bool                CreateImage   (Image& image, VmaAllocator const& allocator);
    void                Destroy       (Image& image, VmaAllocator allocator); 
    void                Destroy       (VkImage& image, VmaAllocation allocation, VmaAllocator allocator);
    // <---------------------------------------------------------------------------VMA
  } // cx




}

#endif
