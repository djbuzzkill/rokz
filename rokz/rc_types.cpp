
#include "rc_types.h"
#include "allocation.h"
#include "buffer.h"
#include "image.h"



using namespace rokz;


// -----------------------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------------------
rc::Buffer::Ref rokz::rc::Create_VB_device (const void* mem, size_t sz_mem, const rokz::Device& device) { 
  printf ("%s", __FUNCTION__);

  if (!mem) {
    printf ("[FAILED] mem  is null");
    return rc::Buffer::Ref (nullptr);
  }

  if (sz_mem == 0) {
    printf ("sz_mem:%zu",  sz_mem);
    return rc::Buffer::Ref (nullptr);
  }
  
  rc::Buffer::Ref vb = std::make_shared<rc::Buffer> (device); 

  VkBufferCreateInfo ci {};
  cx::CreateInfo_VB_device (ci, 1, sz_mem);
  cx::AllocCreateInfo_device (vb->alloc_ci);
  
  if (VK_SUCCESS != vmaCreateBuffer(device.allocator.handle, &ci, &vb->alloc_ci, &vb->handle, &vb->allocation, &vb->alloc_info)) {
    printf ("[FAILED] %s vmaCreateBuffer", __FUNCTION__);
    return rc::Buffer::Ref (nullptr);
  }
  
  printf ("%s", __FUNCTION__);
  return vb;
}

// -----------------------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------------------
rc::Buffer::Ref rc::Create_uniform_mapped  (size_t size_e, size_t num_e, const Device& device)  {

  rc::Buffer::Ref buf = std::make_shared<rc::Buffer> (device); 

  VkBufferCreateInfo ci {};
  cx::CreateInfo_uniform (ci, size_e,  num_e); 
  cx::AllocCreateInfo_mapped (buf->alloc_ci); 
  
  if (VK_SUCCESS != vmaCreateBuffer(device.allocator.handle, &ci, &buf->alloc_ci, &buf->handle, &buf->allocation, &buf->alloc_info)) {
    printf ("[FAILED] %s vmaCreateBuffer", __FUNCTION__);
    rc::Buffer::Ref (nullptr);
  }

  return buf;
}
// -----------------------------------------------------------------------------------------------
rc::Buffer::~Buffer () {
  rokz::cx::Destroy (handle, allocation, device.allocator.handle);
}




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
  rokz::cx::CreateInfo_2D_color_sampling  (ci, sampleflags, wd, ht);

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
rc::Image::~Image() {
  rokz::cx::Destroy (handle, allocation, device.allocator.handle);
}


rc::ImageView::~ImageView () {

  rokz::cx::Destroy (handle, device.handle);

}



rc::ImageView::Ref rc::CreateImageView (rc::Image::Ref image, VkFormat format, const Device& device) {


  VkImageViewCreateInfo ci;
  
  cx::CreateInfo (ci, format, VK_IMAGE_ASPECT_COLOR_BIT, image->handle);  
  
  rc::ImageView::Ref  ret = std::make_shared<rc::ImageView> (device); 

  if (!rokz::cx::CreateImageView (ret->handle, ci, device.handle)) { 
    printf (""); 
    return rc::ImageView::Ref (nullptr); 
  }

  return ret;
}

// ------------------------------------------------------------------------------------------------
//                                 
// ------------------------------------------------------------------------------------------------
// VkImageViewCreateInfo& rc::CreateInfo (VkImageViewCreateInfo& ci, VkFormat format, VkImageAspectFlagBits aspect_flags, const rc::Image::Ref image) {

//   // printf ("%s\n", __FUNCTION__);  

//   ci = {}; 
//   ci.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
//   ci.pNext = nullptr;
//   ci.image    = image->handle;
//   ci.viewType = VK_IMAGE_VIEW_TYPE_2D;
//   ci.format   = format; 
//   ci.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
//   ci.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
//   ci.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
//   ci.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

//   ci.subresourceRange.aspectMask = aspect_flags; 
//   ci.subresourceRange.baseMipLevel = 0;
//   ci.subresourceRange.levelCount = 1;
//   ci.subresourceRange.baseArrayLayer = 0;
//   ci.subresourceRange.layerCount = 1;
//   return ci; 
// }

