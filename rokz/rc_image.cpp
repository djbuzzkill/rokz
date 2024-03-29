
#include "rc_image.hpp"
#include "image.hpp"
#include "allocation.hpp"
#include "utility.hpp"


using namespace rokz;
// -----------------------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------------------
rc::Image::Ref rokz::rc::CreateImage (const VkImageCreateInfo& ci, const Device& device) {

  rc::Image::Ref img = std::make_shared<rc::Image> (device) ;

  if( VK_SUCCESS != vmaCreateImage (device.allocator.handle, &ci, &img->alloc_ci, &img->handle, &img->allocation, &img->alloc_info)) {
    printf ("[FAILED] %s vmaCreateImage\n", __FUNCTION__); 
    return rc::Image::Ref (nullptr); 
  }


  
  return img; 
}

// // -----------------------------------------------------------------------------------------------
// rc::Image::Ref CreateImage_2D_color_sampling (uint32 wd, uint32 ht,
//                                                         VkSampleCountFlagBits sampleflags,
//                                                         const Device& device) {
//   VkImageCreateInfo ci {};
//   cx::CreateInfo_2D_color_sampling  (ci, sampleflags, wd, ht);

//   rc::Image::Ref image = std::make_shared<rc::Image> (device) ;

//   rokz::cx::AllocCreateInfo_device (image->alloc_ci);

//   if( VK_SUCCESS != vmaCreateImage (device.allocator.handle, &ci, &image->alloc_ci,
//                                     &image->handle, &image->allocation, &image->alloc_info)) {
//     printf ("[FAILED] %s vmaCreateImage\n", __FUNCTION__); 
//     rc::Image::Ref (nullptr);
//   }
  
//   return image;
// }


// -----------------------------------------------------------------------------------------------
rc::Image::Ref rokz::rc::CreateImage_2D_color_sampling (uint32 wd, uint32 ht, VkFormat format, 
                                                        VkSampleCountFlagBits sampleflags,
                                                        const Device& device) {
  VkImageCreateInfo ci {};
  cx::CreateInfo_2D_color_sampling  (ci, format, sampleflags, wd, ht);

  rc::Image::Ref image = std::make_shared<rc::Image> (device) ;

  rokz::cx::AllocCreateInfo_device (image->alloc_ci);

  if( VK_SUCCESS != vmaCreateImage (device.allocator.handle, &ci, &image->alloc_ci,
                                    &image->handle, &image->allocation, &image->alloc_info)) {
    printf ("[FAILED] %s vmaCreateImage\n", __FUNCTION__); 
    rc::Image::Ref (nullptr);
  }
  
  return image;
}



// ------------------------------------------------------------------------------------------------
//                                 
// ------------------------------------------------------------------------------------------------
rc::ImageView::Ref rc::CreateImageView (VkImage image, VkFormat format, VkImageAspectFlagBits imageaspect, const Device& device) {
  VkImageViewCreateInfo ci {};
  cx::CreateInfo (ci, format, imageaspect, image);  

  rc::ImageView::Ref  ret = std::make_shared<rc::ImageView> (device); 

  if (!rokz::cx::CreateImageView (ret->handle, ci, device.handle)) { 
    HERE("FAILED CreateImageView");
    return rc::ImageView::Ref (nullptr); 
  }

  return ret;
}

// ------------------------------------------------------------------------------------------------
//                                 
// ------------------------------------------------------------------------------------------------
rc::ImageView::Ref rc::CreateImageView_2D_array (VkImage image, VkFormat format,
                                                  uint32 num_layers, const Device& device) {
  VkImageViewCreateInfo ci {};
  cx::CreateInfo_2D_array (ci, format, num_layers, image);  

  rc::ImageView::Ref  ret = std::make_shared<rc::ImageView> (device); 

  if (!rokz::cx::CreateImageView  (ret->handle, ci, device.handle)) { 
    HERE("FAILED CreateImageView");
    return rc::ImageView::Ref (nullptr); 
  }

  return ret;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
Vec<rc::ImageView::Ref>& rc::CreateImageViews (Vec<rc::ImageView::Ref>&    views, 
                                              const std::vector<VkImage>& images,
                                              VkFormat                    format, 
                                              VkImageAspectFlagBits       aspectflags,
                                              const Device&               device) {

  HERE("113"); 
  views.resize (images.size()); 
  
  for (size_t i = 0; i < images.size(); i++) {
    // CREATEINFO for imageviews from swapchain images
    //CreateInfo (imageviews[i].ci, VK_IMAGE_ASPECT_COLOR_BIT, images[i]); 
    VkImageViewCreateInfo ci {}; 
    rokz::cx::CreateInfo (ci, format, aspectflags, images[i] );
    views[i] = rc::CreateImageView (images[i], format, aspectflags, device);

    if ( !views[i] ) { //                                      if (!CreateImageView (imageviews[i].handle, imageviews[i].ci, device.handle)) {
       printf ("[FAILED] %s create imageview \n", __FUNCTION__); 
    }

  }

  printf ("BAI %s\n", __FUNCTION__); 
  return views;   
}


// -----------------------------------------------------------------------------------------------
rc::Image::~Image() {
  if (handle) rokz::cx::Destroy (handle, allocation, device.allocator.handle);
}

// -----------------------------------------------------------------------------------------------
rc::ImageView::~ImageView () {
  if (handle) rokz::cx::Destroy (handle, device.handle);
}
