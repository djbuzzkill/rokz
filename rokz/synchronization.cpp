

#include "synchronization.h"


// ---------------------------------------------------------------------------------------------
//                                 
// ---------------------------------------------------------------------------------------------
bool rokz::cx::CreateFrameSync (FrameSync&      sync,
                             FrameSyncCreateInfo& create_info,
                             const VkDevice& device) {

  printf ("%s\n", __FUNCTION__);
  
  create_info.semaphore = {};
  create_info.semaphore.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  create_info.semaphore.pNext = nullptr;
  
  create_info.fence ={}; 
  create_info.fence.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  create_info.fence.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  create_info.fence.pNext = nullptr;
  if (vkCreateSemaphore(device, &create_info.semaphore, nullptr, &sync.image_available_sem) != VK_SUCCESS
      || vkCreateSemaphore(device, &create_info.semaphore, nullptr, &sync.render_finished_sem) != VK_SUCCESS
      || vkCreateFence(device, &create_info.fence, nullptr, &sync.in_flight_fen) != VK_SUCCESS)
    {
      printf ("[FAILED]--> create sync objects\n");
      return false; 
    }
  
  return true; 
}

// ---------------------------------------------------------------------------------------------
//                                 
// ---------------------------------------------------------------------------------------------
void rokz::cx:: Destroy (rokz::FrameSync& sync, const rokz::Device& device) {

    vkDestroySemaphore(device.handle, sync.image_available_sem, nullptr);
    sync.image_available_sem = VK_NULL_HANDLE;
    vkDestroySemaphore(device.handle, sync.render_finished_sem, nullptr);
    sync.render_finished_sem = VK_NULL_HANDLE;

    vkDestroyFence (device.handle, sync.in_flight_fen, nullptr);
    sync.in_flight_fen = VK_NULL_HANDLE;
}

// ---------------------------------------------------------------------------------------------
//                                 
// ---------------------------------------------------------------------------------------------
void rokz::cx:: Destroy (Vec<rokz::FrameSync>& syncs, const rokz::Device& device) {

  for (auto& sync : syncs) {
      Destroy (sync, device); 
    }

}


