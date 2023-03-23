
#include "rc_image.h"
#include "image.h"
#include "allocation.h"
#include "utility.h"


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

// -----------------------------------------------------------------------------------------------
rc::Image::Ref rokz::rc::CreateImage_2D_color_sampling (uint32 wd, uint32 ht,
                                                        VkSampleCountFlagBits sampleflags,
                                                        const Device& device) {
  VkImageCreateInfo ci {};
  cx::CreateInfo_2D_color_sampling  (ci, sampleflags, wd, ht);

  rc::Image::Ref image = std::make_shared<rc::Image> (device) ;

  rokz::cx::AllocCreateInfo_device (image->alloc_ci);

  if( VK_SUCCESS != vmaCreateImage (device.allocator.handle, &ci, &image->alloc_ci,
                                    &image->handle, &image->allocation, &image->alloc_info)) {
    printf ("[FAILED] %s vmaCreateImage\n", __FUNCTION__); 
    rc::Image::Ref (nullptr);
  }
  
  return image;
}

// -----------------------------------------------------------------------------------------------
bool rc::GetSwapChainImages (Vec<rc::Image::Ref> &swapchain_images,
                               const Swapchain& swapchain,
                               const Device& device) {
  printf ("%s\n", __FUNCTION__);

  uint32_t image_count = 0; 

  VkResult res;
  res = vkGetSwapchainImagesKHR(device.handle, swapchain.handle, &image_count, nullptr);
  if (res != VK_SUCCESS) {
    printf ("LEAVING[FALSE] after image_count %s\n", __FUNCTION__);
    return false;
  }
  printf ( "no. swapchain images[%u]\n", image_count); 
  
  std::vector<VkImage> vk_images(image_count);
  res = vkGetSwapchainImagesKHR (device.handle, swapchain.handle, &image_count, &vk_images[0]);
  if (res != VK_SUCCESS) {
    printf ("LEAVING[FALSE] after swapchain images %s\n", __FUNCTION__);
    return false;
  }

  swapchain_images.resize(image_count);
  for (size_t i = 0; i < image_count; ++i) {
    // Createinfo for image from swapchain
    // rokz::cx::CreateInfo (swapchain_images[i].ci, swapchain.ci); 
    // swapchain_images[i].handle = vk_images[i];
    swapchain_images[i] = std::make_shared<rc::Image> (device);
    swapchain_images[i]->handle = vk_images[i];
    
  }
  
  printf ("LEAVING[TRUE] %s\n", __FUNCTION__);
  return true;
}



// ------------------------------------------------------------------------------------------------
//                                 
// ------------------------------------------------------------------------------------------------
rc::ImageView::Ref rc::CreateImageView (rc::Image::Ref image, VkFormat format, VkImageAspectFlagBits imageaspect, const Device& device) {
  VkImageViewCreateInfo ci {};
  cx::CreateInfo (ci, format, imageaspect, image->handle);  

  rc::ImageView::Ref  ret = std::make_shared<rc::ImageView> (device); 

  if (!rokz::cx::CreateImageView (ret->handle, ci, device.handle)) { 
    HERE("FAILED CreateImageView");
    return rc::ImageView::Ref (nullptr); 
  }

  return ret;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rc::CreateImageViews (std::vector<rc::ImageView::Ref>&   imageviews, 
                           const std::vector<rc::Image::Ref>& images,
                           VkFormat                           format, 
                           VkImageAspectFlagBits              aspectflags,
                           const Device&                      device) {

  printf ("%s\n", __FUNCTION__); 


  imageviews.resize (images.size()); 
  
  for (size_t i = 0; i < images.size(); i++) {
    // CREATEINFO for imageviews from swapchain images
    //CreateInfo (imageviews[i].ci, VK_IMAGE_ASPECT_COLOR_BIT, images[i]); 

    VkImageViewCreateInfo ci {}; 
    rokz::cx::CreateInfo (ci, format, aspectflags, images[i]->handle);
    imageviews[i] = rc::CreateImageView (images[i], format, aspectflags, device);

    if ( !imageviews[i] ) { //                                      if (!CreateImageView (imageviews[i].handle, imageviews[i].ci, device.handle)) {
       printf ("[FAILED] %s create imageview \n", __FUNCTION__); 
    }

  }

  printf ("BAI %s\n", __FUNCTION__); 
  return true;   
}


// -----------------------------------------------------------------------------------------------
rc::Image::~Image() {
  rokz::cx::Destroy (handle, allocation, device.allocator.handle);
}

// -----------------------------------------------------------------------------------------------
rc::ImageView::~ImageView () {
  rokz::cx::Destroy (handle, device.handle);
}
