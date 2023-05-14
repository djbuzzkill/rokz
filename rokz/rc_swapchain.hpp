
#ifndef ROKZ_RC_SWAPCHAIN_INCLUDE
#define ROKZ_RC_SWAPCHAIN_INCLUDE

#include "common.hpp"
#include "rokz_types.hpp"
#include "rc_image.hpp"
#include <vulkan/vulkan_core.h>


namespace rokz { namespace rc { 

    //vkDestroySwapchainKHR ();

    struct Swapchain : public deviceob<VkSwapchainKHR, Swapchain> {

      Swapchain (const Device& d) : deviceob (d) {
      }

      Vec<uint32> family_indices; 

      virtual ~Swapchain () { if (handle) {
          vkDestroySwapchainKHR (device.handle,  handle, nullptr);
          handle = VK_NULL_HANDLE; 
        }}
    }; 

      
    Vec<VkImage>& GetSwapChainImages (Vec<VkImage>& oimages, const rc::Swapchain::Ref& swapchain, const Device& device); 

    // -----------------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------------
    struct SwapchainGroup {
      Swapchain::Ref          swapchain ;// = glob.swapchain;
      VkFormat                format;
      Vec<VkImage>            images    ;// = glob.swapchain_images; 
      Vec<rc::ImageView::Ref> views;//= glob.swapchain_imageviews; 
      VkExtent2D              extent; 
    }; 


    // struct SwapchainGroupA {
    //   rokz::Swapchain        swapchain    ;// = glob.swapchain; 
    //   Vec<rokz::Image>       images       ;// = glob.swapchain_images; 
    //   Vec<rc::ImageView>   imageviews   ;//= glob.swapchain_imageviews; 
    //   Vec<rc::Framebuffer> framebuffers ;//= glob.swapchain_framebuffers; 
    //   // VkCommandBufferAllocateInfo    command_buffer_alloc_info;
    // }; 
    
    // -----------------------------------------------------------------------------------------
    //
    // -----------------------------------------------------------------------------------------
    bool InitializeSwapchain (SwapchainGroup& scg,
                              const rokz::SwapchainInfo& swapchain_support_info,
                              const VkSurfaceKHR& surface,
                              const VkExtent2D&   extent, 
                              const rokz::PhysicalDevice& physdev,
                              const rokz::Device& device);

  }}

#endif
