
#include "frame.h"
#include "command.h"
#include "render.h"




using namespace rokz;

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool rc::PresentFrame (VkQueue present_que, const rc::Swapchain::Ref& swapchain, uint32_t& image_index, const FrameSync& render_sync) { 

  std::vector<VkSemaphore>     signal_sems = { render_sync.render_finished_sem };
  std::vector<VkSwapchainKHR>  swapchains = { swapchain->handle };

  VkPresentInfoKHR pi {};

 return cx::PresentFrame (present_que , cx::PresentInfo (pi, image_index, swapchains, signal_sems));
}




int rc::FrameDrawBegin (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer,
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



int rc::FrameDrawEnd (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index, const FrameSync& framesync, const Device& device) {

  vkCmdEndRendering (command_buffer);
  //
  if (VK_SUCCESS != vkEndCommandBuffer (command_buffer) != VK_SUCCESS) {
    printf ("[FAILED] record command buffer\n");
    return __LINE__; 
  }


  //rokz::Swapchain& swapchain = scg.swapchain;

  VkSubmitInfo submit_info {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  VkSemaphore wait_semaphores[]      = {framesync.image_available_sem};
  VkSemaphore signal_semaphores[]    = {framesync.render_finished_sem }; 
  VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

  submit_info.pWaitDstStageMask    = wait_stages;
  submit_info.waitSemaphoreCount   = 1;
  submit_info.pWaitSemaphores      = wait_semaphores;
  submit_info.signalSemaphoreCount = 1; 
  submit_info.pSignalSemaphores    = signal_semaphores; 
  submit_info.commandBufferCount   = 1;
  submit_info.pCommandBuffers      = &command_buffer; // &glob.command_buffer_group.buffers[curr_frame];

  if (vkQueueSubmit (device.queues.graphics, 1, &submit_info, framesync.in_flight_fen) != VK_SUCCESS) {
    printf("failed to submit draw command buffer!");
    return false; 
  }

  //  dynamic_rendering now, we have to manually transition
  cx::TransitionImageLayout (scg.images[image_index], scg.image_format,
                             VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 
                             VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                             device.queues.graphics, device.command_pool.handle, device.handle);

  
  if (!rc::PresentFrame ( device.queues.present, scg.swapchain, image_index, framesync)) {
    return __LINE__;
  }

  return 0;

}

