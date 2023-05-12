
#ifndef ROKZ_RENDERPASS_H
#define ROKZ_RENDERPASS_H


#include "common.hpp"
#include "rokz_types.hpp"
#include <vulkan/vulkan_core.h>



namespace rokz {

  // ------------------------------------------------------------------------------------------
  // 
  // ------------------------------------------------------------------------------------------
  bool CreateRenderPass (RenderPass&             render_pass,
                         VkFormat                swapchain_format,
                         VkSampleCountFlagBits   msaa_samples, 
                         const VkDevice&         device,
                         const VkPhysicalDevice& physdev); 

  // ------------------------------------------------------------------------------------------
  // 
  // ------------------------------------------------------------------------------------------
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

  
  // -- rc -- 
  namespace rc {

    struct RenderPass : public deviceob<VkRenderPass, RenderPass> {

      RenderPass (const Device& dev): deviceob (dev) {
      } 
      
      virtual ~RenderPass () { if (handle) { 
          vkDestroyRenderPass (device.handle, handle, VK_NULL_HANDLE); 
          handle = VK_NULL_HANDLE;
        }}
      
      Vec<VkAttachmentDescription> attach_desc;
      Vec<VkAttachmentReference>   attach_ref;

      Vec<VkSubpassDescription>    subpass_descs;
      Vec<VkSubpassDependency>     dependencies;
    };

    // -- 
    RenderPass::Ref CreateRenderPass (Vec<VkAttachmentDescription>& attach_descs,
                                      Vec<VkAttachmentReference>& attach_refs,
                                      Vec<VkSubpassDescription>& subpdescs,
                                      Vec<VkSubpassDependency>& subpdeps, 
                                      const Device& device); 
  } // rc

  
}
#endif
