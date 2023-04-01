

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

    // ---------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------
    VkImageViewCreateInfo& CreateInfo (VkImageViewCreateInfo& ci, VkFormat format, VkImageAspectFlagBits aspect_flags, const VkImage& image);
    VkImageViewCreateInfo& CreateInfo (VkImageViewCreateInfo& ci, VkImageAspectFlagBits aspect_flags, const Image& image);

    VkImageViewCreateInfo& CreateInfo_2D_array  (VkImageViewCreateInfo& ci,
                                                 VkFormat               format,
                                                 rokz::uint32           layercount, 
                                                 const VkImage&         image) ; 

    
    bool CreateImageView (VkImageView& imageview, const VkImageViewCreateInfo& ci, const VkDevice& device); 
    //bool CreateImageViews (std::vector<ImageView>& imageviews, const std::vector<Image>& images, const Device& device); 
    bool CreateImageViews (std::vector<rc::ImageView::Ref>&   imageviews,
                           const std::vector<rc::Image::Ref>& images, VkFormat format,
                           VkImageAspectFlagBits aspectflags, const Device& device);
    



    // ---------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------
    VkImageCreateInfo& CreateInfo    (VkImageCreateInfo& ci, const VkSwapchainCreateInfoKHR& swapchain_ci);
    //  generic ci
    VkImageCreateInfo& CreateInfo_2D (VkImageCreateInfo& ci, VkFormat format, VkImageUsageFlags usage_flags, 
                                      VkSampleCountFlagBits num_samples, uint32_t wd, uint32_t ht);

    VkImageCreateInfo& CreateInfo_2D_array (VkImageCreateInfo&    ci,
                                            VkFormat              format,
                                            uint32                layercount, 
                                            VkImageUsageFlags     usage_flags, 
                                            uint32_t wd, uint32_t ht); 

    // ---------------------------------------------------------------------------------
    bool CreateImage (Image& image, const VkDevice& device); 
    void Destroy (Image& image, const VkDevice& device);
    void Destroy (ImageView& image, const VkDevice& device);
    void Destroy (VkImageView& image, const VkDevice& device);


    // for color textures
    // inline VkImageCreateInfo& CreateInfo_2D_color_sampling (VkImageCreateInfo& ci,
    //                                                         VkSampleCountFlagBits  num_samples,
    //                                                         uint32_t wd, uint32_t ht) {
    //   return CreateInfo_2D (ci, VK_FORMAT_B8G8R8A8_SRGB,  kSamplingUsage, num_samples, wd, ht); 
    // }

    // for color textures
    inline VkImageCreateInfo& CreateInfo_2D_color_sampling (VkImageCreateInfo& ci, VkFormat format, 
                                                            VkSampleCountFlagBits  num_samples,
                                                            uint32_t wd, uint32_t ht) {
      return CreateInfo_2D (ci, format,  kSamplingUsage, num_samples, wd, ht); 
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
      virtual int on_mapped  (void* mappedp, size_t maxsize, const VkExtent2D& ext2d) = 0;

      typedef std::shared_ptr<mappedimage_cb> Ref;
    };

    int TransferToDeviceImage (VkImage& dstb, size_t sizemem, VkFormat targetformat, 
                               const VkExtent2D& ext2d, mappedimage_cb*, const rokz::Device& device);

    inline int TransferToDeviceImage (VkImage& dstb, size_t sizemem, VkFormat targetformat,
                                      const VkExtent2D& ext2d, mappedimage_cb::Ref cb, const rokz::Device& device) {
      return TransferToDeviceImage (dstb, sizemem, targetformat, ext2d, cb.get (), device); 
    }



    // ----------------------------------------------------------------------------------------
    // mebe combine this w other Transfer fn
    // ----------------------------------------------------------------------------------------
    struct mappedlayer_cb {
      typedef std::shared_ptr<mappedlayer_cb> Ref;
      virtual int on_mapped (void* mappedp, size_t maxsize, uint32 layerindex, const VkExtent2D& ext) = 0; 
    };

    // ----------------------------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------------------------
    int TransferToImageLayer (VkImage& dsti, VkFormat format, size_t sizebytes,
                              uint32 numlayers, uint32 beginlayer, const VkExtent2D& ext2d,
                              mappedlayer_cb* cb, const rokz::Device& device); 

    inline int TransferToImageLayer (VkImage& dsti, VkFormat format, size_t sizebytes,
                                     uint32 numlayers, uint32 beginlayer, const VkExtent2D& ext2d,
                                     mappedlayer_cb::Ref cb, const rokz::Device& device) { 
      return TransferToImageLayer (dsti, format, sizebytes, numlayers, beginlayer,
                                   ext2d, cb.get (), device); 
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
