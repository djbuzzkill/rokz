
#include "renderpass.hpp"
#include "utility.hpp"
#include <vulkan/vulkan_core.h>



// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
VkRenderPassCreateInfo& rokz::CreateInfo (VkRenderPassCreateInfo&                     ci,
                                          const std::vector<VkAttachmentDescription>& attach_descs,
                                          const std::vector<VkSubpassDescription>&    subpass_descs,
                                          const std::vector<VkSubpassDependency>&     deps) {

  ci  = {}; 
  ci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  ci.flags = 0; 
  ci.pNext = nullptr; 
  // VkAttachmentDescription's
  ci.attachmentCount = attach_descs.size(); 
  ci.pAttachments = &attach_descs[0];
  // VkSubpassDescription's
  ci.subpassCount = subpass_descs.size();
  ci.pSubpasses   = &subpass_descs[0];
  // VkSubpassDependency's
  ci.dependencyCount = deps.size();
  ci.pDependencies   = &deps[0]; 

  return ci; 
}


// --------------------------------------------------------------------------------------------
//  
// --------------------------------------------------------------------------------------------
VkAttachmentReference& rokz::AttachmentReference (VkAttachmentReference& ref,
                                                  uint32 index, VkImageLayout layout)
{
  ref.attachment = index;
  ref.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; 
  return ref; 
}

// --------------------------------------------------------------------------------------------
//  
// --------------------------------------------------------------------------------------------
VkAttachmentDescription& rokz::AttachmentDescription (VkAttachmentDescription& desc,
                                                      VkFormat format, VkSampleCountFlagBits sample_count_bits, 
                                                      VkAttachmentLoadOp loadop, VkAttachmentStoreOp storeop, 
                                                      VkAttachmentLoadOp stencil_loadop, VkAttachmentStoreOp stencil_storeop, 
                                                      VkImageLayout initial_layout, VkImageLayout final_layout)
{
  desc = {}; 
  desc.flags          = 0;                            // VkAttachmentDescriptionFlagBits
  desc.format         = format;                       // VkFormat 
  desc.samples        = sample_count_bits;            // VkSampleCountFlagBits 
  desc.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;  // VkAttachmentLoadOp  
  desc.storeOp        = VK_ATTACHMENT_STORE_OP_STORE; // VkAttachmentStoreOp 
  desc.stencilLoadOp  = stencil_loadop ;              //
  desc.stencilStoreOp = stencil_storeop;              //
  desc.initialLayout  = initial_layout;               // VkImageLayout 
  desc.finalLayout    = final_layout;                 //
  return desc; 
}

// ----------------------------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------------------------
rokz::rc::RenderPass::Ref rokz::rc::CreateRenderPass (const Vec<VkAttachmentDescription>& descs,
                                                      const Vec<VkSubpassDescription>&    subpdescs,
                                                      const Vec<VkSubpassDependency>&     subpdeps, 
                                                      const Device&                       device) { 

  HERE("HAI"); 
  rokz::rc::RenderPass::Ref ret = std::make_shared<rc::RenderPass> (device) ;   
  VkRenderPassCreateInfo  ci {}; 
  CreateInfo (ci, descs, subpdescs, subpdeps); 

  if (vkCreateRenderPass (device.handle, &ci, nullptr, &ret->handle) != VK_SUCCESS) {
    printf ("[FAILED] %s create render pass\n", __FUNCTION__);
    return rc::RenderPass::Ref (0); ; 
  }

  return ret;
}


// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
bool rokz_old_CreateRenderPass (rokz::RenderPass&             render_pass,
                             VkFormat                swapchain_format,
                             VkSampleCountFlagBits   msaa_samples, 
                             const VkDevice&         device,
                             const VkPhysicalDevice& physdev) {
  //printf ("%s\n", __FUNCTION__); 
  using namespace rokz; 
  // COLOR ATTACHMENT | VkAttachmentDescription 
  auto CO_n = ATTACH_COLOR; 
  render_pass.attach_desc[CO_n] = {}; 
  render_pass.attach_desc[CO_n].format         = swapchain_format ;
  render_pass.attach_desc[CO_n].samples        = msaa_samples;  // VK_SAMPLE_COUNT_1_BIT; // msaa samples
  render_pass.attach_desc[CO_n].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  render_pass.attach_desc[CO_n].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
  render_pass.attach_desc[CO_n].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  render_pass.attach_desc[CO_n].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  render_pass.attach_desc[CO_n].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  render_pass.attach_desc[CO_n].finalLayout    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // when msaa is used otherwise -> VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; 

  // DEPTHSTENCIL ATTACHMENT | VkAttachmentDescription 
  auto DP_n = ATTACH_DEPTHSTENCIL;
  render_pass.attach_desc[DP_n] = {}; 
  // render_pass.attach_desc[dp_in].format         = depth_format;
  ut::FindDepthFormat (render_pass.attach_desc[DP_n].format, physdev);
  render_pass.attach_desc[DP_n].samples        = msaa_samples; // VK_SAMPLE_COUNT_1_BIT;
  render_pass.attach_desc[DP_n].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  render_pass.attach_desc[DP_n].storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  render_pass.attach_desc[DP_n].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  render_pass.attach_desc[DP_n].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  render_pass.attach_desc[DP_n].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  render_pass.attach_desc[DP_n].finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  // COLOR RESOLVE ATTACHMENT | VkAttachmentDescription 
  auto CR_n = ATTACH_COLRESOLV; 
  render_pass.attach_desc[CR_n] = {}; 
  render_pass.attach_desc[CR_n].format         = swapchain_format ;
  render_pass.attach_desc[CR_n].samples        = VK_SAMPLE_COUNT_1_BIT; // msaa samples
  render_pass.attach_desc[CR_n].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  render_pass.attach_desc[CR_n].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
  render_pass.attach_desc[CR_n].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  render_pass.attach_desc[CR_n].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  render_pass.attach_desc[CR_n].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  render_pass.attach_desc[CR_n].finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  //VkAttachmentReference color_attachment_ref{};
  render_pass.attach_ref[CO_n] = {};
  render_pass.attach_ref[CO_n].attachment = CO_n; // index
  render_pass.attach_ref[CO_n].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  // depth
  render_pass.attach_ref[DP_n] = {};
  render_pass.attach_ref[DP_n].attachment = DP_n;
  render_pass.attach_ref[DP_n].layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  // 
  render_pass.attach_ref[CR_n] = {};
  render_pass.attach_ref[CR_n].attachment = CR_n; // index
  render_pass.attach_ref[CR_n].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // <--- color opt is correct
                                       // = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  
  // SUBPASS,  VkSubpassDescription                 
  render_pass.subpass_descs.resize (1);
  render_pass.subpass_descs[0] = {}; 
  render_pass.subpass_descs[0].pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
  render_pass.subpass_descs[0].colorAttachmentCount    = 1;
  render_pass.subpass_descs[0].pColorAttachments       = &render_pass.attach_ref[CO_n];  // co_in, [cr_in] for msaa
  render_pass.subpass_descs[0].inputAttachmentCount    = 0;
  render_pass.subpass_descs[0].pInputAttachments       = nullptr;
  render_pass.subpass_descs[0].pDepthStencilAttachment = &render_pass.attach_ref[DP_n]; //nullptr;
  render_pass.subpass_descs[0].preserveAttachmentCount = 0;
  render_pass.subpass_descs[0].pPreserveAttachments    = nullptr;
  render_pass.subpass_descs[0].pResolveAttachments     = &render_pass.attach_ref[CR_n];
  render_pass.subpass_descs[0].flags = 0 ;
  //
  render_pass.dependencies.resize (1);
  render_pass.dependencies[0].srcSubpass    = VK_SUBPASS_EXTERNAL;
  render_pass.dependencies[0].dstSubpass    = 0;

  render_pass.dependencies[0].srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  render_pass.dependencies[0].srcAccessMask = 0;

  render_pass.dependencies[0].dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  render_pass.dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  
  // CREATEINFO. gets passed back out
  render_pass.ci  = {}; 
  render_pass.ci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

  render_pass.ci.attachmentCount = 3; // color + depthstencil + color resolv
  render_pass.ci.pAttachments = &render_pass.attach_desc[0];

  render_pass.ci.subpassCount = 1;
  render_pass.ci.pSubpasses   = &render_pass.subpass_descs[0];

  render_pass.ci.dependencyCount = render_pass.dependencies.size();
  render_pass.ci.pDependencies = &render_pass.dependencies[0]; 
  render_pass.ci.pNext = nullptr; 
  //
  if (vkCreateRenderPass(device, &render_pass.ci, nullptr, &render_pass.handle) != VK_SUCCESS) {
    printf ("[FAILED] %s create render pass\n", __FUNCTION__);
    return false; 
  }

  return true;
}

