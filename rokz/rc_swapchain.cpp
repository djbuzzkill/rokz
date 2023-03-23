

#include "rc_swapchain.h"
#include "command.h"
#include "context.h"
#include <vulkan/vulkan_core.h>


using namespace rokz;




bool rc::InitializeSwapchain (rc::SwapchainGroup& scg,
                              const rokz::SwapchainSupportInfo& swapchain_support_info,
                              const VkSurfaceKHR& surface,
                              const VkExtent2D&   extent, 
                              const rokz::PhysicalDevice& physdev,
                              const rokz::Device& device) {

  scg.extent    = extent;
  scg.swapchain = std::make_shared<rc::Swapchain> (device); 

  scg.swapchain->family_indices.clear ();
  if (physdev.family_indices.graphics != physdev.family_indices.present) {
    scg.swapchain->family_indices.push_back (physdev.family_indices.graphics.value());
    scg.swapchain->family_indices.push_back (physdev.family_indices.present.value ());
  }
  else {
    scg.swapchain->family_indices.push_back (physdev.family_indices.graphics.value ());
  }
  
  VkSwapchainCreateInfoKHR ci {};

  rokz::cx::CreateInfo_default (ci,  
                                surface,
                                scg.swapchain->family_indices,
                                extent, 
                                swapchain_support_info);

  scg.image_format     = ci.imageFormat;
  cx::CreateSwapchain (scg.swapchain->handle, ci, device); 

  //scg.swapchain.ci.aspectFlags;
  const VkImageAspectFlagBits aspect = VK_IMAGE_ASPECT_COLOR_BIT; 
  rc::GetSwapChainImages (scg.images, scg.swapchain, device); 
  rc::CreateImageViews   (scg.imageviews, scg.images, scg.image_format, aspect, device); //  (std::vector<VkImageView>& swapchain_imageviews);

  for (size_t iimg = 0; iimg < scg.images.size (); ++iimg) {
    // manual transition b/c KHR_dynamic_rendering
    rokz::cx::TransitionImageLayout (scg.images[iimg], scg.image_format,
                                     VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                     device.queues.graphics, device.command_pool.handle, device.handle);
  }
  // end InitializeSwapchain
  return true;
}


// -----------------------------------------------------------------------------------------------
bool rc::GetSwapChainImages (Vec<VkImage> &swapchain_images, const rc::Swapchain::Ref& swapchain, const Device& device) {
  printf ("%s\n", __FUNCTION__);

  uint32_t image_count = 0; 

  VkResult res;
  res = vkGetSwapchainImagesKHR(device.handle, swapchain->handle, &image_count, nullptr);
  if (res != VK_SUCCESS) {
    printf ("LEAVING[FALSE] after image_count %s\n", __FUNCTION__);
    return false;
  }
  printf ( "no. swapchain images[%u]\n", image_count); 
  
  //std::vector<VkImage> vk_images(image_count);
  swapchain_images.resize(image_count);
  
  res = vkGetSwapchainImagesKHR (device.handle, swapchain->handle, &image_count, &swapchain_images[0]);
  if (res != VK_SUCCESS) {
    printf ("LEAVING[FALSE] after swapchain images %s\n", __FUNCTION__);
    return false;
  }

  return true;
}

