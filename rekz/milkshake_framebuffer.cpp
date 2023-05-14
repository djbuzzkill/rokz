
#include "milkshake.h"


using namespace rokz;
using namespace rekz; 
using namespace milkshake; 




// --------------------------------------------------------------------------------------------
//  only 1 geom framebuffer
// --------------------------------------------------------------------------------------------
bool setup_gbuff_framebuffer (Glob& glob) {
  
  // bool old_CreateFramebuffers (std::vector<Framebuffer>&       framebuffers, 
  //                                const std::vector<ImageView>&   imageviews,
  //                                const RenderPass&               render_pass, 
  //                                const VkExtent2D&               swapchain_ext, 
  //                                const VkImageView&              msaa_color_imageview, 
  //                                const VkImageView&              depth_imageview, 
  //                                const Device&                   device) {
  //   printf ("%s\n", __FUNCTION__);

  //   framebuffers.resize (imageviews.size()); 

  //   for (size_t i = 0; i < imageviews.size(); i++) {

  //     framebuffers[i].attachments.clear ();

  //     // does this match renderpass 
  //     framebuffers[i].attachments.push_back (msaa_color_imageview);
  //     framebuffers[i].attachments.push_back (depth_imageview );
  //     framebuffers[i].attachments.push_back (imageviews[i].handle);

  //     CreateInfo (framebuffers[i].ci, render_pass.handle, framebuffers[i].attachments, swapchain_ext); 
    
  //     if (vkCreateFramebuffer(device.handle, &framebuffers[i].ci, nullptr, &framebuffers[i].handle) != VK_SUCCESS) {
  //       printf ("[FAILED] %s create framebuffer\n", __FUNCTION__);
  //       return false;
  //     }

  //   }

  //   return true; 
  // }
  
  // glob.attachment.
  // do we need Nswapchainimages of framebuffers for geombuffer?
  // for (auto v : glob.swapchain_group.views) { 
  Vec<VkImageView> views = {
    glob.gbuff.attachment.position.view->handle, 
    glob.gbuff.attachment.normal.view->handle, 
    glob.gbuff.attachment.albedo.view->handle, 
    glob.gbuff.attachment.depth.view->handle, 
  }; 
  //   } 
    
  glob.gbuff.framebuffer =
    rc::CreateFramebuffer ( glob.gbuff.renderpass->handle, views, kDefaultDimensions, glob.device); 
  

  return false; 
}

// --------------------------------------------------------------------------------------------
//  lcomp has swapchain_images.size() framebuffers
// --------------------------------------------------------------------------------------------
bool setup_lcomp_framebuffers (Glob& glob) {


  glob.swapchain_group.images;
  glob.swapchain_group.views;
  
  // surface color target
  // depth lcmop target

  // bool rokz::CreateFramebuffers (std::vector<Framebuffer>&       framebuffers, 
  //                                const std::vector<ImageView>&   imageviews,
  //                                const RenderPass&               render_pass, 
  //                                const VkExtent2D&               swapchain_ext, 
  //                                const VkImageView&              msaa_color_imageview, 
  //                                const VkImageView&              depth_imageview, 
  //                                const Device&                   device) {
  //   printf ("%s\n", __FUNCTION__);

  //   framebuffers.resize (imageviews.size()); 

  //   for (size_t i = 0; i < imageviews.size(); i++) {

  //     framebuffers[i].attachments.clear ();

  //     // does this match renderpass 
  //     framebuffers[i].attachments.push_back (msaa_color_imageview);
  //     framebuffers[i].attachments.push_back (depth_imageview );
  //     framebuffers[i].attachments.push_back (imageviews[i].handle);

  //     CreateInfo (framebuffers[i].ci, swapchain_ext, render_pass, framebuffers[i].attachments); 
    
  //     if (vkCreateFramebuffer(device.handle, &framebuffers[i].ci, nullptr, &framebuffers[i].handle) != VK_SUCCESS) {
  //       printf ("[FAILED] %s create framebuffer\n", __FUNCTION__);
  //       return false;
  //     }

  //   }

  //   return true; 
  // }


  return false; 
}
