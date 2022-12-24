#ifndef ROKZ_VMA_ALLOCATION_INCLUDE
#define ROKZ_VMA_ALLOCATION_INCLUDE


#include "common.h"


namespace rokz { 
  // -----------------------------------------------------------------------
  //  all VMA in here 
  // -----------------------------------------------------------------------
  VmaAllocationCreateInfo& AllocCreateInfo_local  (VmaAllocationCreateInfo& ci); 
  VmaAllocationCreateInfo& AllocCreateInfo_stage  (VmaAllocationCreateInfo& ci); 
  VmaAllocationCreateInfo& AllocCreateInfo_device (VmaAllocationCreateInfo& ci); 
  VmaAllocationCreateInfo& AllocCreateInfo_mapped (VmaAllocationCreateInfo& ci); 

  void UnmapMemory (VmaAllocation const& allocation, VmaAllocator const& allocator);
  bool MapMemory (void** pmapped, VmaAllocation const& allocation,  VmaAllocator const& allocator);

}

#endif
