
#include "rc_types.h"
#include "allocation.h"
#include "buffer.h"



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

  vmaDestroyBuffer (device.allocator.handle, handle, allocation); 
  handle = VK_NULL_HANDLE; 

}




// -----------------------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------------------
rc::Image::Ref rokz::rc::CreateImage (const VkImageCreateInfo& ci, const Device& device) {

  std::shared_ptr<rc::Image> img = std::make_shared<rc::Image> (device) ;

  if( VK_SUCCESS != vmaCreateImage (device.allocator.handle, &ci, &img->alloc_ci, &img->handle, &img->allocation, &img->alloc_info)) {
    printf ("[FAILED] %s vmaCreateImage\n", __FUNCTION__); 
    return hresourc<VkImage>::Ref(nullptr); 
  }
  
  return img; 
}


// -----------------------------------------------------------------------------------------------
rc::Image::~Image() {

  vmaDestroyImage (device.allocator.handle, handle, allocation); 
  handle = VK_NULL_HANDLE;
  
}
