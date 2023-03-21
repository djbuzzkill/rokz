
#include "marzdata.h"


bool marz::SetupData (marz::Data& dat) {

  // struct MarsDat {
  //   const uint32 x_tile_dim = 1024;
  //   const uint32 z_tile_dim = 1024;
  //   const uint32 x_tile_count = 6;
  //   const uint32 z_tile_count = 16;

  //   Buffer         vb_device;
  //   Buffer         ib_device;
  //   Vec<Image>     colormaps;
  //   Vec<ImageView> colorviews;
  //   Vec<Image>     heightmaps;
  //   Vec<ImageView> heightviews;
  //   Vec<Image>     normalmaps;
  //   Vec<ImageView> normalviews;
  //   Sampler        depthsampler;
  //   Sampler        colorsampler;
  //   Sampler        normalsampler;
  // };
 
  dat.devbuf;    // what rly needs  to b in here 

  dat.colormaps;    
  dat.colorviews;   

  dat.heightmaps;   
  dat.heightviews;  

  
  // dat.normalmaps;   
  // dat.normalviews;  

  dat.heightsampler; 
  dat.colorsampler; 

  //dat.normalsampler;
  return false;
}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
void marz::CleanupData (marz::Data& dat, rokz::Device& device) {


}
