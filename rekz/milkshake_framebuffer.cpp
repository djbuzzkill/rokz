
#include "milkshake.h"


using namespace rokz;
using namespace rekz; 
using namespace milkshake; 




// --------------------------------------------------------------------------------------------
//  only 1 geom framebuffer
// --------------------------------------------------------------------------------------------
bool setup_gbuff_framebuffer (Glob& glob) {

  Vec<VkImageView> views = {
    glob.gbuff.attachment.position.view->handle, 
    glob.gbuff.attachment.normal.view->handle, 
    glob.gbuff.attachment.albedo.view->handle, 
    glob.gbuff.attachment.depth.view->handle, 
  }; 
  //   } 
    
  glob.gbuff.framebuffer =
    rc::CreateFramebuffer (glob.gbuff.renderpass->handle, views, kDefaultDimensions, glob.device); 
  
  return false; 
}

// --------------------------------------------------------------------------------------------
//  lcomp has swapchain_images.size() framebuffers
// --------------------------------------------------------------------------------------------
bool setup_lcomp_framebuffers (Glob& glob) {

  // surface color target
  // depth lcmop target
  uint32 framebuff_count =   glob.swapchain_group.views.size(); ; 
  
  Vec<VkImageView> views; //  = glob.swapchain_group.views;

  glob.lcomp.framebuffers.resize (framebuff_count);
  for (size_t i = 0; i < framebuff_count; i++) {

    views.push_back (glob.swapchain_group.views[i]->handle);
    views.push_back (glob.depth.view->handle);
    
    glob.lcomp.framebuffers[i] =
        rc::CreateFramebuffer (glob.gbuff.renderpass->handle, views, kDefaultDimensions, glob.device); 

  }

  return false; 
}
