


#include "render.hpp"
#include "command.hpp"



using namespace rokz;
// ----------------------------------------------------------------------------------------------
//                                  
// ----------------------------------------------------------------------------------------------
VkResult cx::AcquireFrame (VkSwapchainKHR& swapchain, FrameSync& render_sync,
                           uint32_t& image_index, const Device&  device) {

  vkWaitForFences (device.handle, 1, &render_sync.in_flight_fence, VK_TRUE, UINT64_MAX);
    
  VkResult acquire_res = vkAcquireNextImageKHR (device.handle,
                                                swapchain,
                                                UINT64_MAX,
                                                render_sync.image_available_sem,
                                                VK_NULL_HANDLE,
                                                &image_index);

  vkResetFences (device.handle, 1, &render_sync.in_flight_fence);
  return acquire_res; 

}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
VkPresentInfoKHR& cx::PresentInfo (VkPresentInfoKHR& pi, uint32_t& image_index,
                                   const std::vector<VkSwapchainKHR>& swapchains,
                                   const std::vector<VkSemaphore>& signal_sems) { 

  //printf ("SIZE --> signal_sems[%zu]\n", signal_sems.size()); 
  pi.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  pi.pNext              = nullptr;

  pi.waitSemaphoreCount = signal_sems.size();  // = { render_sync.render_finished_sem };
  pi.pWaitSemaphores    = &signal_sems[0];
  pi.swapchainCount     = swapchains.size ();
  pi.pSwapchains        = &swapchains[0];
  pi.pImageIndices      = &image_index;
  pi.pResults           = nullptr;
  return pi;
}

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
VkRenderingAttachmentInfo& cx::AttachmentInfo (VkRenderingAttachmentInfo& ai,
                                               VkImageView                imageview ,
                                               VkImageLayout              image_layout,
                                               VkResolveModeFlagBits      resolve_mode,

                                               VkImageView                resolve_imageview,
                                               VkImageLayout              resolve_imagelayout ,

                                               VkAttachmentLoadOp         load_op,
                                               VkAttachmentStoreOp        store_op,
                                               VkClearValue               clear_value)
{ //
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

// ------------------------------------------------------------------------------------------
//                       
// ------------------------------------------------------------------------------------------
VkRenderingInfo& cx::RenderingInfo (VkRenderingInfo&                   ri,
                                 const VkRect2D&                       render_area,
                                 uint32_t                              layer_count,
                                 uint32_t                              view_mask,
                                 const Vec<VkRenderingAttachmentInfo>& color_attachments,
                                 const VkRenderingAttachmentInfoKHR*   depth_attachment,
                                 const VkRenderingAttachmentInfoKHR*   stencil_attachment)

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


// ------------------------------------------------------------------------------------------
//                       
// ------------------------------------------------------------------------------------------
void rokz::UpdateDynamicRenderingInfo (rokz::RenderingInfoGroup& ri,
                                       const VkImageView&        msaa_color_imageview ,
                                       const VkImageView&        target_imageview) {
  //printf ("%s\n", __FUNCTION__); 
  rokz::cx::AttachmentInfo (ri.color_attachment_infos[0],

                            msaa_color_imageview, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

                            VK_RESOLVE_MODE_AVERAGE_BIT,

                            target_imageview, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,

                            VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, ri.clear_colors[0]);
}

// -----------------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------------
bool rokz::SetupDynamicRenderingInfo (rokz::RenderingInfoGroup& ri,
                                      const VkImageView& msaa_color_imageview,
                                      const VkImageView& msaa_depth_imageview,
                                      const VkExtent2D&  image_extent) {

  ri.clear_colors.resize (1);
  ri.color_attachment_infos.resize (1);

  ri.clear_colors[0].color = {{0.01f, 0.01f, 0.02f, 1.0f}};
  //rig.clear_colors[1].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  ri.clear_depth.depthStencil = {1.0f, 0};

  rokz::cx::AttachmentInfo (ri.color_attachment_infos[0],
                            msaa_color_imageview,
                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                            VK_RESOLVE_MODE_AVERAGE_BIT,
                            nullptr, // dont know yet
                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                            VK_ATTACHMENT_STORE_OP_STORE,
                            ri.clear_colors[0]);

  rokz::cx::AttachmentInfo (ri.depth_attachment_info,
                            msaa_depth_imageview,
                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                            VK_RESOLVE_MODE_NONE,
                            nullptr,
                            VK_IMAGE_LAYOUT_UNDEFINED,
                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                            ri.clear_depth);

  ri.render_area = { VkOffset2D {0, 0}, image_extent };
  cx::RenderingInfo (ri.ri, ri.render_area, 1, 0, ri.color_attachment_infos, &ri.depth_attachment_info, nullptr);
  return true;
}

// ------------------------------------------------------------------------------------------
//                       
// ------------------------------------------------------------------------------------------
bool rokz::SetupDynamicRenderingInfo (rokz::RenderingInfoGroup& ri,
                                      const VkImageView& msaa_color_imageview,
                                      const VkExtent2D&  image_extent) {

  ri.clear_colors.resize (1);

  ri.color_attachment_infos.resize (1);

  ri.clear_colors[0].color = {{0.05f, 0.1f, 0.2f, 1.0f}};
  //rig.clear_colors[1].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  ri.clear_depth.depthStencil = {1.0f, 0};

  rokz::cx::AttachmentInfo (ri.color_attachment_infos[0],
                            msaa_color_imageview,
                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                            VK_RESOLVE_MODE_AVERAGE_BIT,
                            nullptr, // dont know yet
                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                            VK_ATTACHMENT_STORE_OP_STORE,
                            ri.clear_colors[0]);


  
  ri.render_area = { VkOffset2D {0, 0}, image_extent };

  cx::RenderingInfo (ri.ri, ri.render_area, 1, 0, ri.color_attachment_infos, nullptr, nullptr);
  return true;

}


