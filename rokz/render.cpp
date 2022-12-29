


#include "render.h"

// --------------------------------------------------------------------
// nue
// --------------------------------------------------------------------
VkResult rokz::AcquireFrame (Swapchain& swapchain, RenderSync& render_sync, uint32_t& image_index, const Device& device) {

  vkWaitForFences(device.handle, 1, &render_sync.in_flight_fen, VK_TRUE, UINT64_MAX);
    
  VkResult acquire_res = vkAcquireNextImageKHR (device.handle,
                                                swapchain.handle,
                                                UINT64_MAX,
                                                render_sync.image_available_sem,
                                                VK_NULL_HANDLE,
                                                &image_index);

  vkResetFences (device.handle, 1, &render_sync.in_flight_fen);
  return acquire_res; 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
VkPresentInfoKHR& rokz::PresentInfo (VkPresentInfoKHR& pi, uint32_t& image_index,
                                     const std::vector<VkSwapchainKHR>& swapchains,
                                     const std::vector<VkSemaphore>& signal_sems) { 
  //printf ("SIZE --> signal_sems[%zu]\n", signal_sems.size()); 
  pi.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  pi.pNext              = nullptr;

  pi.waitSemaphoreCount = signal_sems.size();
  pi.pWaitSemaphores    = &signal_sems[0];
  pi.swapchainCount     = swapchains.size ();
  pi.pSwapchains        = &swapchains[0];
  pi.pImageIndices      = &image_index;
  pi.pResults           = nullptr;
  return pi;
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool rokz::PresentFrame (VkQueue& present_que, const VkPresentInfoKHR& pi) { 
  //rokz::FrameGroup& frame_group = glob.frame_group;
 return VK_SUCCESS == vkQueuePresentKHR (present_que , &pi);
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool rokz::PresentFrame (VkQueue& present_que, const rokz::Swapchain& swapchain, uint32_t& image_index, rokz::RenderSync& render_sync) { 

  std::vector<VkSemaphore>     signal_sems = { render_sync.render_finished_sem };
  std::vector<VkSwapchainKHR>  swapchains = { swapchain.handle };

 VkPresentInfoKHR pi {};

 return PresentFrame (present_que , PresentInfo (pi, image_index, swapchains, signal_sems));
}

