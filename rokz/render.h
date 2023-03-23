

#ifndef ROKZ_RENDER_H
#define ROKZ_RENDER_H


#include "common.h"
#include "rokz_types.h"
#include <vulkan/vulkan_core.h>

namespace rokz { 

  namespace cx { 
    // ----------------------------------------------------------------------------
    // 
    // ----------------------------------------------------------------------------
    VkPresentInfoKHR& PresentInfo  (VkPresentInfoKHR& pi, uint32_t& image_index, const std::vector<VkSwapchainKHR>& swapchains,  const std::vector<VkSemaphore>& signal_sems); 

    //VkResult          AcquireFrame (Swapchain& swapchain, FrameSync& render_sync, uint32_t& image_index, const Device&  device); 
    VkResult          AcquireFrame (VkSwapchainKHR& swapchain, FrameSync& render_sync, uint32_t& image_index, const Device&  device); 

    bool              PresentFrame (VkQueue present_que, const Swapchain& swapchain, uint32_t& image_index, const FrameSync& render_sync); 
    bool              PresentFrame (VkQueue present_que, const VkPresentInfoKHR& pi); 


    int               FrameDrawBegin (SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index, const VkRenderingInfo& ri, const Device& device);
    //int               FrameDrawBegin (SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index, const Device& device);
    int               FrameDrawEnd   (SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index, const FrameSync& framesync, const Device& device);

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

  } // cx


  // bool SetupDynamicRenderingInfo (rokz::RenderingInfoGroup& ri,
  //                                 const rokz::ImageView&    msaa_color_imageview ,
  //                                 const rokz::ImageView&    msaa_depth_imageview ,
  //                                 const VkExtent2D&         image_extent); 
  // void UpdateDynamicRenderingInfo (rokz::RenderingInfoGroup& ri,
  //                                  const rokz::ImageView&    msaa_color_imageview ,
  //                                  const rokz::ImageView&    target_imageview);


  bool SetupDynamicRenderingInfo (rokz::RenderingInfoGroup& ri,
                                  const VkImageView& msaa_color_imageview ,
                                  const VkImageView& msaa_depth_imageview ,
                                  const VkExtent2D&  image_extent);

  void UpdateDynamicRenderingInfo (rokz::RenderingInfoGroup& ri, 
                                   const VkImageView&  msaa_color_imageview ,
                                   const VkImageView&  target_imageview);



} // rokz

#endif
