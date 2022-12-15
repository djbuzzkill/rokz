
#include "buffer.h"

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateBuffer (BufferStruc&            buffstruc, 
                         const VkDevice&         device,
                         const VkPhysicalDevice& physdev) {

  printf ("%s\n", __FUNCTION__); 

  if (vkCreateBuffer (device, &buffstruc.create_info, nullptr, &buffstruc.handle) != VK_SUCCESS) {
    printf ("failed to create buffer!");
    return false; 
  }
  
  VkMemoryRequirements mem_reqs;
  vkGetBufferMemoryRequirements(device, buffstruc.handle, &mem_reqs);
    
  buffstruc.alloc_info = {}; 
  buffstruc.alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  buffstruc.alloc_info.allocationSize = mem_reqs.size;
  rokz::FindMemoryType ( buffstruc.alloc_info.memoryTypeIndex, mem_reqs.memoryTypeBits, buffstruc.mem_prop_flags, physdev);

  if (vkAllocateMemory(device, &buffstruc.alloc_info, nullptr, &buffstruc.mem) != VK_SUCCESS) {
    printf ("failed to allocate buffer memory!");
    return false; 
  }

  if (VK_SUCCESS != vkBindBufferMemory (device, buffstruc.handle, buffstruc.mem, 0)) {
    printf ("[%s]  VB memory bind failed\n", __FUNCTION__);
    return false;
  }

  return true; 
} 

// ---------------------------------------------------------------------
// VMA
// ---------------------------------------------------------------------
bool rokz::CreateBuffer (Buffer& buffer, VmaAllocator const& allocator) {

  if (VK_SUCCESS != vmaCreateBuffer(allocator, &buffer.ci, &buffer.alloc_ci, &buffer.handle, &buffer.allocation, &buffer.alloc_info)) {

    printf ("[FAILED] %s vmaCreateBuffer", __FUNCTION__);
    return false; 
  }
  
  return true; 
}




// ---------------------------------------------------------------------
// transfer buffer
// ---------------------------------------------------------------------
// bool rokz::CreateVertexBuffer_transfer (BufferStruc& buffstruc, 
//                                size_t sizeof_elem,
//                                size_t num_elem, 
//                                const VkDevice& device,
//                                const VkPhysicalDevice& physdev) {
//   //
//   buffstruc.create_info = {};
//   buffstruc.create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//   buffstruc.create_info.size  = sizeof_elem * num_elem; 
//   buffstruc.create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT; 
//   buffstruc.create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  
//   buffstruc.mem_prop_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT; 
//   //
//   return rokz::CreateBuffer (buffstruc, device, physdev); 
// }


VkBufferCreateInfo& rokz::CreateInfo_VB_stage (VkBufferCreateInfo& ci, uint32_t vsize, uint32_t numv) {
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  ci.size  = vsize * numv; 
  ci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT; 
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  return ci;
}

VkBufferCreateInfo& rokz::CreateInfo_VB_device (VkBufferCreateInfo& ci, uint32_t vsize, uint32_t numv) {
  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  ci.size  = vsize * numv; 
  ci.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  return ci;
}
// ---------------------------------------------------------------------
//   device buffer
// ---------------------------------------------------------------------
// bool rokz::CreateVertexBuffer_device (BufferStruc& buffstruc, 
//                                       size_t sizeof_elem,
//                                       size_t num_elem, 
//                                       const VkDevice& device,
//                                       const VkPhysicalDevice& physdev) {
//   //
//   buffstruc.create_info = {};
//   buffstruc.create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//   buffstruc.create_info.size  = sizeof_elem * num_elem; 
//   buffstruc.create_info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
//   buffstruc.create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  
//   buffstruc.mem_prop_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT; 
//   //
//   return rokz::CreateBuffer (buffstruc, device, physdev); 
// }

