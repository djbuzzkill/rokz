

#ifndef ROKZ_RENDER_H
#define ROKZ_RENDER_H


#include "common.hpp"
#include "rokz_types.hpp"
#include <vulkan/vulkan_core.h>

namespace rokz { namespace cx { 

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

  bool SetupDynamicRenderingInfo (rokz::RenderingInfoGroup& ri,
                                  const VkImageView&        msaa_color_imageview,
                                  const VkExtent2D&         image_extent); 

  void UpdateDynamicRenderingInfo (rokz::RenderingInfoGroup& ri, 
                                   const VkImageView&  msaa_color_imageview ,
                                   const VkImageView&  target_imageview);



} // rokz

#endif
