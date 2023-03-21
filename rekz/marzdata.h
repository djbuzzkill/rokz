
#ifndef MARZ_DATA_INCLUDE
#define MARZ_DATA_INCLUDE

#include "rekz.h"

namespace marz {

  using namespace rokz;
  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  struct Data {

    const uint32 x_tile_dim   = 1024;
    const uint32 z_tile_dim   = 1024;
    const uint32 x_tile_count = 6;
    const uint32 z_tile_count = 16;

    //Buffer         vb_device;
    rc::Buffer::Ref devbuf;   // use 1 buff for errthing on device
    //Buffer         ib_device;
    
    Vec<Image>     colormaps;
    Vec<ImageView> colorviews;

    Vec<Image>     heightmaps;
    Vec<ImageView> heightviews;

    Vec<Image>     normalmaps;
    Vec<ImageView> normalviews;
    
    Sampler        heightsampler;
    Sampler        colorsampler;
    Sampler        normalsampler;

  };

  bool SetupData   (Data& dat);
  void CleanupData (Data& dat, rokz::Device& device);
  

  
}

#endif
