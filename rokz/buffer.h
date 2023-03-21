
#ifndef ROKZ_BUFFER_H
#define ROKZ_BUFFER_H

#include "common.h"
#include "utility.h"
#include "rokz_types.h"


namespace rokz {

  namespace cx { 
    VkBufferCreateInfo& CreateInfo_IB_16_device (VkBufferCreateInfo& ci, uint32_t num_elem); 
    VkBufferCreateInfo& CreateInfo_IB_16_stage  (VkBufferCreateInfo& ci, uint32_t num_elem); 
    VkBufferCreateInfo& CreateInfo_IB_16_local  (VkBufferCreateInfo& ci, uint32_t num_elem); 

    VkBufferCreateInfo& CreateInfo_VB_device    (VkBufferCreateInfo& ci, uint32_t vsize, uint32_t numv);
    VkBufferCreateInfo& CreateInfo_VB_stage     (VkBufferCreateInfo& ci, uint32_t vsize, uint32_t numv);

    VkBufferCreateInfo& CreateInfo_buffer_stage (VkBufferCreateInfo& ci, uint32_t sizebytes);
    VkBufferCreateInfo& CreateInfo_uniform      (VkBufferCreateInfo& ci, size_t size_e, size_t num_e); 
    VmaAllocationCreateInfo& CreateInfo_default (VmaAllocationCreateInfo& ci) ; 
    //
    bool                CreateBuffer         (Buffer&, VmaAllocator const& allocator);
    bool                CreateBuffer_aligned (Buffer& buffer, VkDeviceSize min_align, VmaAllocator const& allocator); 

    void                Destroy              (VkBuffer& buffer, VmaAllocation allocation, VmaAllocator allocator);
    void                Destroy              (Buffer& buffer, const rokz::Allocator& allocator); 
    
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

    // <---------------------------------------------------------------------------------- nu 

    inline void* MappedPointer (const Buffer& buff) { 
      return  buff.alloc_info.pMappedData;
    }

  } // <---------------------------------------------------------------------------------- cx

  
  bool Create_VB_device    (rokz::Buffer& buf, const void* mem, size_t sz_mem, const rokz::Device& device);
  bool Create_IB_16_device (rokz::Buffer& buf, const void* mem, size_t sz_mem, const rokz::Device& device);
  
} // rokz


#endif

