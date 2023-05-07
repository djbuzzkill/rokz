
#include "frame.hpp"
#include "command.hpp"
#include "render.hpp"
#include <vulkan/vulkan_core.h>




using namespace rokz;
// ----------------------------------------------------------------------------------------------
// acquire swapchain image after | sit_on:in_flight , signal:image_available, 
// ----------------------------------------------------------------------------------------------
// VkResult cx::AcquireFrame (VkSwapchainKHR& swapchain, FrameSync& render_sync,
//                            uint32_t& image_index, const Device&  device) {

//   vkWaitForFences (device.handle, 1, &render_sync.in_flight_fence, VK_TRUE, UINT64_MAX);
    
//   VkResult acquire_res = vkAcquireNextImageKHR (device.handle,
//                                                 swapchain,
//                                                 UINT64_MAX,
//                                                 render_sync.image_available_sem,
//                                                 VK_NULL_HANDLE,
//                                                 &image_index);

//   vkResetFences (device.handle, 1, &render_sync.in_flight_fence);
//   return acquire_res; 

// }

// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
int cx::FrameDrawBegin (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer,
                              uint32_t image_index, const VkRenderingInfo& ri, const Device& device) {

  //rc::Swapchain::Ref& swapchain = scg.swapchain;
  
  // dynamic_rendering now, we have to manually transition
  cx::TransitionImageLayout (scg.images[image_index], scg.image_format,
                             VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
                             device.queues.graphics, device.command_pool.handle, device.handle);

  if (VK_SUCCESS != vkResetCommandBuffer (command_buffer, 0)) {  //   vkResetCommandBuffer (glob.command_buffer_group.buffers[curr_frame], 0);
    return __LINE__; 
  }

  VkCommandBufferBeginInfo begin_info {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.pNext = nullptr;
  begin_info.flags = 0;                  // 
  begin_info.pInheritanceInfo = nullptr; // 

  if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
     printf ("failed to begin recording command buffer!");
     return __LINE__; 
  }

  vkCmdBeginRendering (command_buffer, &ri);
  
  return 0;
}



// ------------------------------------------------------------------------------------------------
// wait_on:image_available, signal:render_finished, sit_on:in_flight
// ------------------------------------------------------------------------------------------------
// int cx::FrameDrawEnd (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index, const FrameSync& framesync, const Device& device) {

//   vkCmdEndRendering (command_buffer);
//   //
//   if (VK_SUCCESS != vkEndCommandBuffer (command_buffer) != VK_SUCCESS) {
//     printf ("[FAILED] record command buffer\n");
//     return __LINE__; 
//   }
//   //rokz::Swapchain& swapchain = scg.swapchain;
//   VkSubmitInfo submit_info {};
//   submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//   VkSemaphore wait_semaphores[]      = { framesync.image_available_sem };
//   VkSemaphore signal_semaphores[]    = { framesync.render_finished_sem }; 
//   VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

//   submit_info.pWaitDstStageMask    = wait_stages;

//   submit_info.waitSemaphoreCount   = 1;
//   submit_info.pWaitSemaphores      = wait_semaphores;

//   submit_info.signalSemaphoreCount = 1; 
//   submit_info.pSignalSemaphores    = signal_semaphores; 

//   submit_info.commandBufferCount   = 1;
//   submit_info.pCommandBuffers      = &command_buffer; // &glob.command_buffer_group.buffers[curr_frame];

//   if (vkQueueSubmit (device.queues.graphics, 1, &submit_info, framesync.in_flight_fence) != VK_SUCCESS) {
//     printf("failed to submit draw command buffer!");
//     return false; 
//   }
  
//   // dynamic_rendering now, we have to manually transition
//   cx::TransitionImageLayout (scg.images[image_index], scg.image_format,
//                              VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
//                              device.queues.graphics,
//                              device.command_pool.handle, device.handle);

//   if (!cx::PresentFrame ( device.queues.present, scg.swapchain, image_index, framesync)) {
//     return __LINE__;
//   }

//   return 0;

// }


// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
VkPresentInfoKHR& cx::PresentInfo (VkPresentInfoKHR& pi, uint32_t& image_index,
                                   const std::vector<VkSwapchainKHR>& swapchains,
                                   const std::vector<VkSemaphore>& wait_sems) { 
  //printf ("SIZE --> signal_sems[%zu]\n", signal_sems.size()); 
  pi.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  pi.pNext              = nullptr;
  pi.waitSemaphoreCount = wait_sems.size();  // wait for render finished 
  pi.pWaitSemaphores    = &wait_sems[0];
  pi.swapchainCount     = swapchains.size ();
  pi.pSwapchains        = &swapchains[0];
  pi.pImageIndices      = &image_index;
  pi.pResults           = nullptr;
  return pi;
}

// ------------------------------------------------------------------------------------------------
// wait_on:render_finished
// ------------------------------------------------------------------------------------------------
// bool cx::PresentFrame (VkQueue present_que, const rc::Swapchain::Ref& swapchain, uint32_t& image_index, const FrameSync& render_sync) { 

//   std::vector<VkSemaphore>     wait_sems = { render_sync.render_finished_sem };
//   std::vector<VkSwapchainKHR>  swapchains = { swapchain->handle };

