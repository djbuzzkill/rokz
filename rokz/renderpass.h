
#ifndef ROKZ_RENDERPASS_H
#define ROKZ_RENDERPASS_H


#include "common.h"
#include "shared_types.h"



namespace rokz {

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  bool CreateRenderPass (RenderPass&             render_pass,
                         VkFormat                swapchain_format,
                         VkSampleCountFlagBits   msaa_samples, 
                         const VkDevice&         device,
                         const VkPhysicalDevice& physdev); 








  

}
#endif
