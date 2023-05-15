

#include "framebuffer.hpp"
#include "renderpass.hpp"
#include <vulkan/vulkan_core.h>



using namespace rokz; 
// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
VkFramebufferCreateInfo& rokz::CreateInfo (VkFramebufferCreateInfo& ci, VkRenderPass render_pass,
                                           const Vec<VkImageView>& attach_views, const VkExtent2D& ext)
{
    ci = {}; 
    ci.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    ci.flags           = 0; 
    ci.pNext           = nullptr;
    ci.renderPass      = render_pass;
    ci.attachmentCount = attach_views.size(); // color + depthstencil + colresolv
    ci.pAttachments    = &attach_views[0]; // attachments;
    ci.width           = ext.width;
    ci.height          = ext.height;
    ci.layers          = 1;

    return ci; 
}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
rc::Framebuffer::Ref rokz::rc::CreateFramebuffer (VkRenderPass renderpass, const Vec<VkImageView>& views, 
                                                  const VkExtent2D& ext2d, const Device& device) {
  //
  rc::Framebuffer::Ref fb = std::make_shared<rc::Framebuffer> (device);

  VkFramebufferCreateInfo ci {};
  CreateInfo (ci, renderpass, views, ext2d); 
    
  if (vkCreateFramebuffer(device.handle, &ci, nullptr, &fb->handle) != VK_SUCCESS) {
    printf ("[FAILED] %s create framebuffer\n", __FUNCTION__);
    fb.reset ();
  }
  
  return fb; 
}

// --------------------------------------------------------------------------------------------
// -- OLD -- 
// --------------------------------------------------------------------------------------------
bool old_CreateFramebuffers (std::vector<Framebuffer>&     framebuffers, 
                             const std::vector<ImageView>& imageviews,
                             const RenderPass&             render_pass, 
                             const VkExtent2D&             swapchain_ext, 
                             const VkImageView&            msaa_color_view, 
                             const VkImageView&            msaa_depth_view, 
                             const Device&                 device) {

  printf ("%s\n", __FUNCTION__);
  framebuffers.resize (imageviews.size()); 

  for (size_t i = 0; i < imageviews.size(); i++) {

    framebuffers[i].attachments.clear ();
    // -- match renderpass order
    framebuffers[i].attachments.push_back (msaa_color_view);      // 0 msaa color
    framebuffers[i].attachments.push_back (msaa_depth_view);      // 1 msaa depth 
    framebuffers[i].attachments.push_back (imageviews[i].handle); // 2 rendertarget

    CreateInfo (framebuffers[i].ci, render_pass.handle, framebuffers[i].attachments, swapchain_ext); 
    
    if (vkCreateFramebuffer(device.handle, &framebuffers[i].ci, nullptr, &framebuffers[i].handle) != VK_SUCCESS) {
      printf ("[FAILED] %s create framebuffer\n", __FUNCTION__);
      return false;
    }

  }

  return true; 
}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
// VkFramebufferCreateInfo& rokz::CreateInfo (VkFramebufferCreateInfo&        ci,
//                                            const VkExtent2D&               swapchain_ext, 
//                                            const RenderPass&               render_pass,
//                                            const std::vector<VkImageView>& attachments)
// {
//     ci = {}; 
//     ci.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//     ci.pNext           = nullptr;
//     ci.renderPass      = render_pass.handle;
//     ci.attachmentCount = attachments.size(); // color + depthstencil + colresolv
//     ci.pAttachments    = &attachments[0]; // attachments;
//     ci.width           = swapchain_ext.width;
//     ci.height          = swapchain_ext.height;
//     ci.layers          = 1;

//     return ci; 
// }
