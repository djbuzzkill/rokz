

#include "onscreen_data.h"
#include "onscreen_pipe.h"
#include "rokz/buffer.h"
#include "rokz/image.h"
#include "rokz/rc_buffer.h"
#include "rokz/rc_image.h"
#include "rokz/rc_types.h"
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
bool rekz::onscreen::SetupData (Data& dat, const Device& device) {

  VkFormat imageformat = VK_FORMAT_R8_UINT; 


  // create image
  VkImageCreateInfo ci = {};
  VkExtent2D idim  = { 64, 64 };
  size_t sizeOf_layer = 64 *64 * sizeof(byte);
  uint32 num_layers = 256;

  uint32 layer_begin = '!'; // ascii index of glyph 33
  uint32 layer_end   = '~'; // ascii index of glyph 126
  
  cx::CreateInfo_2D_array (ci, imageformat, num_layers,  kSamplingUsage, idim.width, idim.height); 

  dat.texture.image = rc::CreateImage (ci, device); 

  // copy to -> dat.image;
  int res = cx::TransferToImageLayer (dat.texture.image->handle, imageformat, sizeOf_layer,
                                      num_layers, layer_begin, idim,
                                      std::make_shared<glyph_layer> (), device); 

  //create view
  dat.texture.view = rc::CreateImageView_2D_array ( dat.texture.image->handle, imageformat,
                                                    num_layers, device);

  // sampler
  dat.texture.sampler = rc::CreateSampler_default (device);


  // geom
  size_t sizeof_geom = 4 * sizeof(rekz::onscreen::Vert); 
  dat.geom = rc::CreateDeviceBuffer (sizeof_geom,  cx::kDeviceGeometryUsage, device); 

  cx::TransferToDeviceBuffer ( dat.geom->handle,  sizeof_geom, std::make_shared<geom_handler>(), device); 


  //c/c++-clang c/c++-gcc c/c++-cppcheck 

  return false; 
}


void rekz::onscreen::Cleanup (Data& dat) {

  
  dat.texture.image.reset ();
  dat.texture.view.reset ();
  dat.texture.sampler.reset ();
  dat.geom.reset (); 

  

}


