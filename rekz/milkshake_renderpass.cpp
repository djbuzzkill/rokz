
#include "milkshake.h"
#include <vulkan/vulkan_core.h>


using namespace rokz;
using namespace rekz; 
using namespace milkshake; 

// --------------------------------------------------------------------------------------------
//  
// --------------------------------------------------------------------------------------------
bool milkshake::setup_gbuff_renderpass (Glob& glob) {

  HERE("HAI");
  
  const Device& device = glob.device; 

  Vec<VkAttachmentDescription> attdescs  (NUM_TOTAL_ATTACHMENTS);
  Vec<VkAttachmentReference>   attrefs   (NUM_TOTAL_ATTACHMENTS);   

  ut::FindDepthFormat (glob.gbuff.attachment.depth.format, device.physical.handle); 
  VkFormat color_format = VK_FORMAT_R16G16B16A16_SFLOAT; 
  VkFormat depth_format = glob.gbuff.attachment.depth.format;
  
  // bool rokz::CreateRenderPass ... 
  for (uint32 i = 0; i < NUM_TOTAL_ATTACHMENTS; ++i) { 

    VkImageLayout image_lo = (i == ATTACH_DEPTHI ? // depth layout otherwise color 
                                  VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    // descs
    attdescs[i] = {}; 
    attdescs[i].format         = (ATTACH_DEPTHI == i ? depth_format : color_format); 
    attdescs[i].samples        = VK_SAMPLE_COUNT_1_BIT; // msaa samples
    attdescs[i].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attdescs[i].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    attdescs[i].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attdescs[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attdescs[i].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    attdescs[i].finalLayout    = image_lo;
    // refs
    attrefs[i] = {};
    attrefs[i].attachment      = i;
    attrefs[i].layout          = image_lo;
  }

 
  Vec<VkSubpassDescription> subpdescs (1);
  subpdescs[0] = {}; 
  subpdescs[0].pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpdescs[0].colorAttachmentCount    = NUM_COLOR_ATTACHMENTS;
  subpdescs[0].pColorAttachments       = &attrefs[0];  // co_in, [cr_in] for msaa
  subpdescs[0].inputAttachmentCount    = 0;
  subpdescs[0].pInputAttachments       = nullptr;
  subpdescs[0].pDepthStencilAttachment = &attrefs[ATTACH_DEPTHI]; //nullptr;
  subpdescs[0].preserveAttachmentCount = 0;
  subpdescs[0].pPreserveAttachments    = nullptr;
  subpdescs[0].pResolveAttachments     = nullptr;
  subpdescs[0].flags = 0 ;

  Vec<VkSubpassDependency> subpdeps (2);
  subpdeps[0].srcSubpass    = VK_SUBPASS_EXTERNAL;
  subpdeps[0].dstSubpass    = 0;
  subpdeps[0].srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  subpdeps[0].srcAccessMask = 0;
  subpdeps[0].dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  subpdeps[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;


  subpdeps[1].srcSubpass    = VK_SUBPASS_EXTERNAL;
  subpdeps[1].dstSubpass    = 0;
  subpdeps[1].srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  subpdeps[1].srcAccessMask = 0;
  subpdeps[1].dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  subpdeps[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  glob.gbuff.renderpass = rc::CreateRenderPass (attdescs, subpdescs, subpdeps, device); 
 
  HERE("BAI");
  return true; 

}

// --------------------------------------------------------------------------------------------
//  
// --------------------------------------------------------------------------------------------
bool milkshake::setup_lcomp_renderpass (Glob& glob) {
  
  HERE("HAI");

  const Device& device = glob.device; 
  ut::FindDepthFormat (glob.lcomp.attachment.depth.format, device.physical.handle); 
  //uint32 framebuffer_count  = glob.swapchain_group.images.size (); 
  const uint32                attachment_count = 2;
  const VkFormat              color_format = glob.swapchain_group.format;
  const VkFormat              depth_format = glob.lcomp.attachment.depth.format;
  const VkSampleCountFlagBits sample_count  =  VK_SAMPLE_COUNT_1_BIT;
  
  Vec<VkAttachmentDescription> attdescs  (attachment_count);
  Vec<VkAttachmentReference>   attrefs   (attachment_count);   

  ColorAttachment_default (attdescs[0], color_format, sample_count); 
  AttachmentReference     (attrefs[0], 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL); 

  DepthAttachment_default (attdescs[1], depth_format, sample_count); 
  AttachmentReference     (attrefs[1], 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL); 



  Vec<VkSubpassDescription> subpdescs (1);
  subpdescs[0] = {}; 
  subpdescs[0].pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;

  subpdescs[0].colorAttachmentCount    = 1;
  subpdescs[0].pColorAttachments       = &attrefs[0]; // co_in, [cr_in] for msaa

  subpdescs[0].pDepthStencilAttachment = &attrefs[1]; //nullptr;

  subpdescs[0].inputAttachmentCount    = 0;
  subpdescs[0].pInputAttachments       = nullptr;

  subpdescs[0].preserveAttachmentCount = 0;
  subpdescs[0].pPreserveAttachments    = nullptr;

  subpdescs[0].pResolveAttachments     = nullptr;
  subpdescs[0].flags = 0 ;

  Vec<VkSubpassDependency>  subpdeps (1);
  subpdeps[0].srcSubpass    = VK_SUBPASS_EXTERNAL;
  subpdeps[0].dstSubpass    = 0;
  subpdeps[0].srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  subpdeps[0].srcAccessMask = 0;
  subpdeps[0].dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  subpdeps[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  glob.lcomp.renderpass = rc::CreateRenderPass (attdescs, subpdescs, subpdeps, device); 
  HERE("BAI");
  return CHECK_VK_HANDLE(glob.lcomp.renderpass->handle); 

}
