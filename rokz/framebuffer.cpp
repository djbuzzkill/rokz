

#include "framebuffer.h"

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------xs
// bool rokz::CreateFramebuffers (
//     std::vector<VkFramebuffer>&           framebuffers,
//     std::vector<VkFramebufferCreateInfo>& create_infos,
//     const RenderPass&                     render_pass, 
//     const VkExtent2D                      swapchain_ext, 
//     const std::vector<VkImageView>&       image_views,
//     const VkImageView&                    msaa_color_imageview, 
//     const VkImageView&                    depth_imageview, 
//     const Device&                         device) {

//   printf ("[%s]\n", __FUNCTION__);
//   framebuffers.resize (image_views.size()); 
//   create_infos.resize (image_views.size()); 
  
//   for (size_t i = 0; i < image_views.size(); i++) {

//     VkImageView attachments[] = {msaa_color_imageview, depth_imageview, image_views[i] };

//     create_infos[i] = {}; 
//     create_infos[i].sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//     create_infos[i].renderPass = render_pass.handle;
//     create_infos[i].attachmentCount = 3; // color + depthstencil + colresolv
//     create_infos[i].pAttachments = attachments;
//     create_infos[i].width = swapchain_ext.width;
//     create_infos[i].height = swapchain_ext.height;
//     create_infos[i].layers = 1;

//     if (vkCreateFramebuffer(device.handle, &create_infos[i], nullptr, &framebuffers[i]) != VK_SUCCESS) {
//       printf ("[FAILED] %s create framebuffer\n", __FUNCTION__);
//       return false;
//     }
//   }

//   return true; 
// }



// bool rokz::CreateFramebuffers (
//     std::vector<VkFramebuffer>&           framebuffers,
//     std::vector<VkFramebufferCreateInfo>& create_infos,
//     const RenderPass&                     render_pass, 
//     const VkExtent2D                      swapchain_ext, 
//     const std::vector<VkImageView>&       image_views,
//     const VkImageView&                    msaa_color_imageview, 
//     const VkImageView&                    depth_imageview, 
//     const Device&                         device) {

//   printf ("[%s]\n", __FUNCTION__);
//   framebuffers.resize (image_views.size()); 
//   create_infos.resize (image_views.size()); 
  
//   for (size_t i = 0; i < image_views.size(); i++) {

//     VkImageView attachments[] = {msaa_color_imageview, depth_imageview, image_views[i] };

//     create_infos[i] = {}; 
//     create_infos[i].sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//     create_infos[i].renderPass = render_pass.handle;
//     create_infos[i].attachmentCount = 3; // color + depthstencil + colresolv
//     create_infos[i].pAttachments = attachments;
//     create_infos[i].width = swapchain_ext.width;
//     create_infos[i].height = swapchain_ext.height;
//     create_infos[i].layers = 1;

//     if (vkCreateFramebuffer(device.handle, &create_infos[i], nullptr, &framebuffers[i]) != VK_SUCCESS) {
//       printf ("[FAILED] %s create framebuffer\n", __FUNCTION__);
//       return false;
//     }
//   }

//   return true; 
// }

// rokz::CreateFramebuffers(std::vector<rokz::Framebuffer,std::allocator<rokz::Framebuffer> >&,
//                          std::vector<rokz::ImageView, std::allocator<rokz::ImageView> > const&,
//                          rokz::RenderPass const&,
//                          VkExtent2D,
//                          VkImageView_T* const&,
//                          VkImageView_T* const&,
//                          rokz::Device const&)'

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
    framebuffers[i].attachments.push_back (msaa_color_imageview);
    framebuffers[i].attachments.push_back (depth_imageview );
    framebuffers[i].attachments.push_back (imageviews[i].handle );
    
    framebuffers[i].ci = {}; 
    framebuffers[i].ci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffers[i].ci.renderPass = render_pass.handle;
    framebuffers[i].ci.attachmentCount = framebuffers[i].attachments.size(); // color + depthstencil + colresolv
    framebuffers[i].ci.pAttachments = &framebuffers[i].attachments[0]; // attachments;
    framebuffers[i].ci.width = swapchain_ext.width;
    framebuffers[i].ci.height = swapchain_ext.height;
    framebuffers[i].ci.layers = 1;

    if (vkCreateFramebuffer(device.handle, &framebuffers[i].ci, nullptr, &framebuffers[i].handle) != VK_SUCCESS) {
      printf ("[FAILED] %s create framebuffer\n", __FUNCTION__);
      return false;
    }
  }

  return true; 
}
