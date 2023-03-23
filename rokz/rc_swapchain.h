
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
    

    
    bool InitializeSwapchain (rc::SwapchainGroup& scg,
                              const rokz::SwapchainSupportInfo& swapchain_support_info,
                              const VkSurfaceKHR& surface,
                              const VkExtent2D&   extent, 
                              const rokz::PhysicalDevice& physdev,
                              const rokz::Device& device);

  /* bool RecreateSwapchain (Swapchain&                          swapchain, */
  /*                         Vec<rc::Image::Ref>&                 swapchain_images,  */
  /*                         //std::vector<Framebuffer>&           framebuffers, */
  /*                         Vec<rc::ImageView::Ref>&             image_views,  */

  /*                         //RenderPass&                         render_pass,  */
  /*                         Image&                              depth_image,  */
  /*                         ImageView&                          depth_imageview, */

  /*                         Image&                              multisamp_color_image,  */
  /*                         ImageView&                          multisamp_color_imageview, */

  /*                         const Device&                         device, */
  /*                         const VmaAllocator&                   allocator, */
  /*                         GLFWwindow*                           glfwin);  */
  }}

#endif
