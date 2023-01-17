
#include "buffer.h"
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

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
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
  ci.size  = sizeOf_vert * numv; 
  ci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT; 
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  return ci;
}

VkBufferCreateInfo& rokz::cx::CreateInfo_VB_device (VkBufferCreateInfo& ci, uint32_t sizeOf_vert, uint32_t numv) {
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
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
  ci.size  = sizeof_index * num_elem; 
  ci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  return ci; 
}

// ---------------------------------------------------------------------
// in device
// ---------------------------------------------------------------------
VkBufferCreateInfo& rokz::cx::CreateInfo_IB_16_device (VkBufferCreateInfo& ci, uint32_t num_elem) {

  const size_t sizeof_index = sizeof (uint16_t);

  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  ci.size  = sizeof_index * num_elem; 
  ci.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  return ci; 
}


// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
bool rokz::cx::MoveToBuffer_XB2DB  (Buffer& buff_dst, // device buffer
                                Buffer& buff_src, // user buffer, 
                                size_t size,
                                const VkCommandPool& command_pool, 
                                const VkQueue& que, 
                                const VkDevice& device) {

  if ( !(buff_src.ci.usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT)) {
    printf ("buff_src is not TRANSFER_SRC "); 
    return false; 
  }

  if ( !(buff_dst.ci.usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT)) {
    printf ("buff_dst is not TRANSFER_DST "); 
    return false; 
  }
  
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
  printf (" ____ copy_desc.size = %zu\n", size); 

  copy_desc.size = size;

  vkCmdCopyBuffer(cmdbuf, buff_src.handle, buff_dst.handle, 1, &copy_desc);
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


// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
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
void rokz::cx::Destroy (Buffer& buffer, VmaAllocator const& allocator) {

  vmaDestroyBuffer (allocator, buffer.handle, buffer.allocation); 

}


