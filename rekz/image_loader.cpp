


#include "image_loader.h"


#include <IL/il.h>
#include <IL/ilu.h>


using namespace rokz;


// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
int rekz::OpenImageFile (const std::string& fqname, rekz::DevILOpenFileCB::Ref cb) {

  DevILImageProps props; 

  ilInit ();
  ilBindImage (ilGenImage ());

  int res = 125;
  if (ilLoadImage(fqname.c_str())) {
    
    printf ("Opened [%s]\n", fqname.c_str() ); 
    props.width    = ilGetInteger (IL_IMAGE_WIDTH); 
    props.height   = ilGetInteger (IL_IMAGE_HEIGHT);
    props.depth    = ilGetInteger (IL_IMAGE_DEPTH);
    props.bytes_per_pixel= ilGetInteger (IL_IMAGE_BYTES_PER_PIXEL); 
    props.bpp      = ilGetInteger (IL_IMAGE_BPP);
    props.type     = ilGetInteger (IL_IMAGE_TYPE);
    props.format   = ilGetInteger (IL_IMAGE_FORMAT);

    res = cb->Exec (ilGetData (), props); 

    ilDeleteImage (ilGetInteger (IL_ACTIVE_IMAGE)); 

  }

  ilShutDown ();
  return res; 
}

