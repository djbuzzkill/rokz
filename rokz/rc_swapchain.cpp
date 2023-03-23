

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

  scg.swapchain.family_indices.clear ();
  if (physdev.family_indices.graphics != physdev.family_indices.present) {
    scg.swapchain.family_indices.push_back (physdev.family_indices.graphics.value());
    scg.swapchain.family_indices.push_back (physdev.family_indices.present.value ());
  }
  else {
    scg.swapchain.family_indices.push_back (physdev.family_indices.graphics.value ());
  }
  
  rokz::cx::CreateInfo_default (scg.swapchain.ci,  
                                surface,
                                scg.swapchain.family_indices,
                                extent, 
                                swapchain_support_info);

  cx::CreateSwapchain ( scg.swapchain.handle, scg.swapchain.ci, device); 

  //scg.swapchain.ci.aspectFlags;
  const VkImageAspectFlagBits aspect = VK_IMAGE_ASPECT_COLOR_BIT; 
  rc::GetSwapChainImages (scg.images, scg.swapchain, device); 
  rc::CreateImageViews   (scg.imageviews, scg.images, scg.swapchain.ci.imageFormat, aspect, device); //  (std::vector<VkImageView>& swapchain_imageviews);

  for (size_t iimg = 0; iimg < scg.images.size (); ++iimg) {
    // manual transition b/c KHR_dynamic_rendering
    rokz::cx::TransitionImageLayout (scg.images[iimg]->handle, scg.swapchain.ci.imageFormat,
                                 VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                 device.queues.graphics, device.command_pool.handle, device.handle);
  }
  // end InitializeSwapchain
  return true;
}
