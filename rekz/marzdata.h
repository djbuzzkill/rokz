
#ifndef MARZ_DATA_INCLUDE
#define MARZ_DATA_INCLUDE

#include "rekz.h"

namespace marz {

  using namespace rokz;

  namespace tile { 
    const uint32 x_dim   = 1024;
    const uint32 z_dim   = 1024;
    const uint32 x_count = 6;
    const uint32 z_count = 16;
  }
  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  struct Data {

    rc::Buffer::Ref         devivcebuffer;   
    
    Vec<rc::Image::Ref>     colormaps;
    Vec<rc::ImageView::Ref> colorviews;

    Vec<rc::Image::Ref>     heightmaps;
    Vec<rc::ImageView::Ref> heightviews;

    Vec<rc::Image::Ref>     normalmaps;
    Vec<rc::ImageView::Ref> normalviews;
    
    Sampler                 heightsampler;
    Sampler                 colorsampler;
    Sampler                 normalsampler;

  };

  bool SetupData   (Data& dat);
  void CleanupData (Data& dat, rokz::Device& device);

  
}

#endif
