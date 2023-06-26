
#include "milkshake.h"
#include <vulkan/vulkan_core.h>


using namespace rokz;
using namespace rekz; 
using namespace milkshake; 




// --------------------------------------------------------------------------------------------
//  only 1 geom framebuffer
// --------------------------------------------------------------------------------------------
bool milkshake::setup_gbuff_framebuffer (Glob& glob) {

  Vec<VkImageView> views = {
    glob.gbuff.attachment.position.view->handle, 
    glob.gbuff.attachment.normal.view->handle, 
    glob.gbuff.attachment.albedo.view->handle, 
    glob.gbuff.attachment.depth.view->handle, 
  }; 
  //   } 
    
  glob.gbuff.framebuffer =
    rc::CreateFramebuffer (glob.gbuff.renderpass->handle, views, kDefaultExtent, glob.device); 


  return CHECK_VK_HANDLE(glob.gbuff.framebuffer->handle);  
}

// --------------------------------------------------------------------------------------------
//  lcomp has swapchain_images.size() framebuffers
// --------------------------------------------------------------------------------------------
bool milkshake::setup_lcomp_framebuffers (Glob& glob) {

  uint32 framebuff_count = glob.swapchain_group.views.size();
  glob.lcomp.framebuffers.resize (framebuff_count);

  for (size_t i = 0; i < framebuff_count; i++) {
    //
    Vec<VkImageView> views = {
      glob.swapchain_group.views[i]->handle,
      glob.lcomp.attachment.depth.view->handle
    }; 
    
    glob.lcomp.framebuffers[i] = rc::CreateFramebuffer (glob.lcomp.renderpass->handle, views,
                                                        kDefaultExtent, glob.device); 
    if (!CHECK_VK_HANDLE(glob.lcomp.framebuffers[i]->handle)) {
      HERE("FAILED -> CreateFramebuffer");
      return false;
    }
  }

  return true; 
}
