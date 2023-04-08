
#ifndef ROKZ_BUFFER_H
#define ROKZ_BUFFER_H

#include "common.h"
#include "utility.h"
#include "rokz_types.h"


namespace rokz { namespace cx { 

  const VkBufferUsageFlags kDeviceGeometryUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT 
                                                | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
                                                | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;


    VkBufferCreateInfo& CreateInfo_IB_16_device (VkBufferCreateInfo& ci, uint32_t num_elem); 
    VkBufferCreateInfo& CreateInfo_IB_16_stage  (VkBufferCreateInfo& ci, uint32_t num_elem); 
    VkBufferCreateInfo& CreateInfo_IB_16_local  (VkBufferCreateInfo& ci, uint32_t num_elem); 

    VkBufferCreateInfo& CreateInfo_VB_device    (VkBufferCreateInfo& ci, uint32_t vsize, uint32_t numv);
    VkBufferCreateInfo& CreateInfo_VB_stage     (VkBufferCreateInfo& ci, uint32_t vsize, uint32_t numv);

    VkBufferCreateInfo& CreateInfo_buffer_stage (VkBufferCreateInfo& ci, uint32_t sizebytes);
    VkBufferCreateInfo& CreateInfo_uniform      (VkBufferCreateInfo& ci, size_t size_e, size_t num_e); 
    VmaAllocationCreateInfo& CreateInfo_default (VmaAllocationCreateInfo& ci) ; 


    VkBufferCreateInfo& CreateInfo (VkBufferCreateInfo& ci, uint32_t reqsize, VkBufferUsageFlags usage);

    //bool CreateBuffer         (VkBuffer&, VmaAllocator const& allocator);
    //
    bool                CreateBuffer (Buffer&, VmaAllocator const& allocator);

    bool                CreateBuffer (VkBuffer&                      buffer,
                                      VmaAllocation&                 allocation,
                                      VmaAllocationInfo&             alloc_info,
                                      const VmaAllocationCreateInfo& alloc_ci,
                                      const VkBufferCreateInfo&      ci,
                                      VmaAllocator const&            allocator);

    bool                CreateBuffer_aligned (Buffer& buffer, VkDeviceSize min_align, VmaAllocator const& allocator); 

    void                Destroy              (VkBuffer& buffer, VmaAllocation allocation, VmaAllocator allocator);
    void                Destroy              (Buffer& buffer, const rokz::Allocator& allocator); 
    

    bool MoveToBuffer_XB2DB (VkBuffer&        buff_dst, // device buffer
                             Buffer&         buff_src, // user buffer, 
                             size_t               size,
                             const VkCommandPool& command_pool, 
                             const VkQueue&       que, 
                             const VkDevice&      device); 

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


  }    
  
  // bool Create_VB_device    (rokz::Buffer& buf, const void* mem, size_t sz_mem, const rokz::Device& device);
  // bool Create_IB_16_device (rokz::Buffer& buf, const void* mem, size_t sz_mem, const rokz::Device& device);
  
} // rokz


#endif

