


#include "transfer_buffer.hpp"
#include "allocation.hpp"
#include "buffer.hpp"


// ----------------------------------------------------------------------------------------
//                    
// ----------------------------------------------------------------------------------------
int rokz::cx::TransferToDeviceBuffer (VkBuffer& dstb, size_t sizemem, mappedbuffer_cb* cb, const rokz::Device& device) {
  void* pmapped  = nullptr;
  rokz::Buffer vb_tmp; 

  rokz::cx::CreateInfo_buffer_stage (vb_tmp.ci, sizemem); 
  rokz::cx::AllocCreateInfo_stage (vb_tmp.alloc_ci);
  rokz::cx::CreateBuffer (vb_tmp, device.allocator.handle);

  int res = __LINE__;
  if (rokz::cx::MapMemory (&pmapped, vb_tmp.allocation, device.allocator.handle)) {

    if (cb) res = cb->on_mapped (pmapped, sizemem); 

    rokz::cx::UnmapMemory (vb_tmp.allocation, device.allocator.handle); 
  }
  else {
    printf ("[FAILED] rokz::cx::MapMemory\n");
    return false;
  }
  
  rokz::cx::MoveToBuffer_XB2DB (dstb, vb_tmp, sizemem, device.command_pool.handle, device.queues.graphics, device.handle); 

  rokz::cx::Destroy (vb_tmp, device.allocator);

  return res; 
}
