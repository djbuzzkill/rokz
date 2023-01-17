#ifndef ROKZ_VMA_ALLOCATION_INCLUDE
#define ROKZ_VMA_ALLOCATION_INCLUDE


#include "common.h"
#include "shared_types.h"
#include <vulkan/vulkan_core.h>


namespace rokz { 


  
  namespace cx { 

    void UnmapMemory (VmaAllocation const& allocation, VmaAllocator const& allocator);
    bool MapMemory (void** pmapped, VmaAllocation const& allocation,  VmaAllocator const& allocator);



    VmaAllocatorCreateInfo& CreateInfo (VmaAllocatorCreateInfo& ci,
                                      const Instance& instance,
                                      const Device& device,
                                      const PhysicalDevice& phydev);

    // -----------------------------------------------------------------------
    //  all VMA in here 
    // -----------------------------------------------------------------------
    VmaAllocationCreateInfo& AllocCreateInfo_local  (VmaAllocationCreateInfo& ci); 
    VmaAllocationCreateInfo& AllocCreateInfo_stage  (VmaAllocationCreateInfo& ci); 
    VmaAllocationCreateInfo& AllocCreateInfo_device (VmaAllocationCreateInfo& ci); 
    VmaAllocationCreateInfo& AllocCreateInfo_mapped (VmaAllocationCreateInfo& ci); 
  }





  
}

#endif
