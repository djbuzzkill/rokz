
#include "buffer.h"
#include "allocation.h"
#include <vulkan/vulkan_core.h>





// ---------------------------------------------------------------------
// VMA
// ---------------------------------------------------------------------
bool rokz::cx::CreateBuffer (rokz::Buffer& buffer, VmaAllocator const& allocator) {

  if (VK_SUCCESS != vmaCreateBuffer(allocator, &buffer.ci, &buffer.alloc_ci, &buffer.handle, &buffer.allocation, &buffer.alloc_info)) {

    printf ("[FAILED] %s vmaCreateBuffer", __FUNCTION__);
    return false; 
  }
  
  return true; 
}


// --------------------------------------------------------------------------------------------
//                         
// --------------------------------------------------------------------------------------------
bool rokz::cx::CreateBuffer (VkBuffer&                      buffer,
                             VmaAllocation&                 allocation,
                             VmaAllocationInfo&             alloc_info,
                             const VmaAllocationCreateInfo& alloc_ci,
                             const VkBufferCreateInfo&      ci,
                             VmaAllocator const&            allocator) {

  if (VK_SUCCESS != vmaCreateBuffer(allocator, &ci, &alloc_ci, &buffer, &allocation, &alloc_info)) {

    printf ("[FAILED] %s vmaCreateBuffer", __FUNCTION__);
    return false; 
  }
  
  return true; 
}



// --------------------------------------------------------------------------------------------
//                         
// --------------------------------------------------------------------------------------------
VmaAllocationCreateInfo& rokz::cx::CreateInfo_default (VmaAllocationCreateInfo& ci) {


  ci = {};
  ci.pool = VK_NULL_HANDLE;
  ci.flags = 0;
  ci.usage = VMA_MEMORY_USAGE_AUTO;
  ci.priority = 1.0f;
  ci.pUserData =  nullptr;
  ci.requiredFlags = 0;
  ci.preferredFlags = 0;
  ci.memoryTypeBits = 0;
  
  return ci;
}

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
bool rokz::cx::CreateBuffer_aligned (Buffer& buffer, VkDeviceSize min_align, VmaAllocator const& allocator) {

  if (VK_SUCCESS != vmaCreateBufferWithAlignment (allocator, &buffer.ci, &buffer.alloc_ci, min_align, &buffer.handle, &buffer.allocation, &buffer.alloc_info)) {

    printf ("[FAILED] %s vmaCreateBuffer", __FUNCTION__);
    return false; 
  }
  
  return true; 
}


// ---------------------------------------------------------------------
// transfer buffer
// ---------------------------------------------------------------------
VkBufferCreateInfo& rokz::cx::CreateInfo_VB_stage (VkBufferCreateInfo& ci, uint32_t sizeOf_vert, uint32_t numv) {
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  ci.pNext = nullptr;
  ci.size  = sizeOf_vert * numv; 
  ci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT; 
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  return ci;
}

VkBufferCreateInfo& rokz::cx::CreateInfo_VB_device (VkBufferCreateInfo& ci, uint32_t sizeOf_vert, uint32_t numv) {
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  ci.pNext = nullptr;
  ci.size  = sizeOf_vert * numv; 
  ci.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  return ci;
}

// ---------------------------------------------------------------------
// 4 transfer
// ---------------------------------------------------------------------
VkBufferCreateInfo& rokz::cx::CreateInfo_buffer_stage (VkBufferCreateInfo& ci, uint32_t sizebytes) {
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO; 
  ci.pNext = nullptr;
  ci.size = sizebytes;
  ci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT; 
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  //memcpy(allocInfo.pMappedData, myData, myDataSize);
  return ci;
}

// ---------------------------------------------------------------------
// VB in device 
// ---------------------------------------------------------------------
VkBufferCreateInfo& CreateInfo_VB_device (VkBufferCreateInfo& ci, uint32_t sizebytes) {

  ci = { };
  ci.sType =  VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO; 
  ci.pNext = nullptr;
  ci.size = sizebytes;
  ci.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT; 
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  //memcpy(allocInfo.pMappedData, myData, myDataSize);
  return ci;
}


