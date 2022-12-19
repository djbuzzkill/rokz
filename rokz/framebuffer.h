//
#ifndef ROKZ_FRAMEBUFFER_H
#define ROKZ_FRAMEBUFFER_H


#include "common.h"
#include "shared_types.h"

namespace rokz {




  bool CreateFramebuffers (std::vector<VkFramebuffer>&           framebuffers,
                           std::vector<VkFramebufferCreateInfo>& create_infos,
                           const RenderPass&                     render_pass,
                           const VkExtent2D                      swapchain_ext,
                           const std::vector<VkImageView>&       image_views,
                           const VkImageView&                    msaa_color_imageview,
                           const VkImageView&                    depth_imageview,
                           const Device&                         device);

  // bool CreateFramebuffer (VkFramebuffer&           framebuffers,
  //                         VkFramebufferCreateInfo& create_infos,
  //                         const RenderPass&        render_pass, 
  //                         const VkExtent2D         swapchain_ext, 
  //                         VkImageView&             image_views, 
  //                         const VkImageView&       msaa_color_imageview, 
  //                         const VkImageView&       depth_imageview, 
  //                         const VkDevice&          device);

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


  
}



#endif
