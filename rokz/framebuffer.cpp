

#include "framebuffer.hpp"

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkFramebufferCreateInfo& rokz::CreateInfo (VkFramebufferCreateInfo&        ci,
                                           const VkExtent2D&               swapchain_ext, 
                                           const RenderPass&               render_pass,
                                           const std::vector<VkImageView>& attachments)
{
    ci = {}; 
    ci.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    ci.pNext           = nullptr;
    ci.renderPass      = render_pass.handle;
    ci.attachmentCount = attachments.size(); // color + depthstencil + colresolv
    ci.pAttachments    = &attachments[0]; // attachments;
    ci.width           = swapchain_ext.width;
    ci.height          = swapchain_ext.height;
    ci.layers          = 1;

    return ci; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateFramebuffers (std::vector<Framebuffer>&       framebuffers, 
                               const std::vector<ImageView>&   imageviews,
                               const RenderPass&               render_pass, 
                               const VkExtent2D&               swapchain_ext, 
                               const VkImageView&              msaa_color_imageview, 
                               const VkImageView&              depth_imageview, 
                               const Device&                   device) {
  printf ("%s\n", __FUNCTION__);

  framebuffers.resize (imageviews.size()); 

  for (size_t i = 0; i < imageviews.size(); i++) {

    framebuffers[i].attachments.clear ();

    // does this match renderpass 
    framebuffers[i].attachments.push_back (msaa_color_imageview);
    framebuffers[i].attachments.push_back (depth_imageview );
    framebuffers[i].attachments.push_back (imageviews[i].handle);

    CreateInfo (framebuffers[i].ci, swapchain_ext, render_pass, framebuffers[i].attachments); 
    
    if (vkCreateFramebuffer(device.handle, &framebuffers[i].ci, nullptr, &framebuffers[i].handle) != VK_SUCCESS) {
      printf ("[FAILED] %s create framebuffer\n", __FUNCTION__);
      return false;
    }

  }

  return true; 
}
