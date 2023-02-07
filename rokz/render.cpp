


#include "render.h"

// --------------------------------------------------------------------
// nue
// --------------------------------------------------------------------
VkResult rokz::cx::AcquireFrame (Swapchain& swapchain, FrameSync& render_sync, uint32_t& image_index, const Device& device) {

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
VkPresentInfoKHR& rokz::cx::PresentInfo (VkPresentInfoKHR& pi, uint32_t& image_index,
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
bool rokz::cx::PresentFrame (VkQueue present_que, const VkPresentInfoKHR& pi) { 
  //rokz::cx::FrameGroup& frame_group = glob.frame_group;
 return VK_SUCCESS == vkQueuePresentKHR (present_que , &pi);
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool rokz::cx::PresentFrame (VkQueue present_que, const rokz::Swapchain& swapchain, uint32_t& image_index, const FrameSync& render_sync) { 

  std::vector<VkSemaphore>     signal_sems = { render_sync.render_finished_sem };
  std::vector<VkSwapchainKHR>  swapchains = { swapchain.handle };

 VkPresentInfoKHR pi {};

 return PresentFrame (present_que , PresentInfo (pi, image_index, swapchains, signal_sems));
}




// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
VkRenderingAttachmentInfo& rokz::cx::AttachmentInfo (VkRenderingAttachmentInfo& ai,

                                                 VkImageView                imageview ,
                                                 VkImageLayout              image_layout,
                                                 VkResolveModeFlagBits      resolve_mode,

                                                 VkImageView                resolve_imageview,
                                                 VkImageLayout              resolve_imagelayout ,

                                                 VkAttachmentLoadOp         load_op,
                                                 VkAttachmentStoreOp        store_op,
                                                 VkClearValue               clear_value)
{
  //
  ai.sType             = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
  ai.pNext             = nullptr;
  ai.imageView         = imageview          ;
  ai.imageLayout       = image_layout       ;
  ai.resolveMode       = resolve_mode       ;
  ai.resolveImageView  = resolve_imageview  ;
  ai.resolveImageLayout= resolve_imagelayout;
  ai.loadOp            = load_op            ;
  ai.storeOp           = store_op           ;
  ai.clearValue        = clear_value        ;

  return ai;
}

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
VkRenderingInfo& rokz::cx::RenderingInfo (VkRenderingInfo& ri,
                                 const VkRect2D&                               render_area,
                                 uint32_t                                      layer_count,
                                 uint32_t                                      view_mask,
                                 const std::vector<VkRenderingAttachmentInfo>& color_attachments,
                                 const VkRenderingAttachmentInfoKHR*           depth_attachment,
                                 const VkRenderingAttachmentInfoKHR*           stencil_attachment)

{

  printf ("%s  -> color_attachments_size[%zu]\n", __FUNCTION__,  color_attachments.size());
  
  ri.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO;
  ri.pNext                = nullptr;               
  ri.flags                = 0;               
  ri.renderArea           = render_area;          
  ri.layerCount           = layer_count;          
  ri.viewMask             = view_mask;            
  ri.colorAttachmentCount = color_attachments.size();
  ri.pColorAttachments    = &color_attachments[0];     
  ri.pDepthAttachment     = depth_attachment;    
  ri.pStencilAttachment   = stencil_attachment;
 
  return ri; 
}
