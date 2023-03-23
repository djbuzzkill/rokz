
#ifndef ROKZ_RC_IMAGE_INCLUDE
#define ROKZ_RC_IMAGE_INCLUDE

#include "common.h"
#include "rokz_types.h"


namespace rokz { namespace rc { 

    // -----------------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------------
    struct Image:  public deviceob<VkImage, Image>  {

      Image (const Device& d) : deviceob (d), alloc_ci (), allocation (), alloc_info () {
      }

      virtual ~Image (); 
      
      // vmaCreateBuffer(glob.allocator, &buffer_info, &allocInfo, &buffer, &allocation, nullptr);
      VmaAllocationCreateInfo alloc_ci; 
      VmaAllocation           allocation;
      VmaAllocationInfo       alloc_info;
  
    }; 

    Image::Ref CreateImage (const VkImageCreateInfo& ci, const Device& device);

    Image::Ref CreateImage_2D_color_sampling (uint32 , uint32, VkSampleCountFlagBits sampleflags, const Device& device); 
    

    // -----------------------------------------------------------------------------------------
    struct ImageView:  public deviceob<VkImageView, ImageView>  {

      ImageView (const Device& d) : deviceob (d) {
      }
      // VkImageView             handle;
      // VkImageViewCreateInfo   ci; 
      virtual ~ImageView (); 
    };

    //    ImageView::Ref CreateImageView (const VkImageViewCreateInfo& ci, const VkDevice& device); 
    ImageView::Ref CreateImageView  (VkImage image, VkFormat format, VkImageAspectFlagBits imageaspect, const Device& device); 
    bool           CreateImageViews (std::vector<rc::ImageView::Ref>& imageviews, const Vec<VkImage>& images,
                                     VkFormat format, VkImageAspectFlagBits aspectflags, const Device& device); 

    
}}

#endif
