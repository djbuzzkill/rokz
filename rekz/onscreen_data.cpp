

#include "onscreen_data.h"
#include "onscreen_pipe.h"
#include "rokz/buffer.h"
#include "rokz/image.h"
#include "rokz/rc_buffer.h"
#include "rokz/rc_image.h"
#include "rokz/rc_types.h"
#include "rokz/shared_descriptor.h"
#include "rokz/shared_types.h"
#include <vulkan/vulkan_core.h>


using namespace rokz;


struct geom_handler : public cx::mappedbuffer_cb {

  geom_handler () { }

  
  virtual int on_mapped (void* mappedp, size_t maxsize)  {

    rekz::onscreen::Vert geom[4] = {
      {{0.0, 0.0, 0.0}, {0.0, 0.0} },
      {{1.0, 0.0, 0.0}, {1.0, 0.0} },
      {{1.0, 1.0, 0.0}, {1.0, 1.0} },
      {{0.0, 1.0, 0.0}, {0.0, 1.0} },
    }; 

    memcpy (mappedp, geom, sizeof(rekz::onscreen::Vert) * 4); 
    
    return 0; 
  }
}; 
// ----------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------
struct glyph_layer : public cx::mappedlayer_cb {

  systempath basepath; 

  glyph_layer (): basepath ("/home/djbuzzkill/owenslake/tmp/testtext/") {
  }
  
  virtual int on_mapped (void* mappedp, size_t maxsize, uint32 layerindex, const VkExtent2D& ext) {

    uint8* uc = (uint8*) mappedp;
    
    if (layerindex >= '!' || layerindex < '~') {
      
      
    }
    else {
      // just black
      std::fill (uc, uc + maxsize , 0);
    }
    
    return 0; 
  }

}; 

// ----------------------------------------------------------------------------------------
bool rekz::onscreen::SetupData (Data& dat, size_t nframesets, const Device& device) {

  VkFormat imageformat = VK_FORMAT_R8_UINT; 


  // create image
  VkImageCreateInfo ci = {};
  VkExtent2D idim  = { 64, 64 };
  size_t sizeOf_layer = 64 *64 * sizeof(byte);
  uint32 num_layers = 128;

  uint32 layer_begin = '!'; // ascii index of glyph 33
  uint32 layer_end   = '~'; // ascii index of glyph 126
  
  cx::CreateInfo_2D_array (ci, imageformat, num_layers,  kSamplingUsage, idim.width, idim.height); 

  dat.texture.image = rc::CreateImage (ci, device); 

  HERE ("YUUH&^");
  // copy to -> dat.image;
  int res = cx::TransferToImageLayer (dat.texture.image->handle, imageformat, sizeOf_layer,
                                      num_layers, layer_begin, idim,
                                      std::make_shared<glyph_layer> (), device); 

  HERE ("YUUH&^");
  if (res != 0) {
    printf  ("%s----> result returned %i\n", __FUNCTION__, res); 
    return false;
  }
  //create view
  dat.texture.view = rc::CreateImageView_2D_array ( dat.texture.image->handle, imageformat,
                                                    num_layers, device);
  HERE ("YUUH&^");

  // sampler
  dat.texture.sampler = rc::CreateSampler_default (device);

  HERE ("YUUH&^");

  // geom
  size_t sizeof_geom = 4 * sizeof(rekz::onscreen::Vert); 
  dat.geom = rc::CreateDeviceBuffer (sizeof_geom,  cx::kDeviceGeometryUsage, device); 

  cx::TransferToDeviceBuffer ( dat.geom->handle,  sizeof_geom, std::make_shared<geom_handler>(), device); 

  // -------------- default string  ---------------
  for (auto& str : dat.strings) {
    str.resize (global_ub::TextItem::max_length, ' ');
    str = "---- test text ----"; 
  }

  // -------------- default string  ---------------
  // const size_t total_ub_size =
  //   UB_sizes[global_ub::TEXTITEMS_BINDINGI];

  // dat.textub.resize (nframesets); // max frame in flight
  // for (size_t iframe = 0; iframe < nframesets; ++iframe) { 
  //   dat.textub[iframe] = rc::Create_uniform_mapped (total_ub_size, 1, device); 
  // }

  //c/c++-clang c/c++-gcc c/c++-cppcheck 
  return false; 
}


void rekz::onscreen::Cleanup (Data& dat) {

  
  dat.texture.image.reset ();
  dat.texture.view.reset ();
  dat.texture.sampler.reset ();
  dat.geom.reset (); 

  

}


