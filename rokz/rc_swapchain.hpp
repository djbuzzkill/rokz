
#ifndef ROKZ_RC_SWAPCHAIN_INCLUDE
#define ROKZ_RC_SWAPCHAIN_INCLUDE

#include "common.hpp"
#include "rokz_types.hpp"
#include "rc_image.hpp"
#include <vulkan/vulkan_core.h>


namespace rokz { namespace rc { 

    
    //vkDestroySwapchainKHR ();

    struct Swapchain : public deviceob<VkSwapchainKHR, Swapchain> {

      Swapchain (const Device& d) : deviceob (d) {}

      Vec<uint32> family_indices; 

      virtual ~Swapchain () {
        if (handle) vkDestroySwapchainKHR (device.handle,  handle, nullptr);
      }
      
    };
    
    bool GetSwapChainImages (Vec<VkImage>& swapchain_images, const rc::Swapchain::Ref& swapchain, const Device& device); 

    // -----------------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------------
    struct SwapchainGroup {
      Swapchain::Ref          swapchain ;// = glob.swapchain;
      VkFormat                image_format;
      VkExtent2D              extent; 
      Vec<VkImage>            images    ;// = glob.swapchain_images; 
      Vec<rc::ImageView::Ref> imageviews;//= glob.swapchain_imageviews; 
    }; 
    
    // -----------------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------------
    bool InitializeSwapchain (SwapchainGroup& scg,
                              const rokz::SwapchainSupportInfo& swapchain_support_info,
                              const VkSurfaceKHR& surface,
                              const VkExtent2D&   extent, 
                              const rokz::PhysicalDevice& physdev,
                              const rokz::Device& device);

  }}

#endif
