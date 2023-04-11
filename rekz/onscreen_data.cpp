

#include "onscreen_data.h"
#include "onscreen_pipe.h"
#include "rekz.h"
#include "font_tool.h"

#include "rokz/buffer.h"
#include "rokz/image.h"
#include "rokz/rc_buffer.h"
#include "rokz/rc_image.h"
#include "rokz/rc_types.h"
#include "rokz/shared_descriptor.h"
#include "rokz/shared_types.h"
#include <vulkan/vulkan_core.h>


using namespace rokz;
// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
const rekz::onscreen::Vert geom[4] = {
  {{0.0, 1.0, 0.0}, {0.0, 1.0}},
  {{0.0, 0.0, 0.0}, {0.0, 0.0}},
  {{1.0, 1.0, 0.0}, {1.0, 1.0}},
  {{1.0, 0.0, 0.0}, {1.0, 0.0}},
}; 

struct geom_handler : public cx::mappedbuffer_cb {

  geom_handler () {
  }
  
  virtual int on_mapped (void* mappedp, size_t maxsize)  {
    //
    // in the future this cb shuld create quat geom for each character
    // 
    memcpy (mappedp, geom, sizeof(rekz::onscreen::Vert) * 4); 
    
    return 0; 
  }
}; 

// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
struct glyph_layer : public cx::mappedlayer_cb {

  systempath basepath; 
  //
  glyph_layer (): basepath ("/home/djbuzzkill/owenslake/tmp/textbin/") {
  }

  // -- handle glyph image  ---------------------------------------------------------------
  virtual int on_mapped (void* mappedp, size_t maxsize, uint32 layeri, const VkExtent2D& ext) {

    uint8* uc = (uint8*) mappedp;
    float* fpix = (float*)mappedp; 
    rokz::systempath fqpath = basepath/rekz::fonttool::font_glyph_filename (layeri); 

    if (std::filesystem::exists (fqpath)) { 

      Vec<float> fpixels ; 
      From_file (fpixels, fqpath, true); 
      printf ( "loading .....numpixels :%zu  %s\n ", fpixels.size(),  fqpath.c_str ()); 
      std::copy(fpixels.begin(), fpixels.end (), fpix); 
      //std::fill (fpix, fpix + fpixels.size () , 1.0);
    }
    else {
      printf ( "no such file....  %s\n ",  fqpath.c_str ()); 
      std::fill (fpix, fpix + maxsize / sizeof(float) , 0.0);
    }
    
    return 0; 
  }

}; 

// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
bool rekz::onscreen::SetupData (Data& dat, size_t nframesets, const Device& device) {

  // create image
  VkImageCreateInfo ci = {};
  VkExtent2D idim  = { 64, 64 };
  size_t sizeOf_layer = 64 * 64 * sizeof(float);
  uint32 num_layers = 128;

  uint32 layer_begin = '!'; // ascii index of glyph 33
  uint32 layer_end   = '~'; // ascii index of glyph 126
  
  const VkFormat imageformat = VK_FORMAT_R32_SFLOAT; 
  
  cx::CreateInfo_2D_array (ci, imageformat, num_layers,  kSamplingUsage, idim.width, idim.height); 

  dat.texture.image = rc::CreateImage (ci, device); 

  // copy to -> dat.image;
  int res = cx::TransferToImageLayer (dat.texture.image->handle, imageformat, sizeOf_layer,
                                      num_layers, 0, idim,
                                      std::make_shared<glyph_layer> (), device); 

  if (res != 0) {
    printf  ("%s----> result returned %i\n", __FUNCTION__, res); 
    return false;
  }
  //create view
  dat.texture.view = rc::CreateImageView_2D_array (dat.texture.image->handle, imageformat,
                                                    num_layers, device);

  // sampler
  dat.texture.sampler = rc::CreateSampler_default (device);

  // geom
  size_t sizeof_geom = 4 * sizeof(rekz::onscreen::Vert); 
  dat.geom = rc::CreateDeviceBuffer (sizeof_geom,  cx::kDeviceGeometryUsage, device); 

  cx::TransferToDeviceBuffer (dat.geom->handle, sizeof_geom, std::make_shared<geom_handler>(), device); 
  // -------------- default string  ---------------
  for (auto& str : dat.strings) {
    const std::string testtext = "---- test text ----"; 
    str.resize (global_ub::text_item_length); 
    std::copy (testtext.begin (), testtext.end (), str.begin ()); 
  }


  for (int i = 0; auto& el : dat.elementmap) {
    printf ("%i, %f, %f, %s \n", i,  
            el.second.color.x, 
            el.second.position.x, 
            el.second.text.c_str() ); 
    
  }
  
  // c/c++-clang c/c++-gcc c/c++-cppcheck 
  return false; 
}




// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------


void rekz::onscreen::Cleanup (Data& dat) {

  dat.texture.image.reset ();
  dat.texture.view.reset ();
  dat.texture.sampler.reset ();
  dat.geom.reset (); 
}


