
#ifndef REKZ_IMAGE_LOADER_INCLUDE
#define REKZ_IMAGE_LOADER_INCLUDE

#include "rokz/rokz.h"

namespace rekz { 

  // ------------------------------------------------------------------------------------------
  // image handling - should we keep
  // ------------------------------------------------------------------------------------------
  struct DevILImageProps {
    int width          ; //= ilGetInteger (IL_IMAGE_WIDTH); 
    int height         ; //= ilGetInteger (IL_IMAGE_HEIGHT);
    int depth          ; //= ilGetInteger (IL_IMAGE_DEPTH);
    int bytes_per_pixel; //= ilGetInteger (IL_IMAGE_BYTES_PER_PIXEL); 
    int bpp            ; //= ilGetInteger (IL_IMAGE_BPP);
    int type           ; //= ilGetInteger (IL_IMAGE_TYPE);
    int format         ; //= ilGetInteger (IL_IMAGE_FORMAT); 
  };

  // ------------------------------------------------------------------------------------------
  // 
  // ------------------------------------------------------------------------------------------
  struct DevILOpenFileCB {

    typedef std::shared_ptr<DevILOpenFileCB> Ref;

    virtual int Exec (const unsigned char* dat, const DevILImageProps& props) = 0;

  protected:
    DevILOpenFileCB () {};
  };
  
  // ------------------------------------------------------------------------------------------
  // 
  // ------------------------------------------------------------------------------------------
  int OpenImageFile (const std::string& fqname, DevILOpenFileCB::Ref cb);

}
#endif
