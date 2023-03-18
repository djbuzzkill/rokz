
#ifndef REKZ_IMAGE_LOADER_INCLUDE
#define REKZ_IMAGE_LOADER_INCLUDE

#include "rokz/rokz.h"

namespace rekz { 

  // ---------------------------------------------------------------------
  // load texture to device memory
  // ---------------------------------------------------------------------
  bool LoadTexture_color_sampling (rokz::Image&             image,
                                   VkFormat                 format,
                                   const VkExtent2D&        ext2d,
                                   const void*              srcimage,
                                   const VmaAllocator&      allocator, 
                                   const VkQueue&           queue, 
                                   const rokz::CommandPool& commandpool, 
                                   const rokz::Device&      device);
  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  uint32_t SizeOfComponents (VkFormat format) ;
  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  uint32_t NumberOfComponents (VkFormat format); 


  // ---------------------------------------------------------------------
  // image handling - should we keep
  // ---------------------------------------------------------------------
  struct DevILImageProps {
    int width          ; //= ilGetInteger (IL_IMAGE_WIDTH); 
    int height         ; //= ilGetInteger (IL_IMAGE_HEIGHT);
    int depth          ; //= ilGetInteger (IL_IMAGE_DEPTH);
    int bytes_per_pixel; //= ilGetInteger (IL_IMAGE_BYTES_PER_PIXEL); 
    int bpp            ; //= ilGetInteger (IL_IMAGE_BPP);
    int type           ; //= ilGetInteger (IL_IMAGE_TYPE);
    int format         ; //= ilGetInteger (IL_IMAGE_FORMAT); 
  };

  using DevILOpenFileCB =  int(*)(const unsigned char* dat, const DevILImageProps& props, void* up); 

  int OpenImageFile (const std::string& fqname, DevILOpenFileCB cb, void* up);

  // struct ImageCB {
  //   virtual int do_shit (const unsigned char* dat, const DevILImageProps&) = 0; 
  // protected:
  //   ImageCB () { }
  // } ;

  // int OpenImageFile (const std::string& fqname, ImageCB*);

}
#endif
