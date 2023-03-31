
#ifndef REKZ_ONSCREEN_DATA_INCLUDE
#define REKZ_ONSCREEN_DATA_INCLUDE

#include "rekz.h"





namespace rekz { namespace onscreen {

    using namespace rokz; 

    struct Texture {
      rc::Image::Ref     image; 
      rc::ImageView::Ref view; 
      rc::Sampler::Ref   sampler; 
    };

    
    struct Data {

      Texture         texture;
      rc::Buffer::Ref geom;
      
    }; 
    

  }}
#endif
