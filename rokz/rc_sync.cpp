




#include "rc_sync.hpp"
#include "rokz_types.hpp"
#include <vulkan/vulkan_core.h>


using namespace rokz;

// -----------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------
rc::Semaphore::Ref CreateSemaphore (const Device& dev)  {
  rc::Semaphore::Ref ret = std::make_shared<rc::Semaphore> (dev);

  VkSemaphoreCreateInfo ci {};
  ci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  ci.pNext = nullptr;

  if (VK_SUCCESS != vkCreateSemaphore (dev.handle, &ci, nullptr, &ret->handle))
    return rc::Semaphore::Ref (nullptr); 

  return ret; 
}



// -----------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------
rc::Fence::Ref CreateFence (VkFenceCreateFlagBits flags, const Device& device) {
  VkFenceCreateInfo ci {}; 
  ci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  ci.flags = flags;
  ci.pNext = nullptr;

  rc::Fence::Ref res =  std::make_shared<rc::Fence> (device); 

  if (VK_SUCCESS != vkCreateFence (device.handle, &ci, nullptr, &res->handle)) 
    return rc::Fence::Ref (nullptr);
  
  return res; 
}
