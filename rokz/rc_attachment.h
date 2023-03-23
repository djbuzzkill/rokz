
#ifndef ROKZ_RC_ATTACHMENT_INCLUDE
#define ROKZ_RC_ATTACHMENT_INCLUDE


#include "common.h"
#include "rokz_types.h"
#include "rc_types.h"



namespace rokz { namespace rc { 

    // ---------------------------------------------------------------------------------------------
    //                          
    // ---------------------------------------------------------------------------------------------
    bool CreateDepthBufferTarget (rc::Image::Ref&       depth_image,
                                  rc::ImageView::Ref&   depth_imageview,

                                  VkSampleCountFlagBits msaa_samples, 
                                  VkFormat              depth_format,
                                  const VkExtent2D&     ext,
                                  const Device&   device);


    rc::Image::Ref CreateMSAADepthImage  (VkSampleCountFlagBits msaa_samples, 
                                          VkFormat              depth_format,
                                          const VkExtent2D&     ext,
                                          const Device&   device);

    //rc::Image::Ref CreateDepthImage  (VkSampleCountFlagBits msaa_samples, 
                                      // VkFormat              depth_format,
                                      // const VkExtent2D&     ext,
                                      // const Device&   device);

    // ---------------------------------------------------------------------------------------------
    //                          
    // ---------------------------------------------------------------------------------------------
    rc::Image::Ref CreateMSAAColorImage (VkSampleCountFlagBits msaa_samples,
                                         VkFormat              image_format,
                                         const VkExtent2D&     ext,
                                         const Device&         device);

    bool CreateMSAAColorTarget  (rc::Image::Ref&       color_image, 
                                 rc::ImageView::Ref&   color_imageview, 
                                 VkSampleCountFlagBits msaa_samples,
                                 VkFormat              image_format,
                                 const VkExtent2D&     ext,
                                 const Device&         device);

    // ---------------------------------------------------------------------------------------------
    //                          
    // ---------------------------------------------------------------------------------------------
    bool SetupMSAARenderingAttachments (rc::Image::Ref&       msaa_color_image       ,
                                        rc::ImageView::Ref&   msaa_color_imageview   ,

                                        rc::Image::Ref&       msaa_depth_image       ,
                                        rc::ImageView::Ref&   msaa_depth_imageview   ,

                                        VkSampleCountFlagBits msaa_samples           ,
                                        VkFormat              swapchain_image_format ,
                                        VkFormat              msaa_depth_format      ,
                                        const VkExtent2D&     image_ext, 
                                        const Device& device);

  }}


#endif
