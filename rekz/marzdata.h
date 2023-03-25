
#ifndef MARZ_DATA_INCLUDE
#define MARZ_DATA_INCLUDE

#include "rekz.h"

namespace marz {

  using namespace rokz;


  const auto kMaxNameLen = 64;
  const char k_height_name_format[] = "marz-RED-DEM_%ux%u.bin";
  const char k_color_name_format[]  = "marz-RED-DRG_%ux%u.bin";
  const char k_coord_name_format[]  = "marz-RED-IGM_%ux%u.bin";

  namespace tile { 
    const uint32 x_dim   = 1024;
    const uint32 z_dim   = 1024;
    const uint32 x_count = 6;
    const uint32 z_count = 16;
  }

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  std::string name_format (const char* name_format, uint32 x, uint32 z); 

  inline std::string height_name (uint32 x, uint32 z) {
    return name_format (k_height_name_format, x, z); 
  }

  inline std::string coord_name (uint32 x, uint32 z) {
    return name_format (k_coord_name_format, x, z); 
  }

  inline std::string color_name (uint32 x, uint32 z) {
    return name_format (k_color_name_format, x, z); 
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