// ---------------------------------------------------------------------
// transfer
// ---------------------------------------------------------------------
VkBufferCreateInfo& rokz::cx::CreateInfo_IB_16_stage (VkBufferCreateInfo& ci, uint32_t num_elem) {

  const size_t sizeof_index = sizeof (uint16_t);

  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  ci.pNext = nullptr;
  ci.size  = sizeof_index * num_elem; 
  ci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  return ci; 
}


// -------------------------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------------------------
VkBufferCreateInfo& rokz::cx::CreateInfo (VkBufferCreateInfo& ci, uint32_t reqsize, VkBufferUsageFlags usage) {

  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  ci.pNext = nullptr;
  ci.size  = reqsize; 
  ci.usage = usage;
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  return ci; 
}

// -------------------------------------------------------------------------------------------
// in device                      
// -------------------------------------------------------------------------------------------
VkBufferCreateInfo& rokz::cx::CreateInfo_IB_16_device (VkBufferCreateInfo& ci, uint32_t num_elem) {

  const size_t sizeof_index = sizeof (uint16_t);

  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  ci.pNext = nullptr;

  ci.size  = sizeof_index * num_elem; 
  ci.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  return ci; 
}


// -----------------------------------------------------------------------------------------------
//                            
// -----------------------------------------------------------------------------------------------
bool rokz::cx::MoveToBuffer_XB2DB  (Buffer& dstb, // device buffer
                                    Buffer& srcb, // user buffer, 
                                    size_t size,
                                    const VkCommandPool& command_pool, 
                                    const VkQueue& que, 
                                    const VkDevice& device) {

  if ( !(srcb.ci.usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT)) {
    printf ("buff_src is not TRANSFER_SRC "); 
    return false; 
  }

  if ( !(dstb.ci.usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT)) {
    printf ("buff_dst is not TRANSFER_DST "); 
    return false; 
  }


  return MoveToBuffer_XB2DB  (dstb.handle, // device buffer
                              srcb, // user buffer, 
                              size,
                              command_pool, 
                              que, 
                              device);
}


// -----------------------------------------------------------------------------------------------
//                            
// -----------------------------------------------------------------------------------------------
bool rokz::cx::MoveToBuffer_XB2DB  (VkBuffer& buff_dst, // device buffer
                                    Buffer& buff_src, // user buffer, 
                                    size_t size,
                                    const VkCommandPool& command_pool, 
                                    const VkQueue& que, 
                                    const VkDevice& device) {

  
  VkCommandBufferAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandPool = command_pool;
  alloc_info.commandBufferCount = 1;

  VkCommandBuffer cmdbuf;
  vkAllocateCommandBuffers(device, &alloc_info, &cmdbuf);

  VkCommandBufferBeginInfo begin_info{};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vkBeginCommandBuffer (cmdbuf, &begin_info);

  VkBufferCopy copy_desc{};
  copy_desc.srcOffset = 0; 
  copy_desc.dstOffset = 0; 

  copy_desc.size = size;

  vkCmdCopyBuffer(cmdbuf, buff_src.handle, buff_dst, 1, &copy_desc);
  vkEndCommandBuffer(cmdbuf);    

  VkSubmitInfo submit_info{};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &cmdbuf;
    
  vkQueueSubmit(que, 1, &submit_info, VK_NULL_HANDLE);
  vkQueueWaitIdle(que);

  vkFreeCommandBuffers (device, command_pool, 1, &cmdbuf);

  return true; 
}



