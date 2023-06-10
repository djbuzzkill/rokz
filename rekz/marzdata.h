
#ifndef MARZ_DATA_INCLUDE
#define MARZ_DATA_INCLUDE

#include "rekz.h"
#include <vulkan/vulkan.h>

namespace marz {

  using namespace rokz;
  // --------------------------------------------------------------------------
  const auto kMaxNameLen = 64;
  const char k_height_name_format[] = "marz-RED-DEM_x%uz%u.bin";
  const char k_color_name_format[]  = "marz-RED-DRG_x%uz%u.bin";
  const char k_coord_name_format[]  = "marz-RED-IGM_x%uz%u.bin";

  // --------------------------------------------------------------------------
  namespace tile { 

    extern std::filesystem::path basepath;

    const uint32 x_dim   = 1024;
    const uint32 z_dim   = 1024;
    const uint32 x_count = 6;
    const uint32 z_count = 16;

    typedef float colortype; 
    typedef float heighttype;

    const VkFormat heightformat = VK_FORMAT_R32_SFLOAT;
    const VkFormat colorformat  = VK_FORMAT_R32_SFLOAT;

    const size_t sizeofcolor  = sizeof(colortype) * x_dim * z_dim;
    const size_t sizeofheight = sizeof(heighttype) * x_dim * z_dim;
      
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
  }

  // --------------------------------------------------------------------------
  namespace roi { // based on tiles

    const uint32 X_BEG  = 0;
    const uint32 X_LAST = 5;

    const uint32 Z_BEG  = 0;
    const uint32 Z_LAST = 9;

    const uint32 XDim = X_LAST - X_BEG + 1; 
    const uint32 ZDim = Z_LAST - Z_BEG + 1;

    inline uint32 linear_index (uint32 x, uint32 z) {
      return z * XDim + x; 
    } 
  }

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
