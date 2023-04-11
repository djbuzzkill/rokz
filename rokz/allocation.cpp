#include "allocation.hpp"



VmaAllocatorCreateInfo& rokz::cx::CreateInfo (VmaAllocatorCreateInfo& ci, const Instance& instance, const Device& device, const PhysicalDevice& phydev) {


  ci = {};

  ci.vulkanApiVersion = VK_API_VERSION_1_3;
  ci.flags            = 0;
  ci.physicalDevice   = phydev.handle;
  ci.device           = device.handle;
  ci.instance         = instance.handle;
  return ci;
}

// ---------------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------------
VmaAllocationCreateInfo& rokz::cx::AllocCreateInfo_local    (VmaAllocationCreateInfo& ci){

  ci = {};
  ci.usage = VMA_MEMORY_USAGE_AUTO;
  return ci;
}

// ---------------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------------
VmaAllocationCreateInfo& rokz::cx::AllocCreateInfo_stage (VmaAllocationCreateInfo& alloc_info) {
  
  alloc_info = {};
  alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
  alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
                   | VMA_ALLOCATION_CREATE_MAPPED_BIT;
  //alloc_info.priority = 1.0f;
  return alloc_info; 
}
// ---------------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------------
VmaAllocationCreateInfo& rokz::cx::AllocCreateInfo_device (VmaAllocationCreateInfo& alloc_info) {
  
  alloc_info = {};
  alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
  alloc_info.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
  alloc_info.priority = 1.0f;
  return alloc_info; 
}

// ---------------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------------
VmaAllocationCreateInfo& rokz::cx::AllocCreateInfo_mapped (VmaAllocationCreateInfo& ci) {
  ci = {}; 

  ci.usage = VMA_MEMORY_USAGE_AUTO;
  ci.flags  = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
            | VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT
            | VMA_ALLOCATION_CREATE_MAPPED_BIT;
  
  return ci;

}

// ---------------------------------------------------------------------------
// VMA 
// ---------------------------------------------------------------------------
void rokz::cx::UnmapMemory (VmaAllocation const& allocation, VmaAllocator const& allocator) {

  vmaUnmapMemory (allocator, allocation); 

}

bool rokz::cx::MapMemory (void** pmapped, VmaAllocation const& allocation,  VmaAllocator const& allocator) {
  
  if (VK_SUCCESS != vmaMapMemory (allocator, allocation, pmapped)) {
    printf ("[FAILED] %s vmaMapMemory\n", __FUNCTION__); 
    return false; 
  }
  return true;
}

  