// ----------------------------------------------------------------------------------------
//                    
// ----------------------------------------------------------------------------------------
bool transfer_memory_to_device_buffer (rokz::Buffer& dstb, const void* mem, size_t sz_mem, const rokz::Device& device) {

  void* pmapped  = nullptr;
  rokz::Buffer vb_tmp; 

  rokz::cx::CreateInfo_buffer_stage (vb_tmp.ci, sz_mem); 
  rokz::cx::AllocCreateInfo_stage (vb_tmp.alloc_ci);
  rokz::cx::CreateBuffer (vb_tmp, device.allocator.handle);

  if (rokz::cx::MapMemory (&pmapped, vb_tmp.allocation, device.allocator.handle)) {
    memcpy (pmapped, mem, sz_mem); 
    rokz::cx::UnmapMemory (vb_tmp.allocation, device.allocator.handle); 
  }
  else {
    printf ("[FAILED] rokz::cx::MapMemory\n");
    return false;
  }
  
  rokz::cx::MoveToBuffer_XB2DB (dstb, vb_tmp, sz_mem, device.command_pool.handle, device.queues.graphics, device.handle); 

  rokz::cx::Destroy (vb_tmp, device.allocator);

  return true; 
}

// ----------------------------------------------------------------------------------------
//                    
// ----------------------------------------------------------------------------------------
bool
//rokz::
Create_VB_device (rokz::Buffer& buf, const void* mem, size_t sz_mem, const rokz::Device& device) {

  printf ("%s", __FUNCTION__);

  if (!mem) {
    printf ("[FAILED] mem  is null");
    return false;
  }

  if (sz_mem == 0) {
    printf ("sz_mem:%zu",  sz_mem);
    return false;
  }
  
  rokz::cx::CreateInfo_VB_device (buf.ci, 1, sz_mem);
  rokz::cx::AllocCreateInfo_device (buf.alloc_ci); 

  if (rokz::cx::CreateBuffer (buf, device.allocator.handle)) { 

    if (transfer_memory_to_device_buffer (buf, mem, sz_mem, device)) { 
      printf ("--> [true]\n");
      return true;
    }
  }

  printf ("%s", __FUNCTION__);
  return false;
}

// ----------------------------------------------------------------------------------------
//                    
// ----------------------------------------------------------------------------------------

bool
//rokz::
Create_IB_16_device (rokz::Buffer& buf, const void* mem, size_t num_inds, const rokz::Device& device) {
  printf ("%s", __FUNCTION__);

  if (!mem) {
    printf ("[FAILED] 'mem' is null\n");
    return false;
  }

  if (num_inds == 0) {
    printf ("[FAILED] num_inds:%zu\n", num_inds);
    return false;
  }
  
  rokz::cx::CreateInfo_IB_16_device (buf.ci, num_inds);
  rokz::cx::AllocCreateInfo_device (buf.alloc_ci); 

  if (rokz::cx::CreateBuffer (buf, device.allocator.handle)) { 

    if (transfer_memory_to_device_buffer (buf, mem, sizeof(uint16_t) * num_inds, device)) { 
      printf ("--> [true]\n");
      return true;
    }
    
    printf ("--> [true]\n");
    return true; 
  }
  
  printf ("[FAILED] CreateBuffer\n ");
  return false;
}

// ----------------------------------------------------------------------------------------
//                    
// ----------------------------------------------------------------------------------------
VkBufferCreateInfo& rokz::cx::CreateInfo_uniform (VkBufferCreateInfo& ci, size_t sizeOf_elem, size_t num_e) {

  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  ci.pNext = nullptr; 
  ci.size  = sizeOf_elem * num_e; 
  ci.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  return ci; 
}

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
void rokz::cx::Destroy (VkBuffer& buffer, VmaAllocation allocation, VmaAllocator allocator) {
  vmaDestroyBuffer (allocator, buffer, allocation); 
  buffer  = VK_NULL_HANDLE; 
  
}

void rokz::cx::Destroy (Buffer& buffer, const rokz::Allocator& allocator) {
  cx::Destroy (buffer.handle, buffer.allocation, allocator.handle); 
}




