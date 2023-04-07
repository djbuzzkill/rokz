
#ifndef REKZ_ONSCREEN_DATA_INCLUDE
#define REKZ_ONSCREEN_DATA_INCLUDE

#include "rekz.h"
#include "rekz/onscreen_pipe.h"
#include "rokz/shared_descriptor.h"


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
      rc::Buffer::Ref             geom;   // for the rectangle
      Texture                     texture;
      std::array<std::string, global_ub::kMaxTextElements> strings; // y not just put directly into textub
    }; 
    
    // ----------------------------------------------------------------------------------------
    bool SetupData (Data& dat, size_t numframes, const Device& device);   
    void Cleanup   (Data& dat);   
    
  }}
#endif