//   VkPresentInfoKHR pi {};

//  return cx::PresentFrame (present_que , cx::PresentInfo (pi, image_index, swapchains, wait_sems));
// }


// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
bool cx::PresentFrame (VkQueue present_que, const VkPresentInfoKHR& pi) { 
  //rokz::cx::FrameGroup& frame_group = glob.frame_group;
 return VK_SUCCESS == vkQueuePresentKHR (present_que , &pi);
}


// ------------------------------------------------------------------------------------------------
// no_frame_sync
// ------------------------------------------------------------------------------------------------
VkResult cx::AcquireFrame (VkSwapchainKHR& swapchain, uint32_t& image_index,
                           VkFence fen_in_flight, VkSemaphore sem_image_available, const Device& device) {
  
  vkWaitForFences (device.handle, 1, &fen_in_flight, VK_TRUE, UINT64_MAX);
  
  VkResult acquire_res = vkAcquireNextImageKHR (device.handle,
                                                swapchain,
                                                UINT64_MAX,
                                                sem_image_available,
                                                VK_NULL_HANDLE,
                                                &image_index);
  //
  vkResetFences (device.handle, 1, &fen_in_flight);
  return acquire_res; 
}

// ------------------------------------------------------------------------------------------------
// no_frame_sync | trigger:fence_in_flight
// ------------------------------------------------------------------------------------------------
int cx::FrameDrawEnd (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer,
                      uint32_t image_index,

                      VkFence     fence_in_flight, 
                      VkSemaphore sem_image_available,
                      VkSemaphore sem_render_finished,
                      const Device& device) {

  vkCmdEndRendering (command_buffer);
  //
  if (VK_SUCCESS != vkEndCommandBuffer (command_buffer) != VK_SUCCESS) {
    printf ("[FAILED] record command buffer\n");
    return __LINE__; 
  }
  //rokz::Swapchain& swapchain = scg.swapchain;
  VkSubmitInfo submit_info {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

  Vec<VkSemaphore> waits    = { sem_image_available };
  Vec<VkSemaphore> signals  = { sem_render_finished }; 

  submit_info.pWaitDstStageMask    = wait_stages;

  submit_info.waitSemaphoreCount   = waits.size();
  submit_info.pWaitSemaphores      = &waits[0];

  submit_info.signalSemaphoreCount = signals.size(); 
  submit_info.pSignalSemaphores    = &signals[0]; 

  submit_info.commandBufferCount   = 1;
  submit_info.pCommandBuffers      = &command_buffer; // &glob.command_buffer_group.buffers[curr_frame];

  if (vkQueueSubmit (device.queues.graphics, 1, &submit_info, fence_in_flight) != VK_SUCCESS) {
    printf("failed to submit draw command buffer!");
    return false; 
  }
  
  // dynamic_rendering now, we have to manually transition
  cx::TransitionImageLayout (scg.images[image_index], scg.image_format,
                             VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                             device.queues.graphics,
                             device.command_pool.handle, device.handle);

  Vec<VkSemaphore> wait_render_finished = {
    sem_render_finished
  }; 

  if (!cx::PresentFrame ( device.queues.present, image_index, scg.swapchain, wait_render_finished)) {
    return __LINE__;
  }

  return 0;
}


// ------------------------------------------------------------------------------------------------
// no_frame_sync | wait_on:render_finished
// ------------------------------------------------------------------------------------------------
bool cx::PresentFrame (VkQueue present_que, uint32_t& image_index,
                       const rc::Swapchain::Ref& swapchain, const Vec<VkSemaphore>& waits) { 

  std::vector<VkSwapchainKHR>  swapchains = { swapchain->handle };

  VkPresentInfoKHR pi {};

 return cx::PresentFrame (present_que , cx::PresentInfo (pi, image_index, swapchains, waits));
}






// -- notes
//
//    +------>---+
//    |          |                            
//    ^          V                            
//    |      acquire_frame: signal:image_avaialble | wait_on:in_flight
//    |          |
//    |          V
//    |      draw_begin: nada 
//    ^          |
//    |          V
//    |      draw_end: wait:image_available | signal:render_finished |
//    |          |     signal_on:in_flight | wait_for:COLOR_ATTACHMENT_OUTPUT
//    |          |
//    ^          V
//    |     present_Frame: wait:render_finished | 
//    |          |                            
//    |          |                            
//    +------<---+
//
//
//                     +-----------<--------------<--------------+           
//                     |                                         |           
//                     V                                         ^           
//   acquire_frame: signal:image_avaialble | wait_on:in_flight   |           
//                     |                                         |           
//                     V                                         ^           
//   draw_geom: wait:image_available  | signal:geombuffer        |           
//                     |                                         |           
//                     V                                         |           
//   draw_light: wait:geombuffer | signal:lighting_finished      |           
//                     |                                         ^           
//                     V                                         |           
//   draw_end: wait:lighting_finished | signal:render_finished|  |
//                 wait_for:COLOR_ATTACHMENT_OUTPUT              |           
//                     |                                         |           
//                     V                                         ^           
//   present_Frame: wait:render_finished                         |           
//                     |                                         |           
//                     |                                         |           
//                     +--------------->------------>------------+           
//
