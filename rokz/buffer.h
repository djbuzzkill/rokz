
#ifndef ROKZ_BUFFER_H
#define ROKZ_BUFFER_H

#include "common.h"
#include "utility.h"
#include "shared_types.h"


namespace rokz {

  struct BufferStruc {
    VkBuffer              handle;
    VkDeviceMemory        mem; 
    VkBufferUsageFlags    usage_flags; 
    VkMemoryPropertyFlags mem_prop_flags;
    VkBufferCreateInfo    create_info; 
    VkMemoryAllocateInfo  alloc_info;
    VkIndexType          index_type;
  }; 
  

  bool CreateBuffer (BufferStruc& buffstruc, const VkDevice& device, const VkPhysicalDevice& physdev); 

  // simple vb in user memory 
  bool CreateVertexBuffer (BufferStruc& buffstruc, 
                           size_t sizeof_elem,
                           size_t num_elem, 
                           const VkDevice& device,
                           const VkPhysicalDevice& physdev); 

  //  transfer buffer
  bool CreateVertexBuffer_transfer (BufferStruc& buffstruc, 
                           size_t sizeof_elem,
                           size_t num_elem, 
                           const VkDevice& device,
                           const VkPhysicalDevice& physdev); 
  // create device buffer
  bool CreateVertexBuffer_device (BufferStruc& buffstruc, 
                           size_t sizeof_elem,
                           size_t num_elem, 
                           const VkDevice& device,
                           const VkPhysicalDevice& physdev); 
  // 
  bool CreateIndexBuffer_transfer (BufferStruc& buffstruc, 
                                   VkIndexType index_type, // VK_INDEX_TYPE_UINT16
                                   size_t num_elem, 
                                   const VkDevice& device,
                                   const VkPhysicalDevice& physdev); 

  bool CreateIndexBuffer_device (BufferStruc& buffstruc, 
                                 VkIndexType index_type, // VK_INDEX_TYPE_UINT16
                                 size_t num_elem, 
                                 const VkDevice& device,
                                 const VkPhysicalDevice& physdev); 

  //
  // lock user memory vb
  bool MapBuffer   (void** mappedmem, BufferStruc& vb, const VkDevice& device);
  void UnmapBuffer (BufferStruc& vb, const VkDevice& device); 

  // copy to user vb from user pointer
  bool MoveToBuffer_user_mem  (BufferStruc& buff_dev,
                      const void* buff_xfer,
                      size_t size,
                      const VkDevice& device);

  // move from transfer buffer to device buffer
  bool MoveToBuffer_XB2DB  (BufferStruc&         buff_dst, // device buffer
                            BufferStruc&         buff_src, // user buffer, 
                            size_t               size,
                            const VkCommandPool& command_pool, 
                            const VkQueue&       que, 
                            const VkDevice&      device); 
  // 
  void DestroyBuffer (BufferStruc& buf, const VkDevice &device); 

  
}


#endif

