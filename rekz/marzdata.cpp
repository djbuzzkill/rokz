
#include "marzdata.h"
#include <vulkan/vulkan_core.h>


bool marz::SetupData (marz::Data& dat) {

  dat.devivcebuffer;    // what rly needs  to b in here 

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
