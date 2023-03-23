
#include "rc_attachment.h"
#include "image.h"
#include "allocation.h"
#include "command.h"
#include "rokz/rc_types.h"
#include <vulkan/vulkan_core.h>


using namespace rokz;




rc::Image::Ref rc::CreateMSAADepthImage  (VkSampleCountFlagBits msaa_samples, 
                                          VkFormat              depth_format,
                                          const VkExtent2D&     ext,
                                          const Device&   device) {
  VkImageCreateInfo ci {};
  
  rokz::cx::CreateInfo_2D_depthstencil (ci, depth_format, msaa_samples, ext.width, ext.height);

  rc::Image::Ref image = std::make_shared<rc::Image> (device);

  rokz::cx::AllocCreateInfo_device (image->alloc_ci);

  if( VK_SUCCESS != vmaCreateImage (device.allocator.handle, &ci, &image->alloc_ci,
                                    &image->handle, &image->allocation, &image->alloc_info)) {
    printf ("[FAILED] %s vmaCreateImage\n", __FUNCTION__); 
    rc::Image::Ref (nullptr);
  }


  return image; 
  
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool rokz::rc::CreateDepthBufferTarget (rc::Image::Ref&          depthimage,
                                        rc::ImageView::Ref&      depthimageview,
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
  depthimage = rc::CreateMSAADepthImage (msaa_samples, depth_format, ext, device);
  //rokz::cx::AllocCreateInfo_device (depthimage->alloc_ci); 
  //rokz::cx::CreateImage (depth_image, device.allocator.handle);

  depthimageview = rc::CreateImageView (depthimage, depth_format,  VK_IMAGE_ASPECT_DEPTH_BIT, device); 


  // rokz::cx::CreateInfo (depthimageviewci, VK_IMAGE_ASPECT_DEPTH_BIT, depth_image); 
  // rokz::cx::CreateImageView (depth_imageview.handle, depth_imageview.ci, device.handle);

  // manual transition b/c dynamic_rendering
  rokz::cx::TransitionImageLayout (depthimage->handle, depth_format, VK_IMAGE_LAYOUT_UNDEFINED,
                                   VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                   device.queues.graphics, device.command_pool.handle, device.handle);

  return true;
}


// ---------------------------------------------------------------------------------------------
//                          
// ---------------------------------------------------------------------------------------------
rc::Image::Ref rc::CreateMSAAColorImage (VkSampleCountFlagBits msaa_samples,
                                         VkFormat              image_format,
                                         const VkExtent2D&     ext,
                                         const Device&         device)
{

  VkImageCreateInfo ci {};
  
  rokz::cx::CreateInfo_2D_color_target (ci, image_format, msaa_samples, ext.width, ext.height);

  rc::Image::Ref image = std::make_shared<rc::Image> (device);

  rokz::cx::AllocCreateInfo_device (image->alloc_ci);

  if( VK_SUCCESS != vmaCreateImage (device.allocator.handle, &ci, &image->alloc_ci,
                                    &image->handle, &image->allocation, &image->alloc_info)) {
    printf ("[FAILED] %s vmaCreateImage\n", __FUNCTION__); 
    rc::Image::Ref (nullptr);
  }

  return image; 
}


// ---------------------------------------------------------------------------------------------
bool rc::CreateMSAAColorTarget (rc::Image::Ref&          colorimage, 
                                rc::ImageView::Ref&      colorimageview, 
                                VkSampleCountFlagBits msaa_samples,
                                VkFormat              image_format,
                                const VkExtent2D&     ext,
                                const rokz::Device&   device) {
  printf ("%s\n", __FUNCTION__); 
  //rokz::cx::CreateInfo_2D_color_target (color_image.ci, image_format, msaa_samples, ext.width, ext.height);
  // rokz::cx::AllocCreateInfo_device (color_image.alloc_ci);
  // rokz::cx::CreateImage (color_image, device.allocator.handle);

  colorimage = rc::CreateMSAAColorImage (msaa_samples, image_format,  ext, device);  
  
  // imageview 
  // rokz::cx::CreateInfo (color_imageview.ci, VK_IMAGE_ASPECT_COLOR_BIT, color_image);
  // rokz::cx::CreateImageView (color_imageview.handle, color_imageview.ci, device.handle);
  colorimageview = rc::CreateImageView (colorimage, image_format, VK_IMAGE_ASPECT_COLOR_BIT, device);

  // dynamic_rendering
  rokz::cx::TransitionImageLayout (colorimage->handle, image_format, VK_IMAGE_LAYOUT_UNDEFINED,
                                   VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, device.queues.graphics,
                                   device.command_pool.handle, device.handle);
  return true;
}



// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//bool SetupRenderingAttachments (Glob& glob, rokz::SwapchainGroup& scg, const rokz::Device& device) { 
bool rokz::rc::SetupMSAARenderingAttachments (rc::Image::Ref&       msaa_color_image       ,
                                              rc::ImageView::Ref&   msaa_color_imageview   ,

                                              rc::Image::Ref&       msaa_depth_image       ,
                                              rc::ImageView::Ref&   msaa_depth_imageview   ,

                                              VkSampleCountFlagBits msaa_samples           ,
                                              VkFormat              swapchain_image_format ,
                                              VkFormat              msaa_depth_format      ,
                                              const VkExtent2D&     image_ext, 
                                              const Device& device) { 

  //CreateMSAAColorImage -> (image, imageview)
  rc::CreateMSAAColorTarget (msaa_color_image, msaa_color_imageview, msaa_samples,
                               swapchain_image_format, image_ext, device);

  // CreateDepthBufferImage -> (image, imageview)
  rc::CreateDepthBufferTarget (msaa_depth_image, msaa_depth_imageview, msaa_samples,
                                 msaa_depth_format, image_ext, device);

  return true;

}

