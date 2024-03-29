
#include "command.hpp"
#include <vulkan/vulkan_core.h>



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
    VkImageMemoryBarrier& barrier,
    VkPipelineStageFlags& src_stage,
    VkPipelineStageFlags& dst_stage,
    VkFormat              format, 
    VkImageLayout         old_layout,
    VkImageLayout         new_layout) {

  //
  // 
  if (new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {

    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

    if (has_stencil_component(format)) {
      barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    
  } else {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  }

  // 
  // make ready for host transfer
  //
  if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED
   && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  }

  //
  // new from host/ready/ready for read only 
  //
  else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {

    src_stage             = VK_PIPELINE_STAGE_TRANSFER_BIT;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    dst_stage             = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  }

  //
  // new depth/ make ready for use
  //
  else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED
        && new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {

    src_stage             = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    barrier.srcAccessMask = VK_ACCESS_NONE;

    dst_stage             = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  }


  // 
  // the begining color target pre render
  //
  else if (old_layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        && new_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL ) {

    src_stage             = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    barrier.srcAccessMask = VK_ACCESS_NONE;

    dst_stage             = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT ;
  }
  // 
  // end drawing ready for presenting
  //
  else if (old_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        && new_layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {

    barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    src_stage             = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    dst_stage             = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT ; // ;
    barrier.dstAccessMask = VK_ACCESS_NONE;   // is VK_ACCESS_NONE correct?
  }

  else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED
        && new_layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {

    src_stage             = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    barrier.srcAccessMask = VK_ACCESS_NONE;

    dst_stage             = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    barrier.dstAccessMask = VK_ACCESS_NONE;
  }

  else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED
        && new_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL ) {

    src_stage             = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    barrier.srcAccessMask = VK_ACCESS_NONE;

    dst_stage             = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT ;
  }

  else {
    printf ("%s --> [ERROR] UNHANDLED TRANSITION \n", __FUNCTION__);
  }

  return barrier;
}


// ------------------------------------------------------------------------------------------
//                       
// ------------------------------------------------------------------------------------------
// void rokz::cx::TransitionImageLayout (VkImage              image,
//                                   VkFormat             format,
//                                   const VkImageLayout& old_layout,
//                                   const VkImageLayout& new_layout,
//                                   const VkQueue&       queue,
//                                   const VkCommandPool& command_pool,
//                                   const VkDevice&      device) {

//   VkCommandBuffer command_buffer  = BeginCommandList (command_pool, device); 

//   VkImageMemoryBarrier barrier{};
//   barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
//   barrier.pNext = nullptr;

//   barrier.oldLayout = old_layout;
//   barrier.newLayout = new_layout;
//   barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//   barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

//   barrier.image = image;
//   barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//   barrier.subresourceRange.baseMipLevel = 0;
//   barrier.subresourceRange.levelCount = 1;
//   barrier.subresourceRange.baseArrayLayer = 0;
//   barrier.subresourceRange.layerCount = 1;
  
//   VkPipelineStageFlags dst_flags, src_flags;  
//   transition_barrier_mask (barrier, src_flags, dst_flags, format, old_layout, new_layout); 
//   vkCmdPipelineBarrier (command_buffer, src_flags, dst_flags, 0,
//                         0, nullptr, 0, nullptr, 1, &barrier);
  
//   EndCommandList (command_buffer, queue, command_pool, device);
  
// }

// ------------------------------------------------------------------------------------------
//                       
// ------------------------------------------------------------------------------------------
void rokz::cx::TransitionImageLayout (VkImage              image,
                                      uint32               layeri, 
                                      VkFormat             format,
                                      const VkImageLayout& old_layout,
                                      const VkImageLayout& new_layout,
                                      const VkQueue&       queue,
                                      const VkCommandPool& command_pool,
                                      const VkDevice&      device) {
  using namespace rokz; 
  
  VkCommandBuffer command_buffer  = cx::BeginCommandList (command_pool, device); 

  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.pNext = nullptr;

  barrier.oldLayout = old_layout;
  barrier.newLayout = new_layout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

  const uint32 layer_count = 1;       // <---- shud we parameterize
  
  barrier.image                           = image;
  barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel   = 0;
  barrier.subresourceRange.levelCount     = 1;
  barrier.subresourceRange.baseArrayLayer = layeri;
  barrier.subresourceRange.layerCount     = layer_count;
  
  VkPipelineStageFlags dst_flags, src_flags;  
  transition_barrier_mask (barrier, src_flags, dst_flags, format, old_layout, new_layout); 
  vkCmdPipelineBarrier (command_buffer, src_flags, dst_flags, 0,
                        0, nullptr, 0, nullptr, 1, &barrier);
  
  cx::EndCommandList (command_buffer, queue, command_pool, device);
  
}
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
VkCommandBuffer rokz::cx::BeginCommandList (const VkCommandPool& command_pool,
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
void rokz::cx::EndCommandList (VkCommandBuffer&     command_buffer,
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
void rokz::cx::CopyBufferToImage (VkImage&          image,
                                  uint32            imagelayer , 
                                  const VkBuffer    buffer,
                                  uint32            width,
                                  uint32            height, 
                                  const VkQueue&       queue,
                                  const VkCommandPool& command_pool,
                                  const VkDevice&      device) {

  VkCommandBuffer  command_buffer  = BeginCommandList (command_pool, device); 

  VkBufferImageCopy region{};
  region.bufferOffset      = 0;
  region.bufferRowLength   = 0;
  region.bufferImageHeight = 0;

  region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel       = 0;
  region.imageSubresource.baseArrayLayer = imagelayer;
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