//
// ---------------------------------------------------------------------
bool rokz::CreateVertexBuffer (BufferStruc& buffstruc, 
                               size_t sizeof_elem,
                               size_t num_elem, 
                               const VkDevice& device,
                               const VkPhysicalDevice& physdev) {
  //
  buffstruc.create_info = {};
  buffstruc.create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffstruc.create_info.size  = sizeof_elem * num_elem; 
  buffstruc.create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT; 
  buffstruc.create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  
  buffstruc.mem_prop_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT; 
  //
  return rokz::CreateBuffer (buffstruc, device, physdev); 
}




// ---------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------
VkBufferCreateInfo& rokz::CreateInfo_buffer_stage (VkBufferCreateInfo& ci, uint32_t sizebytes) {

  ci = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
  ci.pNext = nullptr;
  ci.size = sizebytes;
  ci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT; 
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  //memcpy(allocInfo.pMappedData, myData, myDataSize);
  return ci;
}

// ---------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------
VkBufferCreateInfo& CreateInfo_VB_device (VkBufferCreateInfo& ci, uint32_t sizebytes) {

  ci = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
  ci.pNext = nullptr;
  ci.size = sizebytes;
  ci.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT; 
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  //memcpy(allocInfo.pMappedData, myData, myDataSize);
  return ci;
}


// ---------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------
VkBufferCreateInfo& rokz::CreateInfo_IB_16_stage (VkBufferCreateInfo& ci, uint32_t num_elem) {

  const size_t sizeof_index = sizeof (uint16_t);

  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  ci.size  = sizeof_index * num_elem; 
  ci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  return ci; 
}

// ---------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------
VkBufferCreateInfo& rokz::CreateInfo_IB_16_device (VkBufferCreateInfo& ci, uint32_t num_elem) {

  const size_t sizeof_index = sizeof (uint16_t);

  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  ci.size  = sizeof_index * num_elem; 
  ci.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  return ci; 
}


// ---------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------
bool rokz::MoveToBuffer_XB2DB  (Buffer& buff_dst, // device buffer
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
  return true; 
}


void rokz::Destroy (Buffer& buffer, VmaAllocator const& allocator) {

  vmaDestroyBuffer (allocator, buffer.handle, buffer.allocation); 

}


VkBufferCreateInfo& rokz::CreateInfo_uniform (VkBufferCreateInfo& ci, size_t size_e, size_t num_e) {

  ci = {};
  ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  ci.size  = size_e * num_e; 
  ci.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  return ci; 
}

                                       
// bool rokz::CreateUniformBuffer (BufferStruc& buffstruc, 
//                                 size_t sizeof_elem,
//                                 size_t num_elem, 
//                                 const VkDevice& device,
//                                 const VkPhysicalDevice& physdev){

//   buffstruc.create_info = {};
//   buffstruc.create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//   buffstruc.create_info.size  = sizeof_elem * num_elem; 
//   buffstruc.create_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
//   buffstruc.create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  
//   buffstruc.mem_prop_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

//   return rokz::CreateBuffer (buffstruc, device, physdev); 
// }

// ---------------------------------------------------------------------
// lock/unlock vb
// ---------------------------------------------------------------------
bool rokz::MapBuffer (void** ptr, BufferStruc& vb, const VkDevice& device) { 

  assert (vb.mem_prop_flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT); 

  if (vb.mem_prop_flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
    return VK_SUCCESS == vkMapMemory (device, vb.mem, 0, vb.create_info.size, 0, ptr);
  }
  
  return false; 
}

// 
void rokz::UnmapBuffer (BufferStruc& vb, const VkDevice& device) {
  vkUnmapMemory (device, vb.mem);
}

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
// bool rokz::MoveToBuffer_user_mem (BufferStruc& dst_vb, const void* src, size_t size, const VkDevice& device) {
//   void* dst;

//   if (VK_SUCCESS == vkMapMemory (device, dst_vb.mem, 0, dst_vb.create_info.size, 0, &dst)) {
    
//     memcpy (dst, src, size);
//     vkUnmapMemory (device, dst_vb.mem);
//     return true; 
//   }
//   return false;
// }

void rokz::DestroyBuffer (BufferStruc& buf, const VkDevice &device) {

  vkDestroyBuffer (device, buf.handle, nullptr); 
  vkFreeMemory (device, buf.mem, nullptr);
}







