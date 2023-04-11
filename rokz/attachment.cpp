
#include "attachment.hpp"
#include "image.hpp"
#include "allocation.hpp"
#include "command.hpp"

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool rokz::CreateDepthBufferTarget (rokz::Image&          depth_image,
                                    rokz::ImageView&      depth_imageview,
                                    //rokz::SwapchainGroup& scg,
                                    VkSampleCountFlagBits msaa_samples, 
                                    VkFormat              depth_format,
                                    const VkExtent2D&     ext,
                                    const rokz::Device&   device)
{
  printf ("%s\n", __FUNCTION__); 

  //rokz::SwapchainGroup& scg = glob.swapchain_group;
  
  //
  // uint32_t wd = scg.swapchain.ci.imageExtent.width; 
  // uint32_t ht = scg.swapchain.ci.imageExtent.height;   

  rokz::cx::CreateInfo_2D_depthstencil (depth_image.ci,
                                        depth_format, 
                                        msaa_samples,
                                        ext.width, ext.height);

  rokz::cx::AllocCreateInfo_device (depth_image.alloc_ci); 
  rokz::cx::CreateImage (depth_image, device.allocator.handle);

  rokz::cx::CreateInfo (depth_imageview.ci, VK_IMAGE_ASPECT_DEPTH_BIT, depth_image); 
  rokz::cx::CreateImageView (depth_imageview.handle, depth_imageview.ci, device.handle);

  // manual transition b/c dynamic_rendering
  rokz::cx::TransitionImageLayout (depth_image.handle, depth_format,
                                   VK_IMAGE_LAYOUT_UNDEFINED,
                                   VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                   device.queues.graphics, device.command_pool.handle, device.handle);

  return true;
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool rokz::CreateMSAAColorTarget  (rokz::Image&          color_image, 
                             rokz::ImageView&      color_imageview, 
                             VkSampleCountFlagBits msaa_samples,
                             VkFormat              image_format,
                             const VkExtent2D&     ext,
                             const rokz::Device&   device) {
  printf ("%s\n", __FUNCTION__); 
  rokz::cx::CreateInfo_2D_color_target (color_image.ci, image_format, msaa_samples, ext.width, ext.height);

  rokz::cx::AllocCreateInfo_device (color_image.alloc_ci);
  rokz::cx::CreateImage (color_image, device.allocator.handle);

  // imageview 
  rokz::cx::CreateInfo (color_imageview.ci, VK_IMAGE_ASPECT_COLOR_BIT, color_image);
  rokz::cx::CreateImageView (color_imageview.handle, color_imageview.ci, device.handle);
  // dynamic_rendering
  rokz::cx::TransitionImageLayout (color_image.handle, image_format, VK_IMAGE_LAYOUT_UNDEFINED,
                                   VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, device.queues.graphics,
                                   device.command_pool.handle, device.handle);
  return true;
}



// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//bool SetupRenderingAttachments (Glob& glob, rokz::SwapchainGroup& scg, const rokz::Device& device) { 
bool rokz::SetupMSAARenderingAttachments (Image&          msaa_color_image       ,
                                          ImageView&      msaa_color_imageview   ,

                                          Image&          msaa_depth_image       ,
                                          ImageView&      msaa_depth_imageview   ,

                                          VkSampleCountFlagBits msaa_samples           ,
                                          VkFormat              swapchain_image_format ,
                                          VkFormat              msaa_depth_format      ,
                                          const VkExtent2D&     image_ext, 
                                          const Device& device) { 

  //CreateMSAAColorImage -> (image, imageview)
  rokz::CreateMSAAColorTarget (msaa_color_image, msaa_color_imageview, msaa_samples,
                               swapchain_image_format, image_ext, device);

  // CreateDepthBufferImage -> (image, imageview)
  rokz::CreateDepthBufferTarget (msaa_depth_image, msaa_depth_imageview, msaa_samples,
                                 msaa_depth_format, image_ext, device);

  return true;

}

