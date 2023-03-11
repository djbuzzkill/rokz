//
#ifndef ROKZ_ATTACHMENT_INLUCDE
#define ROKZ_ATTACHMENT_INLUCDE

#include "common.h"
#include "rokz_types.h"

namespace rokz {

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  bool CreateDepthBufferTarget (Image&          depth_image,
                                ImageView&      depth_imageview,
                                //rokz::SwapchainGroup& scg,
                                VkSampleCountFlagBits msaa_samples, 
                                VkFormat              depth_format,
                                const VkExtent2D&     ext,
                                const Device&   device);
  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------

  bool CreateMSAAColorTarget  (Image&                color_image, 
                               ImageView&            color_imageview, 
                               VkSampleCountFlagBits msaa_samples,
                               VkFormat              image_format,
                               const VkExtent2D&     ext,
                               const Device&         device);


}



#endif
