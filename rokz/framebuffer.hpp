//
#ifndef ROKZ_FRAMEBUFFER_H
#define ROKZ_FRAMEBUFFER_H


#include "common.hpp"
#include "rokz_types.hpp"
#include "rc_types.hpp"
#include "rc_image.hpp"
#include <vulkan/vulkan_core.h>

namespace rokz {

  
  // VkFramebufferCreateInfo& CreateInfo (VkFramebufferCreateInfo& ci,
  //                                      const VkExtent2D&               swapchain_ext, 
  //                                      const RenderPass&               render_pass,
  //                                      const std::vector<VkImageView>& attachments); 

  VkFramebufferCreateInfo& CreateInfo (VkFramebufferCreateInfo&  ci,
                                       VkRenderPass             render_pass,
                                       const Vec<VkImageView>&   attachments, 
                                       const VkExtent2D&         swapchain_ext); 
  // ---------------------------------------------------------------------
  bool CreateFramebuffer (VkFramebuffer&           framebuffers,
                          VkFramebufferCreateInfo& create_infos,
                          const RenderPass&        render_pass, 
                          const VkExtent2D         swapchain_ext, 
                          VkImageView&             image_views, 
                          const VkImageView&       msaa_color_imageview, 
                          const VkImageView&       depth_imageview, 
                          const VkDevice&          device); 

  
  bool CreateFramebuffers (std::vector<Framebuffer>&       framebuffers, 
                           const std::vector<ImageView>&   imageviews,
                           const RenderPass&               render_pass, 
                           const VkExtent2D&               swapchain_ext, 
                           const VkImageView&              msaa_color_imageview, 
                           const VkImageView&              depth_imageview, 
                           const Device&                   device); 



  // -- -- 
  namespace rc { // 


    // Framebuffer::Ref CreateFramebuffer (const Vec<rc::ImageView::Ref>& views, const rc::RenderPass::Ref& renderpass,
    //                                     const VkExtent2D& ext2d, const Device& device); 

    Framebuffer::Ref CreateFramebuffer (VkRenderPass renderpass, const Vec<VkImageView>& views, 
                                        const VkExtent2D& ext2d, const Device& device); 


  }

  
}



#endif
