
#ifndef ROKZ_RC_SWAPCHAIN_INCLUDE
#define ROKZ_RC_SWAPCHAIN_INCLUDE

#include "common.h"
#include "rokz_types.h"
#include "rc_image.h"


namespace rokz { namespace rc { 
    // -----------------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------------
    struct SwapchainGroup {
      rokz::Swapchain         swapchain ;// = glob.swapchain; 
      Vec<rc::Image::Ref>     images    ;// = glob.swapchain_images; 
      Vec<rc::ImageView::Ref> imageviews;//= glob.swapchain_imageviews; 
    }; 
    
    // -----------------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------------
    bool InitializeSwapchain (rc::SwapchainGroup& scg,
                              const rokz::SwapchainSupportInfo& swapchain_support_info,
                              const VkSurfaceKHR& surface,
                              const VkExtent2D&   extent, 
                              const rokz::PhysicalDevice& physdev,
                              const rokz::Device& device);


  }}

#endif
