


#include "render.h"

// --------------------------------------------------------------------
// 
// --------------------------------------------------------------------

#if false 

bool rokz::RenderFrame (Glob &glob, uint32_t curr_frame, std::vector<rokz::RenderSync>& syncs, bool& resize, double dt) {


  const VkDevice& device = glob.device.handle;


  vkWaitForFences(glob.device.handle, 1, &syncs[curr_frame].in_flight_fen, VK_TRUE, UINT64_MAX);



  
  uint32_t image_index = 0;
  VkResult acquire_res = vkAcquireNextImageKHR (glob.device.handle,
                                                glob.swapchain.handle,
                                                UINT64_MAX,
                                                syncs[curr_frame].image_available_sem,
                                                VK_NULL_HANDLE,
                                                &image_index);

  if (acquire_res == VK_ERROR_OUT_OF_DATE_KHR || acquire_res == VK_SUBOPTIMAL_KHR || resize) {
    resize = false; 
    return rokz::RecreateSwapchain (glob.swapchain, 
                                    glob.swapchain_images,

                                    glob.swapchain_framebuffers,
                                    glob.swapchain_imageviews,
                                    glob.render_pass, 

                                    glob.depth_image, glob.depth_imageview,

                                    glob.multisamp_color_image, glob.multisamp_color_imageview,
                                    // glob.surface,
                                    // glob.physical_device.handle,
                                    glob.device,
                                    glob.allocator,
                                    glob.window.glfw_window);
  } 
  else if (acquire_res != VK_SUCCESS) {
    printf("failed to acquire swap chain image!");
    return false;
  }

  vkResetFences (glob.device.handle, 1, &syncs[curr_frame].in_flight_fen);

  UpdateUniformBuffer (glob, curr_frame, dt); 
  
  vkResetCommandBuffer (glob.command_buffer_group.buffers[curr_frame], 0);

  rokz::RecordCommandBuffer_indexed (glob.command_buffer_group.buffers[curr_frame],
                                     glob.pipeline,
                                     glob.descr_group.desc_sets[curr_frame], 
                                     glob.vma_vb_device.handle, 
                                     glob.vma_ib_device.handle,
                                     glob.swapchain.ci.imageExtent,
                                     glob.swapchain_framebuffers[image_index].handle,
                                     glob.render_pass,
                                     glob.device.handle);

  
  VkSubmitInfo submit_info {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  VkSemaphore wait_semaphores[]      = {syncs[curr_frame].image_available_sem};
  VkSemaphore signal_semaphores[]    = {syncs[curr_frame].render_finished_sem }; 
  VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

  submit_info.pWaitDstStageMask    = wait_stages;

  submit_info.waitSemaphoreCount   = 1;
  submit_info.pWaitSemaphores      = wait_semaphores;
  submit_info.signalSemaphoreCount = 1; 
  submit_info.pSignalSemaphores    = signal_semaphores; 
  submit_info.commandBufferCount   = 1;
  submit_info.pCommandBuffers      = &glob.command_buffer_group.buffers[curr_frame];

  
  if (vkQueueSubmit (glob.queues.graphics, 1, &submit_info, syncs[curr_frame].in_flight_fen) != VK_SUCCESS) {
    printf("failed to submit draw command buffer!");
  }
  
 VkSwapchainKHR   swapchains[] = { glob.swapchain.handle };

 VkPresentInfoKHR present_info {};
 present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
 present_info.waitSemaphoreCount = 1;
 present_info.pWaitSemaphores = signal_semaphores;
 present_info.swapchainCount  = 1;
 present_info.pSwapchains     = swapchains;
 present_info.pImageIndices   = &image_index;
 present_info.pResults        = nullptr;

 vkQueuePresentKHR (glob.queues.present, &present_info);

 return true; 
}

#endif
