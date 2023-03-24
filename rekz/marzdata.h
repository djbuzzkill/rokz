
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
  struct TextureBundle {
    rc::Image::Ref&     map;
    rc::ImageView::Ref& view;
    rc::Sampler::Ref&   sampler;
  };

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  struct Data {

    rc::Buffer::Ref         devicebuffer;

    Vec<rc::Image::Ref>     colormaps;
    Vec<rc::ImageView::Ref> colorviews;
    rc::Sampler::Ref        colorsampler;

    Vec<rc::Image::Ref>     heightmaps;
    Vec<rc::ImageView::Ref> heightviews;
    rc::Sampler::Ref        heightsampler;

    Vec<rc::Image::Ref>     normalmaps;
    Vec<rc::ImageView::Ref> normalviews;
    rc::Sampler::Ref        normalsampler;
  };

  bool SetupData   (Data& dat, const rokz::Device& device);
  void CleanupData (Data& dat, const rokz::Device& device);
}

#endif
