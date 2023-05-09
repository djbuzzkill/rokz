//
#ifndef ROKZ_FRAMEBUFFER_H
#define ROKZ_FRAMEBUFFER_H


#include "common.hpp"
#include "rokz_types.hpp"

namespace rokz {

  
  VkFramebufferCreateInfo& CreateInfo (VkFramebufferCreateInfo& ci,
                                       const VkExtent2D&               swapchain_ext, 
                                       const RenderPass&               render_pass,
                                       const std::vector<VkImageView>& attachments); 
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


  namespace rc {

    struct Framebuffer : public deviceob<VkFramebuffer, Framebuffer> { 
      Framebuffer (const Device& dev) : deviceob (dev) { 
      }
    
      //VkFramebufferCreateInfo  ci;
      std::vector<VkImageView> attachments; 
    };

    
  }

  
}



#endif
