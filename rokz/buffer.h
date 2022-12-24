
#ifndef ROKZ_BUFFER_H
#define ROKZ_BUFFER_H

#include "common.h"
#include "utility.h"
#include "shared_types.h"


namespace rokz {

// ---------------------------------------------------------------------
  struct BufferStruc {
    VkBuffer              handle;
    VkDeviceMemory        mem; 
    VkBufferUsageFlags    usage_flags; 
    VkMemoryPropertyFlags mem_prop_flags;
    VkBufferCreateInfo    create_info; 
    VkMemoryAllocateInfo  alloc_info;
    VkIndexType           index_type;
  }; 

  // move from transfer buffer to device buffer
  // bool MoveToBuffer_XB2DB (BufferStruc&         buff_dst, // device buffer
  //                          BufferStruc&         buff_src, // user buffer, 
  //                          size_t               size,
  //                          const VkCommandPool& command_pool, 
  //                          const VkQueue&       que, 
  //                          const VkDevice&      device); 

  // ---------------------------------------------------------------------
  // 
  // ---------------------------------------------------------------------
  bool CreateBuffer (BufferStruc& buffstruc, const VkDevice& device, const VkPhysicalDevice& physdev); 

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
  // lock user memory vb
  bool MapBuffer   (void** mappedmem, BufferStruc& vb, const VkDevice& device);
  void UnmapBuffer (BufferStruc& vb, const VkDevice& device); 
  // 
  void DestroyBuffer (BufferStruc& buf, const VkDevice &device); 


  // 
  //
  // 


  // nu ---------------------------------------------------------------------------------> 
  struct Buffer {
    VkBuffer                handle;
    VkBufferCreateInfo      ci; 
    VmaAllocationCreateInfo alloc_ci; 
    VmaAllocation           allocation;
    VmaAllocationInfo       alloc_info;
  }; 


  VkBufferCreateInfo& CreateInfo_IB_16_device (VkBufferCreateInfo& ci, uint32_t num_elem); 
  VkBufferCreateInfo& CreateInfo_IB_16_stage  (VkBufferCreateInfo& ci, uint32_t num_elem); 
  VkBufferCreateInfo& CreateInfo_IB_16_local  (VkBufferCreateInfo& ci, uint32_t num_elem); 

  VkBufferCreateInfo& CreateInfo_VB_device    (VkBufferCreateInfo& ci, uint32_t vsize, uint32_t numv);
  VkBufferCreateInfo& CreateInfo_VB_stage     (VkBufferCreateInfo& ci, uint32_t vsize, uint32_t numv);

  VkBufferCreateInfo& CreateInfo_buffer_stage (VkBufferCreateInfo& ci, uint32_t sizebytes);
  VkBufferCreateInfo& CreateInfo_uniform      (VkBufferCreateInfo& ci, size_t size_e, size_t num_e); 
  //
  bool                CreateBuffer         (Buffer&, VmaAllocator const& allocator);

  bool MoveToBuffer_XB2DB (Buffer&         buff_dst, // device buffer
                           Buffer&         buff_src, // user buffer, 
                           size_t               size,
                           const VkCommandPool& command_pool, 
                           const VkQueue&       que, 
                           const VkDevice&      device); 

  inline bool MoveToBuffer_stage_2_dev (Buffer&              buff_dst, // device buffer
                                        Buffer&              buff_src, // user buffer, 
                                        size_t               size,
                                        const VkCommandPool& command_pool, 
                                        const VkQueue&       que, 
                                        const VkDevice&      device) {
    return MoveToBuffer_XB2DB (buff_dst, buff_src, size, command_pool, que, device); 
  }

  void Destroy (Buffer& buffer, VmaAllocator const& allocator); 
  // <---------------------------------------------------------------------------------- nu 

  
}


#endif

