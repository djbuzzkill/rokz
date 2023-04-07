
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


    void TransitionImageLayout (VkImage              image,
                                rokz::uint32         layeri, 
                                VkFormat             format,
                                const VkImageLayout& old_layout,
                                const VkImageLayout& new_layout,
                                const VkQueue&       queue,
                                const VkCommandPool& command_pool,
                                const VkDevice&      device); 

    inline void TransitionImageLayout (VkImage image, VkFormat format,
                                const VkImageLayout& old_layout,
                                const VkImageLayout& new_layout,
                                const VkQueue&       queue,
                                const VkCommandPool& command_pool,
                                const VkDevice&      device) {
      TransitionImageLayout (image, 0, format,
                             old_layout, new_layout, queue, command_pool, device); 
      
    }

    // ------------------------------------------------------------------------------------
    void CopyBufferToImage (VkImage&        image,
                            uint32          imagelayer,
                            // ??? uint32 miplayer, 
                            const VkBuffer  buffer,
                            uint32 width, uint32 height, 
                            const VkQueue&       queue,
                            const VkCommandPool& command_pool,
                            const VkDevice&      device); 
    // ------------------------------------------------------------------------------------
    inline void CopyBufferToImage (VkImage&        image,
                                   const VkBuffer  buffer,
                                   uint32 width, uint32 height, 
                                   const VkQueue&       queue,
                                   const VkCommandPool& command_pool,
                                   const VkDevice&      device) {
      CopyBufferToImage (image, 0, buffer, width, height,  queue, command_pool, device); 
    }

    
  }}
#endif
