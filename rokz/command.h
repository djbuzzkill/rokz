
#ifndef ROKZ_COMMAND_H
#define ROKZ_COMMAND_H

#include "common.h"
#include "buffer.h"


namespace rokz {

  namespace cx { 
    // --------------------------------------------------------
    VkCommandBuffer BeginCommandList (const VkCommandPool& command_pool,
                                      const VkDevice& device); 

    // --------------------------------------------------------
    void EndCommandList (VkCommandBuffer&     command_buffer,
                         const VkQueue&       queue,
                         const VkCommandPool& command_pool,
                         const VkDevice&      device); 

    // --------------------------------------------------------
    void TransitionImageLayout (VkImage image, VkFormat format,
                                const VkImageLayout& old_layout,
                                const VkImageLayout& new_layout,
                                const VkQueue&       queue,
                                const VkCommandPool& command_pool,
                                const VkDevice&      device); 

    // --------------------------------------------------------
    void CopyBufferToImage (VkImage&        image,
                            const VkBuffer  buffer,
                            uint32_t width, uint32_t height, 
                            const VkQueue&       queue,
                            const VkCommandPool& command_pool,
                            const VkDevice&      device); 

  }
}
#endif
