
#include "milkshake.h"


using namespace rokz;
using namespace rekz; 
using namespace milkshake; 


// --------------------------------------------------------------------------------------------
//  
// --------------------------------------------------------------------------------------------
bool setup_gbuff_attachments (Glob& glob) {

  HERE("HAI");
  const Device& device = glob.device; 
  const VkSampleCountFlagBits sample_bit_count = VK_SAMPLE_COUNT_1_BIT; 
  const VkImageUsageFlags color_target_usage   = rokz::kColorTargetUsage | VK_IMAGE_USAGE_SAMPLED_BIT; 
  const VkImageUsageFlags depth_target_usage   = rokz::kDepthStencilUsage | VK_IMAGE_USAGE_SAMPLED_BIT; 

  { rc::Attachment& position = glob.gbuff.attachment.position;
    VkImageCreateInfo ci {}; 
    cx::CreateInfo_2D (ci, VK_FORMAT_R16G16B16A16_SFLOAT, color_target_usage, sample_bit_count,
                       kDefaultDimensions.width, kDefaultDimensions.height);
    position.format = VK_FORMAT_R16G16B16A16_SFLOAT; 
    position.image  = rc::CreateImage (ci, device);
    position.view   = rc::CreateImageView (position.image->handle,
                                           position.format, VK_IMAGE_ASPECT_COLOR_BIT, glob.device); 
  }
  //
  { rc::Attachment& normal = glob.gbuff.attachment.normal;
    VkImageCreateInfo ci {}; 
    cx::CreateInfo_2D (ci, VK_FORMAT_R16G16B16A16_SFLOAT, color_target_usage, sample_bit_count,
                       kDefaultDimensions.width, kDefaultDimensions.height);
    normal.format  = VK_FORMAT_R16G16B16A16_SFLOAT;
    normal.image   = rc::CreateImage (ci, device);
    normal.view    = rc::CreateImageView (normal.image->handle, normal.format,
                                       VK_IMAGE_ASPECT_COLOR_BIT, glob.device); 
  }
  //
  { rc::Attachment& albedo  = glob.gbuff.attachment.albedo;
    VkImageCreateInfo ci {}; 
    cx::CreateInfo_2D (ci, VK_FORMAT_R16G16B16A16_SFLOAT, color_target_usage, sample_bit_count,
                       kDefaultDimensions.width, kDefaultDimensions.height);

     
    albedo.format = VK_FORMAT_R16G16B16A16_SFLOAT;
    albedo.image  = rc::CreateImage (ci, device);
    albedo.view  = rc::CreateImageView (albedo.image->handle, albedo.format,
                                        VK_IMAGE_ASPECT_COLOR_BIT, glob.device); 
  }

  //
  { rc::Attachment& depth  = glob.gbuff.attachment.depth; 

    VkImageCreateInfo ci {}; 
    cx::CreateInfo_2D (ci, glob.depth.format, depth_target_usage, sample_bit_count, 
                       kDefaultDimensions.width, kDefaultDimensions.height);

    depth.format = glob.depth.format;
    depth.image = rc::CreateImage (ci, device); 
    depth.view  = rc::CreateImageView (depth.image->handle, depth.format,
                                      VK_IMAGE_ASPECT_DEPTH_BIT, glob.device); 
  }

  HERE("BAI");
  return true; 
}

// --------------------------------------------------------------------------------------------
//  only 1 geom framebuffer
// --------------------------------------------------------------------------------------------
bool setup_lcomp_attachments  (Glob& glob) {

  HERE("HAI");
  const Device& device = glob.device; 
  const VkSampleCountFlagBits sample_bit_count = VK_SAMPLE_COUNT_1_BIT; 
  const VkImageUsageFlags depth_target_usage   = rokz::kDepthStencilUsage; 
  // only depth needs to be created, color target will use target from swapchain 
  { rc::Attachment& depth = glob.lcomp.attachment.depth; 
    VkImageCreateInfo ci {}; 
    cx::CreateInfo_2D (ci, glob.depth.format, depth_target_usage, sample_bit_count, 
                       kDefaultDimensions.width, kDefaultDimensions.height);

    depth.format = glob.depth.format; 
    depth.image = rc::CreateImage (ci, device); 
    depth.view  = rc::CreateImageView (depth.image->handle, depth.format,
                                       VK_IMAGE_ASPECT_DEPTH_BIT, device); 
  }
  HERE("BAI");
  return true; 
}
