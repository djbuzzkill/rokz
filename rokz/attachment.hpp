//
#ifndef ROKZ_ATTACHMENT_INLUCDE
#define ROKZ_ATTACHMENT_INLUCDE

#include "common.h"
#include "rokz_types.h"

namespace rokz {

  // ---------------------------------------------------------------------------------------------
  //                          
  // ---------------------------------------------------------------------------------------------
  bool CreateDepthBufferTarget (Image&          depth_image,
                                ImageView&      depth_imageview,
                                //rokz::SwapchainGroup& scg,
                                VkSampleCountFlagBits msaa_samples, 
                                VkFormat              depth_format,
                                const VkExtent2D&     ext,
                                const Device&   device);
  // ---------------------------------------------------------------------------------------------
  //                          
  // ---------------------------------------------------------------------------------------------

  bool CreateMSAAColorTarget  (Image&                color_image, 
                               ImageView&            color_imageview, 
                               VkSampleCountFlagBits msaa_samples,
                               VkFormat              image_format,
                               const VkExtent2D&     ext,
                               const Device&         device);

  // ---------------------------------------------------------------------------------------------
  //                          
  // ---------------------------------------------------------------------------------------------
  bool SetupMSAARenderingAttachments (Image&          msaa_color_image       ,
                                      ImageView&      msaa_color_imageview   ,

                                      Image&          msaa_depth_image       ,
                                      ImageView&      msaa_depth_imageview   ,

                                      VkSampleCountFlagBits msaa_samples           ,
                                      VkFormat              swapchain_image_format ,
                                      VkFormat              msaa_depth_format      ,
                                      const VkExtent2D&     image_ext, 
                                      const Device& device);

}



#endif
