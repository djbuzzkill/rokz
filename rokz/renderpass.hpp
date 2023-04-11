
#ifndef ROKZ_RENDERPASS_H
#define ROKZ_RENDERPASS_H


#include "common.h"
#include "rokz_types.h"



namespace rokz {

  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  bool CreateRenderPass (RenderPass&             render_pass,
                         VkFormat                swapchain_format,
                         VkSampleCountFlagBits   msaa_samples, 
                         const VkDevice&         device,
                         const VkPhysicalDevice& physdev); 



  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  VkRenderPassCreateInfo& CreateInfo (VkRenderPassCreateInfo&                     ci,
                                      const std::vector<VkAttachmentDescription>& attach_descs,
                                      const std::vector<VkSubpassDescription>&    subpass_descs,
                                      const std::vector<VkSubpassDependency>&     deps); 


  // VkFormat                                    swapchain_format,
  // VkSampleCountFlagBits                       msaa_samples,

  VkAttachmentDescription& ColorAttachment_default  (VkAttachmentDescription& ad);
  VkAttachmentDescription& DepthStencil_default     (VkAttachmentDescription& ad);
  VkAttachmentDescription& PresentSrc_default       (VkAttachmentDescription& ad);

  
  VkSubpassDescription&    SubpassDesc_default     (VkSubpassDescription&    spd); 
  VkSubpassDependency&     SubpassDep_default      (VkSubpassDependency&     dep); 

  


  bool CreateRenderPass (RenderPass&             render_pass,
                         const VkDevice&         device,
                         const VkPhysicalDevice& physdev); 



  
}
#endif
