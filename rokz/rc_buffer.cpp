

#include "rc_buffer.hpp"
#include "buffer.hpp"
#include "allocation.hpp"


using namespace rokz;


rc::Buffer::~Buffer () {
  if (handle) rokz::cx::Destroy (handle, allocation, device.allocator.handle);
}

// -----------------------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------------------

// rc::Buffer::Ref rokz::rc::Create_VB_device (const void* mem, size_t sz_mem, const rokz::Device& device) { 
//   printf ("%s", __FUNCTION__);

//   if (!mem) {
//     printf ("[FAILED] mem  is null");
//     return rc::Buffer::Ref (nullptr);
//   }

//   if (sz_mem == 0) {
//     printf ("sz_mem:%zu",  sz_mem);
//     return rc::Buffer::Ref (nullptr);
//   }
  
//   rc::Buffer::Ref vb = std::make_shared<rc::Buffer> (device); 

//   VkBufferCreateInfo ci {};
//   cx::CreateInfo_VB_device (ci, 1, sz_mem);
//   cx::AllocCreateInfo_device (vb->alloc_ci);
  
//   if (VK_SUCCESS != vmaCreateBuffer(device.allocator.handle, &ci, &vb->alloc_ci, &vb->handle, &vb->allocation, &vb->alloc_info)) {
//     printf ("[FAILED] %s vmaCreateBuffer", __FUNCTION__);
//     return rc::Buffer::Ref (nullptr);
//   }
  
//   printf ("%s", __FUNCTION__);
//   return vb;
// }

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
//
// -----------------------------------------------------------------------------------------------
rc::Buffer::Ref rokz::rc::CreateDeviceBuffer (size_t reqsize, VkBufferUsageFlags usage, const Device& device) {

  VkBufferCreateInfo ci {};
  rc::Buffer::Ref buf = std::make_shared<rc::Buffer>(device); 

  if (rokz::cx::CreateBuffer ( buf->handle,
                               buf->allocation,
                               buf->alloc_info,
                               cx::AllocCreateInfo_device (buf->alloc_ci),
                               cx::CreateInfo (ci, reqsize, usage),
                               device.allocator.handle))
    {
      HERE("success");


      return buf;
    }

  //  cx::CreateBuffer (buf->handle,   
  return rc::Buffer::Ref (nullptr); 
}
