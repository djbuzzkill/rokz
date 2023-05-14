
#include "milkshake.h"


using namespace rokz;
using namespace rekz; 
using namespace milkshake; 


// --------------------------------------------------------------------------------------------
//  
// --------------------------------------------------------------------------------------------
bool setup_gbuff_attachments (Glob& glob) {

  const Device& device = glob.device; 
  const VkSampleCountFlagBits sample_bit_count = VK_SAMPLE_COUNT_1_BIT; 
  const VkImageUsageFlags color_target_usage   = rokz::kColorTargetUsage | VK_IMAGE_USAGE_SAMPLED_BIT; 
  const VkImageUsageFlags depth_target_usage   = rokz::kDepthStencilUsage | VK_IMAGE_USAGE_SAMPLED_BIT; 

  glob.msaa_samples = sample_bit_count; 
  
  //Vec<rc::Attachment>& colorattach = glob.gbuff.attachment.color; 
  //rc::Attachment&      depthattach = glob.attachment.depth;

  { // position
    VkImageCreateInfo ci {}; 

    cx::CreateInfo_2D (ci, VK_FORMAT_R16G16B16A16_SFLOAT, color_target_usage, sample_bit_count,
                       kDefaultDimensions.width, kDefaultDimensions.height);

    glob.gbuff.attachment.position.format = VK_FORMAT_R16G16B16A16_SFLOAT; 
    glob.gbuff.attachment.position.image  = rc::CreateImage (ci, device);
    glob.gbuff.attachment.position.view   =
      rc::CreateImageView (glob.gbuff.attachment.position.image->handle,
                           glob.gbuff.attachment.position.format,  VK_IMAGE_ASPECT_COLOR_BIT, glob.device); 
    // create imageview...
    assert (false); 
  }

  { // normal
    VkImageCreateInfo ci {}; 
    cx::CreateInfo_2D (ci, VK_FORMAT_R16G16B16A16_SFLOAT, color_target_usage, sample_bit_count,
                       kDefaultDimensions.width, kDefaultDimensions.height);
    glob.gbuff.attachment.normal.format  =  VK_FORMAT_R16G16B16A16_SFLOAT;
    glob.gbuff.attachment.normal.image =  rc::CreateImage (ci, device);
    glob.gbuff.attachment.normal.view =   
        rc::CreateImageView (glob.gbuff.attachment.normal.image->handle,
                             glob.gbuff.attachment.normal.format, VK_IMAGE_ASPECT_COLOR_BIT, glob.device); 
       // create imageview...
    assert (false); 
  }


  { // albedo
    VkImageCreateInfo ci {}; 
    cx::CreateInfo_2D (ci, VK_FORMAT_R16G16B16A16_SFLOAT, color_target_usage, sample_bit_count,
                       kDefaultDimensions.width, kDefaultDimensions.height);

    glob.gbuff.attachment.albedo.format = VK_FORMAT_R16G16B16A16_SFLOAT;
    glob.gbuff.attachment.albedo.image  = rc::CreateImage (ci, device);
    glob.gbuff.attachment.albedo.view  =  
        rc::CreateImageView (glob.gbuff.attachment.albedo.image->handle,
                             glob.gbuff.attachment.albedo.format, VK_IMAGE_ASPECT_COLOR_BIT, glob.device); 

    // create imageview...
    assert (false); 
  }

  { // depth 
    VkImageCreateInfo ci {}; 
    rokz::ut::FindDepthFormat (glob.depth.format, glob.device.physical.handle);
  
    cx::CreateInfo_2D (ci, glob.depth.format, depth_target_usage, sample_bit_count, 
                       kDefaultDimensions.width, kDefaultDimensions.height);

    //glob.msaa_samples rokz::ut::MaxUsableSampleCount (glob.device.physical); 
    glob.gbuff.attachment.depth.image = rc::CreateImage (ci, device); 
    glob.gbuff.attachment.depth.view = 
      rc::CreateImageView (glob.gbuff.attachment.normal.image->handle,
                           glob.depth.format, VK_IMAGE_ASPECT_DEPTH_BIT, glob.device); 

    // create imageview...
    assert (false); 
  }

  return true; 
}

// --------------------------------------------------------------------------------------------
//  only 1 geom framebuffer
// --------------------------------------------------------------------------------------------
bool setup_lcomp_attachments  (Glob& glob) {


  const Device& device = glob.device; 
  const VkSampleCountFlagBits sample_bit_count = VK_SAMPLE_COUNT_1_BIT; 
  const VkImageUsageFlags depth_target_usage   = rokz::kDepthStencilUsage; 

  { // depth 
    VkImageCreateInfo ci {}; 
    rokz::ut::FindDepthFormat (glob.depth.format, glob.device.physical.handle);
  
    cx::CreateInfo_2D (ci, glob.depth.format, depth_target_usage, sample_bit_count, 
                       kDefaultDimensions.width, kDefaultDimensions.height);

    glob.lcomp.attachment.depth.image = rc::CreateImage (ci, device); 
    glob.lcomp.attachment.depth.view =
      rc::CreateImageView (glob.gbuff.attachment.normal.image->handle,
                           glob.depth.format, VK_IMAGE_ASPECT_DEPTH_BIT, device); 

    // create imageview...
    assert (false); 
  }
  return true; 
}
