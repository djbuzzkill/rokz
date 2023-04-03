
#ifndef REKZ_ONSCREEN_DATA_INCLUDE
#define REKZ_ONSCREEN_DATA_INCLUDE

#include "rekz.h"
#include "rekz/onscreen_pipe.h"


namespace rekz { namespace onscreen {

    // ----------------------------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------------------------
    using namespace rokz; 

    struct Texture {
      rc::Image::Ref     image; 
      rc::ImageView::Ref view; 
      rc::Sampler::Ref   sampler; 
    };

    // ----------------------------------------------------------------------------------------
    struct Data {
      rc::Buffer::Ref                    geom;   // for the rectangle

      Vec<rc::Buffer::Ref>               ubs;   // for the rectangle
      
      Texture                            texture;
      std::array<std::string, kMaxCount> strings; 
    }; 
    
    // ----------------------------------------------------------------------------------------
    bool SetupData (Data& dat, const Device& device);   
    void Cleanup   (Data& dat);   
    
  }}
#endif
