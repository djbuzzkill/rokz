
#ifndef ROKZ_TEXTURE_H
#define ROKZ_TEXTURE_H

#include "common.hpp"
#include "rokz_types.hpp"
#include "rc_image.hpp"


namespace rokz {


  // --------------------------------------------------------------------
  // ?? do these belong here
  // --------------------------------------------------------------------
  uint32_t SizeOfComponents (VkFormat format) ;
  // ------------------------------------------------------------------------------------------
  // ?? do these belong here
  // ------------------------------------------------------------------------------------------
  uint32_t NumberOfComponents (VkFormat format); 

  // ---------------------------------------------------------------------
  // this shud go in texture
  // ---------------------------------------------------------------------
  bool LoadTexture_color_sampling (rokz::Image&             image,
                                   VkFormat                 format,
                                   const VkExtent2D&        ext2d,
                                   const void*              srcimage,
                                   const VmaAllocator&      allocator, 
                                   const VkQueue&           queue, 
                                   const rokz::CommandPool& commandpool, 
                                   const rokz::Device&      device);


  rokz::rc::Image::Ref
  LoadTexture_color_sampling (VkFormat                 format,
                              const VkExtent2D&        ext2d,
                              const void*              srcimage,
                              const VmaAllocator&      allocator, 
                              const VkQueue&           queue, 
                              const rokz::CommandPool& commandpool, 
                              const rokz::Device&      device); 

}
#endif
