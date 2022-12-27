
#include "command.h"



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool has_stencil_component(VkFormat format) {
  return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
VkImageMemoryBarrier& transition_barrier_mask (
    VkPipelineStageFlags& dst_stage,
    VkPipelineStageFlags& src_stage,
    VkImageMemoryBarrier& barrier,
    VkImageLayout new_layout,
    VkImageLayout old_layout) {

  // if (new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
  //   barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

  //   if (hasStencilComponent(format)) {
  //     barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
  //   }
  // } else {
  //   barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  // }
  
  if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  }
  else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
         & new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {

    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  }

  else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED
        && new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {

    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dst_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  }
  else {
     printf ("[ERROR] unknown layout transition\n");
  }

  return barrier;
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void rokz::TransitionImageLayout (VkImage image,
                                  VkFormat format,
                                  const VkImageLayout& old_layout,
                                  const VkImageLayout& new_layout,
                                  const VkQueue&       queue,
                                  const VkCommandPool& command_pool,
                                  const VkDevice&      device) {

  (void) format; // <-- for now

  VkCommandBuffer command_buffer  = BeginCommandList (command_pool, device); 

  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.pNext = nullptr;
  barrier.oldLayout = old_layout;
  barrier.newLayout = new_layout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

  barrier.image = image;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  // VkBufferMemoryBarrier _buffmem_barr;
  // VkMemoryBarrier       _mem_barr;
  
  VkPipelineStageFlags dst_stage, src_stage;  
  transition_barrier_mask (dst_stage, src_stage, barrier , new_layout , old_layout); 
  // barrier.srcAccessMask = 0;  printf ("[TODO] (%i)\n", __LINE__);
  // barrier.dstAccessMask = 0;  printf ("[TODO] (%i)\n", __LINE__);

  vkCmdPipelineBarrier(command_buffer,
                       src_stage,
                       dst_stage,
                       0,
                       0, nullptr,
                       0, nullptr,
                       1, &barrier
                       );
  
  EndCommandList (command_buffer, queue, command_pool, device);
  
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
VkCommandBuffer rokz::BeginCommandList (const VkCommandPool& command_pool,
                                        const VkDevice& device) {

  VkCommandBufferAllocateInfo alloc_info {};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.pNext = nullptr;
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandPool = command_pool;
  alloc_info.commandBufferCount = 1;

  VkCommandBuffer command_buffer;
  vkAllocateCommandBuffers(device, &alloc_info, &command_buffer);

  VkCommandBufferBeginInfo begin_info {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.pNext = nullptr;
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  if (VK_SUCCESS != vkBeginCommandBuffer(command_buffer, &begin_info)) {

    printf ("[FAILED] %s begin command buffer\n", __FUNCTION__); 
    return nullptr;
  }

  return command_buffer;
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void rokz::EndCommandList (VkCommandBuffer&     command_buffer,
                           const VkQueue&       queue,
                           const VkCommandPool& command_pool,
                           const VkDevice&      device) {

  vkEndCommandBuffer(command_buffer);

  VkSubmitInfo submit_info {};
  submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.pNext              = nullptr;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers    = &command_buffer;

  vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE);
  vkQueueWaitIdle(queue);

  vkFreeCommandBuffers (device, command_pool, 1, &command_buffer);
}



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void  rokz::CopyBuffer (rokz::BufferStruc&       dst,
                        const rokz::BufferStruc& src,
                        VkDeviceSize             size,
                        const VkQueue&           queue,
                        const VkCommandPool&     command_pool,
                        const VkDevice&          device) {

  VkCommandBuffer command_buffer = BeginCommandList (command_pool, device);

  VkBufferCopy copy_region{};
  copy_region.size = size;
  vkCmdCopyBuffer (command_buffer, src.handle, dst.handle, 1, &copy_region);

  EndCommandList (command_buffer, queue, command_pool, device);  

}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void rokz::CopyBufferToImage (VkImage&          image,
                              const VkBuffer    buffer,
                              uint32_t width,   uint32_t height, 
                              const VkQueue&       queue,
                              const VkCommandPool& command_pool,
                              const VkDevice&      device) {

  printf ("%s\n", __FUNCTION__); 

  VkCommandBuffer  command_buffer  = BeginCommandList (command_pool, device); 

  VkBufferImageCopy region{};
  region.bufferOffset      = 0;
  region.bufferRowLength   = 0;
  region.bufferImageHeight = 0;

  region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel       = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount     = 1;
  region.imageOffset                     = {0, 0, 0};
  region.imageExtent                     = { width, height, 1 };
 
  vkCmdCopyBufferToImage(command_buffer,
                         buffer,
                         image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                         1,
                         &region
                         );
 
  EndCommandList (command_buffer, queue, command_pool, device);
}
