
#include "frame.hpp"
#include "command.hpp"
#include "render.hpp"
#include <vulkan/vulkan_core.h>




using namespace rokz;
// ------------------------------------------------------------------------------------------------
// no_frame_sync
// ------------------------------------------------------------------------------------------------
VkResult cx::AcquireFrame (VkSwapchainKHR& swapchain, uint32_t& image_index,
                           VkFence in_flight, VkSemaphore image_available, const Device& device) {
  
  vkWaitForFences (device.handle, 1, &in_flight, VK_TRUE, UINT64_MAX);
  
  VkResult acquire_res = vkAcquireNextImageKHR (device.handle,
                                                swapchain,
                                                UINT64_MAX,
                                                image_available,
                                                VK_NULL_HANDLE,
                                                &image_index);
  //
  vkResetFences (device.handle, 1, &in_flight);
  return acquire_res; 
}
// ------------------------------------------------------------------------------------------------
// 
// ------------------------------------------------------------------------------------------------
void cx::BeginRenderPass (VkCommandBuffer         cb,
                         VkRenderPass             renderpass, 
                         VkFramebuffer            framebuffer, 
                         VkSubpassContents        contents,
                         const VkRect2D&          area, 
                         const Vec<VkClearValue>& clearvalues)
 {
   // [ https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkCmdBeginRenderPass2.html ]
   if (VK_SUCCESS != vkResetCommandBuffer (cb, 0)) {  //   vkResetCommandBuffer (glob.command_buffer_group.buffers[curr_frame], 0);
     HERE("FAILED -> vkResetCommandBuffer");
     return; 
   }

   VkCommandBufferBeginInfo begin_info {};
   begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
   begin_info.pNext = nullptr;
   begin_info.flags = 0;                  // 
   begin_info.pInheritanceInfo = nullptr; // 

   if (vkBeginCommandBuffer(cb, &begin_info) != VK_SUCCESS) {
     HERE ("FAILED -> vkBeginCommandBuffer");
     return; 
   }

      
   VkRenderPassBeginInfo rp {};
   rp.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
   rp.pNext           = nullptr;
   rp.renderPass      = renderpass;
   rp.framebuffer     = framebuffer; 
   rp.renderArea      = area; 
   //rp.renderArea      = VkRect2D {VkOffset2D{0, 0}, glob.swapchain_group.extent}; 
   rp.clearValueCount = (uint32_t)clearvalues.size ();
   rp.pClearValues    = &clearvalues[0]; 

   // contents : 
   vkCmdBeginRenderPass (cb, &rp, contents); 

}


