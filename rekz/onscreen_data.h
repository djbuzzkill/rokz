
#ifndef REKZ_ONSCREEN_DATA_INCLUDE
#define REKZ_ONSCREEN_DATA_INCLUDE

#include "rekz.h"
#include "rekz/onscreen_pipe.h"
#include "rokz/shared_descriptor.hpp"


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

      enum { kMaxTextElements  = 128}; 
      
      rc::Buffer::Ref             geom;   // for the rectangle, separate points per character?
      Texture                     texture;
      std::array<std::string, kMaxTextElements> strings; // y not just put directly into textub

      // ----- not used yet ------------
      // replace strings.
      std::map<std::string, global_ub::TextElement> elementmap; 

    }; 
    
    // ----------------------------------------------------------------------------------------
    bool SetupData (Data& dat, size_t numframes, const Device& device);   
    void Cleanup   (Data& dat);   
    
  }}
#endif
