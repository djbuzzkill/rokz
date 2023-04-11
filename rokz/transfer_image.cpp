
#include "transfer_image.hpp"
#include "utility.hpp"
#include "allocation.hpp"
#include "buffer.hpp"
#include "command.hpp"
#include "image.hpp"

using namespace rokz;
// ---------------------------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------------------------
int rokz::cx::TransferToDeviceImage (VkImage& dimg, size_t imagesize, VkFormat targetformat,
                                     const VkExtent2D& ext2d, mappedimage_cb* cb, 
                                     const rokz::Device& device) {

  void* mappedp = nullptr; 

  rokz::Buffer stage_buff; 
  rokz::cx::CreateInfo_buffer_stage (stage_buff.ci, imagesize);
  rokz::cx::AllocCreateInfo_stage (stage_buff.alloc_ci);
  rokz::cx::CreateBuffer (stage_buff, device.allocator.handle); 

  assert (device.allocator.handle);
  
  if (!rokz::cx::MapMemory (&mappedp, stage_buff.allocation, device.allocator.handle)) { 
    HERE ("FAILED MAP MEMORY");
    return __LINE__;
  }

  int res = __LINE__;

  if (cb) res = cb->on_mapped (mappedp, imagesize, ext2d); 

  rokz::cx::UnmapMemory (stage_buff.allocation, device.allocator.handle);

  
  if (res == 0) {
    rokz::cx::TransitionImageLayout (dimg, targetformat, VK_IMAGE_LAYOUT_UNDEFINED,
                                     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                     device.queues.graphics, device.command_pool.handle, device.handle);

    rokz::cx::CopyBufferToImage (dimg, stage_buff.handle, ext2d.width, ext2d.height,
                                 device.queues.graphics, device.command_pool.handle, device.handle);

    rokz::cx::TransitionImageLayout (dimg, targetformat, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                     device.queues.graphics, device.command_pool.handle, device.handle);

  }

  rokz::cx::Destroy (stage_buff, device.allocator); 

  if (GOT_PROB (res)) 
    HERE("LEAVING TRANSER BUT U HAV PROBZ"); 
    
  return res;
  
}





// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
int rokz::cx::TransferToImageLayer (VkImage& dsti, VkFormat format, size_t sizebytes ,
                                    uint32 numlayers, uint32 beginlayer, const VkExtent2D& ext2d,
                                    mappedlayer_cb* cb, const Device& device) { 

  void* mappedp = nullptr; 

  rokz::Buffer stage_buff; 
  rokz::cx::CreateInfo_buffer_stage (stage_buff.ci, sizebytes);
  rokz::cx::AllocCreateInfo_stage (stage_buff.alloc_ci);
  rokz::cx::CreateBuffer (stage_buff, device.allocator.handle); 

  assert (device.allocator.handle);
  
  int res = __LINE__;
  //
  for (uint32 layeri = 0; layeri < numlayers; ++layeri) {
    
    if (!rokz::cx::MapMemory (&mappedp, stage_buff.allocation, device.allocator.handle)) { 
      HERE ("FAILED MAP MEMORY");
      return __LINE__;
    }

    uint32 curlayer = layeri + beginlayer;
    res = cb->on_mapped (mappedp, sizebytes, curlayer, ext2d);

    rokz::cx::UnmapMemory (stage_buff.allocation, device.allocator.handle);

    if (res != 0) {
      assert (false); 
      HERE (">> break out >>");
      break;
    }
    
    rokz::cx::TransitionImageLayout (dsti, curlayer, format, VK_IMAGE_LAYOUT_UNDEFINED,
                                     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                     device.queues.graphics, device.command_pool.handle, device.handle);

    rokz::cx::CopyBufferToImage (dsti, curlayer, stage_buff.handle, ext2d.width, ext2d.height,
                                 device.queues.graphics, device.command_pool.handle, device.handle);

    rokz::cx::TransitionImageLayout (dsti, curlayer, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                     device.queues.graphics, device.command_pool.handle, device.handle);
  }

  rokz::cx::Destroy (stage_buff, device.allocator); 

  if (GOT_PROB (res)) {
    assert (false); 
    HERE("LEAVING TRANSER BUT U HAV PROBZ");
  }
    
  return res;
}