// ------------------------------------------------------------------------------------------------
// 
// ------------------------------------------------------------------------------------------------
bool cx::EndRenderPass (VkCommandBuffer cb, VkQueue queue, VkSemaphore wait_sem,
                       VkSemaphore signal_sem, VkFence signal_fence)
{
    // [ https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkCmdEndRenderPass2.html ]
  vkCmdEndRenderPass (cb); 

  //
  if (VK_SUCCESS != vkEndCommandBuffer (cb) != VK_SUCCESS) {
    HERE("FAILED -> vkEndCommandBuffer");
    return false; 
  }

  VkQueue          graphic_queue; // = glob.device.queues.graphics
  VkFence          in_flight_fence; // = glob.framesyncgroup.syncs[curr_frame].in_flight_fence

  VkSemaphore      image_available_sem; // glob.framesyncgroup.syncs[curr_frame].image_available_sem
  VkSemaphore      render_finished_sem; //  glob.framesyncgroup.syncs[curr_frame].render_finished_sem

  {
    VkSubmitInfo submit_info {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = nullptr;
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    // VkCommandBuffer  command_buffers[] = {cb};
    // Vec<VkSemaphore> waits    = { image_available_sem };
    // Vec<VkSemaphore> signals  = { render_finished_sem }; 

    submit_info.pWaitDstStageMask    = wait_stages;

    submit_info.waitSemaphoreCount   = 1; // sem_image_available waits here 
    submit_info.pWaitSemaphores      = &wait_sem;

    submit_info.signalSemaphoreCount = 1;  // sem_render_finished signals here 
    submit_info.pSignalSemaphores    = &signal_sem; 

    submit_info.commandBufferCount   = 1;
    submit_info.pCommandBuffers      = &cb; // &glob.command_buffer_group.buffers[curr_frame];

    //
    if (vkQueueSubmit (queue, 1, &submit_info, signal_fence) != VK_SUCCESS) {
      HERE("FAILE -> vkQueueSubmit");
      return false; 
    }
  }

  return true;
}

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
//
// ------------------------------------------------------------------------------------------------
VkPresentInfoKHR& cx::PresentInfo (VkPresentInfoKHR& pi, uint32_t& image_index,
                                   const std::vector<VkSwapchainKHR>& swapchains, VkSemaphore wait) { 
  //printf ("SIZE --> signal_sems[%zu]\n", signal_sems.size()); 
  pi.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  pi.pNext              = nullptr;
  pi.waitSemaphoreCount = 1;  // wait for render finished 
  pi.pWaitSemaphores    = &wait;
  pi.swapchainCount     = swapchains.size ();
  pi.pSwapchains        = &swapchains[0];
  pi.pImageIndices      = &image_index;
  pi.pResults           = nullptr;
  return pi;
}


// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
bool cx::PresentFrame (VkQueue present_que, const VkPresentInfoKHR& pi) { 
  //rokz::cx::FrameGroup& frame_group = glob.frame_group;
 return VK_SUCCESS == vkQueuePresentKHR (present_que , &pi);
}

// ------------------------------------------------------------------------------------------------
// no_frame_sync | trigger:fence_in_flight
// ------------------------------------------------------------------------------------------------
int cx::FrameDrawingBegin (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer,
                              uint32_t image_index, const VkRenderingInfo& ri, const Device& device) {
  //rc::Swapchain::Ref& swapchain = scg.swapchain;
  // dynamic_rendering now, we have to manually transition
  cx::TransitionImageLayout (scg.images[image_index], scg.format,
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
//
// ------------------------------------------------------------------------------------------------
int cx::FrameDrawingEnd (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer,
                      uint32_t image_index,
                      VkSemaphore sem_image_available,
                      VkSemaphore sem_render_finished,
                      VkFence     in_flight, 
                      const Device& device) {

  // dynamic rendering, v1.3
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

  submit_info.waitSemaphoreCount   = waits.size(); // sem_image_available waits here 
  submit_info.pWaitSemaphores      = &waits[0];

  submit_info.signalSemaphoreCount = signals.size();  // sem_render_finished signals here 
  submit_info.pSignalSemaphores    = &signals[0]; 

  submit_info.commandBufferCount   = 1;
  submit_info.pCommandBuffers      = &command_buffer; // &glob.command_buffer_group.buffers[curr_frame];

  //
  //
  if (vkQueueSubmit (device.queues.graphics, 1, &submit_info, in_flight) != VK_SUCCESS) {
    printf("failed to submit draw command buffer!");
    return false; 
  }
  
  // dynamic_rendering now, we have to manually transition
  cx::TransitionImageLayout (scg.images[image_index], scg.format,
                             VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                             device.queues.graphics,
                             device.command_pool.handle, device.handle);


  Vec<VkSemaphore> wait_render_finished = {
    sem_render_finished  // sem_render_finished waits here 
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
  return cx::PresentFrame (present_que,
                           cx::PresentInfo (pi, image_index, swapchains, waits));
}



// ------------------------------------------------------------------------------------------------
// 
// ------------------------------------------------------------------------------------------------
void cx::NextSubpass (VkCommandBuffer cb, VkSubpassContents begin_contents) {

    // [ https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkCmdNextSubpass2KHR.html ]
    
    // [ https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkSubpassDescription2KHR.html ]
    VkSubpassBeginInfo subpbeg {};
    subpbeg.sType = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO;
    subpbeg.pNext = nullptr;
    subpbeg.contents = begin_contents; 

    VkSubpassEndInfo   subpend {};
    subpend.sType = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO; 
    subpend.pNext = nullptr; 
    
    vkCmdNextSubpass2 (cb, &subpbeg, &subpend); 
}




// bool cx::EndRenderPass (VkCommandBuffer cb) {

//   // [ https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/vkCmdEndRenderPass2.html ]
//   vkCmdEndRenderPass (cb); 

//   //
//   if (VK_SUCCESS != vkEndCommandBuffer (cb) != VK_SUCCESS) {
//     HERE("FAILED -> vkEndCommandBuffer");
//     return false; 
//   }

//   VkQueue          graphic_queue; // = glob.device.queues.graphics
//   VkFence          in_flight_fence; // = glob.framesyncgroup.syncs[curr_frame].in_flight_fence

//   VkSemaphore      image_available_sem; // glob.framesyncgroup.syncs[curr_frame].image_available_sem
//   VkSemaphore      render_finished_sem; //  glob.framesyncgroup.syncs[curr_frame].render_finished_sem

//   VkCommandBuffer  command_buffers[] = {cb};

//   {
//     VkSubmitInfo submit_info {};
//     submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//     submit_info.pNext = nullptr;

//     VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
//     Vec<VkSemaphore> waits    = { image_available_sem };
//     Vec<VkSemaphore> signals  = { render_finished_sem }; 

//     submit_info.pWaitDstStageMask    = wait_stages;

//     submit_info.waitSemaphoreCount   = waits.size(); // sem_image_available waits here 
//     submit_info.pWaitSemaphores      = &waits[0];

//     submit_info.signalSemaphoreCount = signals.size();  // sem_render_finished signals here 
//     submit_info.pSignalSemaphores    = &signals[0]; 

//     submit_info.commandBufferCount   = 1;
//     submit_info.pCommandBuffers      = command_buffers; // &glob.command_buffer_group.buffers[curr_frame];

//     //
//     if (vkQueueSubmit (graphic_queue, 1, &submit_info, 
//                        in_flight_fence) != VK_SUCCESS) {
//       HERE("FAILE -> vkQueueSubmit");
//       return false; 
//     }
//   }

//   return true;
// }



//
// -- notes -- 
//
//    +------>---+
//    |          |                            
//    ^          V                            
//    |      [acquire_frame] signal:image_avaialble | sit:in_flight
//    |          |
//    |          V
//    |      [draw_begin] nada 
//    ^          |
//    |          V
//    |      [draw_end] wait:image_available | {signal->wait}:render_finished | flag:in_flight
//    |          |     
//    |          |
//    ^          V
//    |     [present_Frame] wait:render_finished | 
//    |          |                            
//    |          |                            
//    +------<---+
//   
// -- multi pass --
//
//    +----->-----+    
//    |           |                                                              
//    ^           V                                                              
//    |   [acquire_frame] signal:image_avaialble | sit:in_flight               
//    |           |                                                              
//    ^           V                                                              
//    |   [draw_begin] no syncing done
//    |           |                                                   
//    |           V                                                   
//    ^   [draw_geom] wait:image_available  | signal:geompass                    
//    |           |                                                              
//    |           V                                                              
//    |   [draw_light] wait:geompass | signal:lightpass                  
//    ^           |                                                              
//    |           V                                                              
//    |   [draw_end] {signal->wait}:lightpass  | flag:in_flight 
//    |           |                                                              
//    ^           V                                                              
//    |   [present_Frame] wait:lightpass 
//    |           |                                                              
//    |           |                                                              
//    +-----<-----+
//

// -- multi pass --
//
//    +----->-----+    
//    |           |                                                              
//    ^           V                                                              
//    |   [acquire_frame] signal:image_avaialble | sit_on:in_flight               
//    |           |                                                              
//    ^           V                                                              
//    |   [draw_begin] no syncing done
//    |           |                                                   
//    |           V                                                   
//    ^   [draw_geom] wait:image_available  | signal:geompass                    
//    |           |                                                              
//    |           V                                                              
//    |   [draw_light] wait:geompass | signal:lightpass                  
//    ^           |  
//    |           V                                                              
//    |   [draw_end] {signal->wait}:lightpass  | signal:image_availble
//    |           |                                                              
//    ^           V                                                              
//    |   [present_Frame] wait:lightpass 
//    |           |                                                              
//    |           |                                                              
//    +-----<-----+
//
