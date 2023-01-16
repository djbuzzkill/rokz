

#ifndef ROKZ_RENDER_H
#define ROKZ_RENDER_H


#include "common.h"
#include "shared_types.h"

namespace rokz { 




  // ----------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------
  VkPresentInfoKHR& PresentInfo  (VkPresentInfoKHR& pi, uint32_t& image_index, const std::vector<VkSwapchainKHR>& swapchains,  const std::vector<VkSemaphore>& signal_sems); 

  VkResult          AcquireFrame (Swapchain& swapchain, RenderSync& render_sync, uint32_t& image_index, const Device&  device); 

  bool              PresentFrame (VkQueue& present_que, const Swapchain& swapchain, uint32_t& image_index, RenderSync& render_sync); 
  bool              PresentFrame (VkQueue& present_que, const VkPresentInfoKHR& pi); 


  // ----------------------------------------------------------------------------
  // dyanmic rendering | vk 1.3
  // ----------------------------------------------------------------------------
  VkRenderingAttachmentInfo& AttachmentInfo (VkRenderingAttachmentInfo& ai,
                                             VkImageView                image_view, 
                                             VkImageLayout              image_layout,

                                             VkResolveModeFlagBits      resolve_flags,
                                             VkImageView                resolve_imageview,
                                             VkImageLayout              resolve_image_layout,

                                             VkAttachmentLoadOp         attachment_load_op,
                                             VkAttachmentStoreOp        attachment_store_op,
                                             VkClearValue               clear_value);
  // dynamic rendering
  VkRenderingInfo& RenderingInfo (VkRenderingInfo& ri,
                                  const VkRect2D&                               render_area,
                                  uint32_t                                      layer_count,
                                  uint32_t                                      view_mask,
                                  const std::vector<VkRenderingAttachmentInfo>& color_attachments,
                                  const VkRenderingAttachmentInfoKHR*           depth_attachment,
                                  const VkRenderingAttachmentInfoKHR*           stencil_attachment); 




}

#endif
