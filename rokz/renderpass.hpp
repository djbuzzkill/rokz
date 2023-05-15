
#ifndef ROKZ_RENDERPASS_H
#define ROKZ_RENDERPASS_H


#include "common.hpp"
#include "rokz_types.hpp"
#include "rc_types.hpp"
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

  
  VkSubpassDescription&    SubpassDesc_default     (VkSubpassDescription&    spd); 
  VkSubpassDependency&     SubpassDep_default      (VkSubpassDependency&     dep); 


  VkAttachmentReference&
  AttachmentReference (VkAttachmentReference& ref, uint32 index, VkImageLayout layout);

  //
  VkAttachmentDescription& AttachmentDescription (VkAttachmentDescription& desc,              
                                                  VkFormat format, VkSampleCountFlagBits sample_count_bits, 
                                                  VkAttachmentLoadOp loadop, VkAttachmentStoreOp storeop, 
                                                  VkAttachmentLoadOp stencil_loadop, VkAttachmentStoreOp stencil_storeop, 
                                                  VkImageLayout initial_layout, VkImageLayout final_layout); 

  // -- 
  namespace rc {
  rc::RenderPass::Ref CreateRenderPass (const Vec<VkAttachmentDescription>& descs,
                                        const Vec<VkSubpassDescription>&    subpdescs,
                                        const Vec<VkSubpassDependency>&     subpdeps, 
                                        const Device&                       device); 
  }


  #include "renderpass.inl"

}
#endif
