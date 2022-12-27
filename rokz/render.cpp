


#include "render.h"

// --------------------------------------------------------------------
// 
// --------------------------------------------------------------------

// --------------------------------------------------------------------
// nue
// --------------------------------------------------------------------
VkResult rokz::AcquireFrame (Swapchain& swapchain, RenderSync& render_sync, uint32_t& image_index, const Device&device) {

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

bool rokz::PresentFrame (VkQueue& present_que, const rokz::Swapchain& swapchain, uint32_t& image_index, rokz::RenderSync& render_sync) { 
  //rokz::FrameGroup& frame_group = glob.frame_group;
  
 VkSemaphore    signal_sems[] = { render_sync.render_finished_sem }; 
 VkSwapchainKHR swapchains[] = { swapchain.handle };

 VkPresentInfoKHR present_info {};
 present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
 present_info.waitSemaphoreCount = 1;
 present_info.pWaitSemaphores = signal_sems;
 present_info.swapchainCount  = 1;
 present_info.pSwapchains     = swapchains;
 present_info.pImageIndices   = &image_index;
 present_info.pResults        = nullptr;

 return VK_SUCCESS == vkQueuePresentKHR (present_que , &present_info);
}


