#ifndef ROKZ_VMA_ALLOCATION_INCLUDE
#define ROKZ_VMA_ALLOCATION_INCLUDE


#include "common.h"


namespace rokz { 

  // VMA
  VmaAllocationCreateInfo& AllocCreateInfo_local    (VmaAllocationCreateInfo& alloc_info); 
  VmaAllocationCreateInfo& AllocCreateInfo_stage   (VmaAllocationCreateInfo& alloc_info); 
  VmaAllocationCreateInfo& AllocCreateInfo_device   (VmaAllocationCreateInfo& alloc_info); 

  void UnmapMemory (VmaAllocation const& allocation, VmaAllocator const& allocator);
  bool MapMemory (void** pmapped, VmaAllocation const& allocation,  VmaAllocator const& allocator);

}

#endif
