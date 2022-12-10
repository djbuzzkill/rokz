
#include "context.h"
#include "image.h"


const bool kEnableValidationLayers = true;
  //#endif



const std::vector<const char*> validation_layers  = {
  "VK_LAYER_KHRONOS_validation"
};


const std::vector<const char*>& GetValidationLayers () {
  return validation_layers; 
}

// kittyCAD API Token: 
// b78b74a3-2183-45f6-8bf1-4b996e9a825b


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
//int rokz::CreateInstance (Instance, appinfo, createinfo)

int rokz::CreateInstance (VkInstance& instance, VkApplicationInfo& app_info, VkInstanceCreateInfo& inst_info) {

  // VkApplicationInfo
  app_info.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "Hello Triangle";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName      = "No Engine";
  app_info.engineVersion    = VK_MAKE_VERSION(1, 1, 0);
  app_info.apiVersion       = VK_API_VERSION_1_1;
  //glob.app_info.pNext = nullptr; 

  // VkInstanceCreateInfo
  std::vector<const char*> req_exts; 
  rokz::GetRequiredExtensionNames (req_exts);  

  inst_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  inst_info.pApplicationInfo = &app_info;
  // 
  inst_info.enabledExtensionCount = req_exts.size();
  inst_info.ppEnabledExtensionNames = &req_exts[0];
  inst_info.pNext = nullptr; 


  if (kEnableValidationLayers && CheckValidationSupport (validation_layers)) {
    printf ("ENABLE VALIDATION LAYERS\n"); 
    inst_info.enabledLayerCount   = validation_layers.size(); 
    inst_info.ppEnabledLayerNames = &validation_layers[0]; 
    // !! SETUP up additional output cb handling...
  }
  else {
    printf ("VALIDATION LAYERS ARE DISABLED\n"); 
    inst_info.enabledLayerCount   = 0;
    inst_info.ppEnabledLayerNames = nullptr;
  }

  // CREATEINSTANCE
  printf ("vkCreateInstance() \n"); 
  if (vkCreateInstance (&inst_info, nullptr, &instance) != VK_SUCCESS) {
    printf("failed to create instance!");
    return __LINE__;
  }
  
  printf("WoooooOOOooooOoooooooOOoOoOOOoooOoOOOOOOO!!!1\n");
  // ENUMERATEINSTANCEEXTENSIONPROPERTIES
  uint32_t ext_count = 0;
  vkEnumerateInstanceExtensionProperties (nullptr, &ext_count, nullptr);

  printf("   num extensions[%u]\n", ext_count);
  std::vector<VkExtensionProperties> extensions(ext_count);
  vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, extensions.data());

  std::cout << "available extensions:\n";
  for (const auto &extension : extensions) {
    std::cout << '\t' << extension.extensionName << '\n';
  }
  
  return 0;
}







VkDeviceQueueCreateInfo& rokz::Default (VkDeviceQueueCreateInfo& info,
                                        uint32_t que_fam_index,
                                        float* q_priority){

  info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; 
  info.queueFamilyIndex = que_fam_index ; // glob.queue_fams.graphics.value();
  info.queueCount       = 1; 
  info.pQueuePriorities = q_priority;

  return info; 
}

VkSwapchainCreateInfoKHR& rokz::Default (VkSwapchainCreateInfoKHR& info,
                                         const VkSurfaceKHR&       surf) {
  
  info.sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  info.surface = surf;

  return info; 
}

VkDeviceCreateInfo& rokz::Default (VkDeviceCreateInfo&       info,
                                   VkDeviceQueueCreateInfo*  quecreateinfo,
                                   VkPhysicalDeviceFeatures* devfeats) {


  const std::vector<const char*>& device_extensions = GetDeviceExtensionNames ();
  
  info.sType                 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  info.enabledExtensionCount = static_cast<uint32_t> (device_extensions.size ());
  info.ppEnabledExtensionNames= &device_extensions[0]; 
  info.queueCreateInfoCount  = 1;   
  info.pQueueCreateInfos     = quecreateinfo; /// &glob.create_info.queue;
  info.pEnabledFeatures      = devfeats; // &glob.device_features;
  info.enabledLayerCount     = validation_layers.size();   
  info.ppEnabledLayerNames   = &validation_layers[0]; 
  return info;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateLogicalDevice (
    VkDevice*                 device,
    const VkDeviceCreateInfo* createinfo,
    const VkPhysicalDevice&   physdev) {
  printf ("%s", __FUNCTION__);

  VkResult res = vkCreateDevice (physdev, createinfo, nullptr, device);

  printf ("...%s\n", VK_SUCCESS == res ? "SUCCESS" : "FAILED"); 

  return VK_SUCCESS == res; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateSurface (VkSurfaceKHR* surf, GLFWwindow* glfwin, const VkInstance& inst) {
  printf ("%s", __FUNCTION__);

  if (glfwCreateWindowSurface (inst, glfwin, nullptr, surf) != VK_SUCCESS) {
    printf ("...FAILED\n"); 
    return false; 
    //throw std::runtime_error("failed to create window surface!");
  }

  printf ("...SUCCESS\n"); 
  return true; 
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkSurfaceFormatKHR rokz::ChooseSwapSurfaceFormat (const std::vector<VkSurfaceFormatKHR>& available_formats) {
  for (const auto& f : available_formats) {
    if (f.format == VK_FORMAT_B8G8R8A8_SRGB && f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return f;
    }
  }

  return available_formats[0]; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkPresentModeKHR rokz::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &available_modes) {

  for (const auto& mode : available_modes) {
    if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return mode;
    }
  }
  
  return VK_PRESENT_MODE_FIFO_KHR;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkExtent2D rokz::ChooseSwapExtent (const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* win) {

  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  }
  else {
    int width, height;
    glfwGetFramebufferSize (win, &width, &height);
    
    VkExtent2D actual_extent = {
      static_cast<uint32_t>(width),
      static_cast<uint32_t>(height)
    };
    
    actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    
    return actual_extent;
  }
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateSwapchain (VkSwapchainKHR& swapchain, 
                            VkSwapchainCreateInfoKHR& swapchaincreateinfo,
                            const VkSurfaceKHR& surf,
                            const VkPhysicalDevice& physdev, 
                            const VkDevice& dev, 
                            GLFWwindow* glfwin) {
  printf ("%s", __FUNCTION__);

  rokz::SwapchainSupportInfo swapchain_supp_info {};
  QuerySwapchainSupport (swapchain_supp_info, surf, physdev); 
  
  Default (swapchaincreateinfo, surf); 
  VkSurfaceFormatKHR surface_format = ChooseSwapSurfaceFormat (swapchain_supp_info.formats);
  VkPresentModeKHR   present_mode   = ChooseSwapPresentMode   (swapchain_supp_info.present_modes);
  VkExtent2D         extent         = ChooseSwapExtent        (swapchain_supp_info.capabilities, glfwin);
  

  uint32_t image_count = swapchain_supp_info.capabilities.minImageCount + 1; 
  if (swapchain_supp_info.capabilities.maxImageCount > 0 && image_count > swapchain_supp_info.capabilities.maxImageCount) {
    image_count = swapchain_supp_info.capabilities.maxImageCount;
  }

  swapchaincreateinfo.minImageCount = image_count;
  swapchaincreateinfo.imageFormat = surface_format.format;
  swapchaincreateinfo.imageColorSpace = surface_format.colorSpace;
  swapchaincreateinfo.imageExtent = extent;
  swapchaincreateinfo.imageArrayLayers = 1;
  swapchaincreateinfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  
  QueueFamilyIndices que_fam_inds; 
  FindQueueFamilies (que_fam_inds, surf, physdev); 

  uint32_t fam_inds[] = {

    que_fam_inds.graphics.value(),
    que_fam_inds.present.value (),
    
  }; 
  
  if (que_fam_inds.graphics != que_fam_inds.present) {
    printf ("[VK_SHARING_MODE_CONCURRENT]\n");
    swapchaincreateinfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    swapchaincreateinfo.queueFamilyIndexCount = 2;
    swapchaincreateinfo.pQueueFamilyIndices = fam_inds;
  } else {
    printf ("[VK_SHARING_MODE_EXCLUSIVE]\n");
    swapchaincreateinfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchaincreateinfo.queueFamilyIndexCount = 0;     
    swapchaincreateinfo.pQueueFamilyIndices = nullptr; 
  }

  swapchaincreateinfo.preTransform   = swapchain_supp_info.capabilities.currentTransform;
  swapchaincreateinfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

  swapchaincreateinfo.presentMode    = present_mode; 
  swapchaincreateinfo.clipped        = VK_TRUE;
  swapchaincreateinfo.oldSwapchain   = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR (dev, &swapchaincreateinfo, nullptr, &swapchain) != VK_SUCCESS) {
    printf ("failed to create swap chain!\n");
    printf ("LEAVING[FALSE] %s\n", __FUNCTION__);
    return false; 
  }

  printf ("LEAVING[TRUE] %s\n", __FUNCTION__);
  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::GetSwapChainImages (std::vector<VkImage> &swapchain_images,
                               VkSwapchainKHR& swapchain,
                               const VkDevice& dev) {
  printf ("%s\n", __FUNCTION__);

  uint32_t image_count = 0; 

  VkResult res;
  res = vkGetSwapchainImagesKHR(dev, swapchain, &image_count, nullptr);
  
  if (res != VK_SUCCESS) {
    printf ("LEAVING[FALSE] after image_count %s\n", __FUNCTION__);
    return false;
  }
  printf ( "no. swapchain images[%u]\n", image_count); 

  swapchain_images.resize(image_count);
  res = vkGetSwapchainImagesKHR (dev, swapchain, &image_count, &swapchain_images[0]);
  if (res != VK_SUCCESS) {
    printf ("LEAVING[FALSE] after swapchain images %s\n", __FUNCTION__);
    return false;
  }

  printf ("LEAVING[TRUE] %s\n", __FUNCTION__);
  return true;
}

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
VkImageViewCreateInfo Default (VkImageViewCreateInfo& createinfo, 
                               const VkImage&         image,
                               VkFormat               fmt) {
  
  printf ("[TODO] use -> image.cpp::Init (VkImageViewCreateInfo&\n"); 
  createinfo = {}; 
  createinfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  createinfo.image = image;
  createinfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  createinfo.format = fmt; 
  createinfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  createinfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  createinfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  createinfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

  createinfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  createinfo.subresourceRange.baseMipLevel = 0;
  createinfo.subresourceRange.levelCount = 1;
  createinfo.subresourceRange.baseArrayLayer = 0;
  createinfo.subresourceRange.layerCount = 1;

  return createinfo; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateImageViews (std::vector<VkImageView>&  swapchain_imageviews,
                            const std::vector<VkImage>& swapchain_images,
                            VkFormat                    surf_fmt, 
                            const VkDevice& dev) {
  printf ("%s\n", __FUNCTION__); 
  
  swapchain_imageviews.resize (swapchain_images.size()); 
  
  for (size_t i = 0; i < swapchain_imageviews.size(); i++) {

    VkImageViewCreateInfo ci = {};

    //Init (createinfo, VK_IMAGE_ASPECT_COLOR_BIT, swapchain_images[i] ); 
    //    Default (createinfo, swapchain_images[i], surf_fmt); 
    rokz::Image image_temp {};

    image_temp.handle = swapchain_images[i];
    image_temp.ci.format = surf_fmt;
    
    Init (ci, VK_IMAGE_ASPECT_COLOR_BIT, image_temp); 
    
    VkResult res = vkCreateImageView (dev, &ci, nullptr, &swapchain_imageviews[i]);
    if (res != VK_SUCCESS) {
      printf ("[FAILED] %s create imageview \n", __FUNCTION__); 
      return false; 
    }
  }

  printf ("BAI %s\n", __FUNCTION__); 
  return true;   
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateDynamicStates (std::vector<VkDynamicState>& dynamic_states, VkPipelineDynamicStateCreateInfo& dynamic_state_create_info) {

  // DYNAMIC STATE
  dynamic_states.clear ();
  dynamic_states.push_back (VK_DYNAMIC_STATE_VIEWPORT);
  dynamic_states.push_back (VK_DYNAMIC_STATE_SCISSOR);
  
  dynamic_state_create_info =  {};
  dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state_create_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
  dynamic_state_create_info.pDynamicStates = &dynamic_states[0];

  return true; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateColorBlendState (VkPipelineColorBlendAttachmentState& color_blend_attachment_state,
                                  VkPipelineColorBlendStateCreateInfo& color_blending_create_info) {
  // COLOR BLENDING
  color_blend_attachment_state = {};
  color_blend_attachment_state.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  color_blend_attachment_state.blendEnable = VK_FALSE;
  color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; 
  color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; 
  color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD; 
  color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; 
  color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD; 
  // Create Info
  color_blending_create_info = {};
  color_blending_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blending_create_info.logicOpEnable = VK_FALSE;
  color_blending_create_info.logicOp = VK_LOGIC_OP_COPY; 
  color_blending_create_info.attachmentCount = 1;
  color_blending_create_info.pAttachments = &color_blend_attachment_state;
  color_blending_create_info.blendConstants[0] = 0.0f; 
  color_blending_create_info.blendConstants[1] = 0.0f; 
  color_blending_create_info.blendConstants[2] = 0.0f; 
  color_blending_create_info.blendConstants[3] = 0.0f; 
  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateRenderPass (RenderPass &render_pass, VkFormat swapchain_format, const VkDevice &device, const VkPhysicalDevice& physdev) {
  //printf ("%s\n", __FUNCTION__); 

  // COLOR ATTACHMENT
  auto co_in = RenderPass::COLOR; 
  render_pass.attach_desc[co_in] = {}; 
  render_pass.attach_desc[co_in].format = swapchain_format ;
  render_pass.attach_desc[co_in].samples = VK_SAMPLE_COUNT_1_BIT;
  render_pass.attach_desc[co_in].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  render_pass.attach_desc[co_in].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  render_pass.attach_desc[co_in].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  render_pass.attach_desc[co_in].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  render_pass.attach_desc[co_in].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  render_pass.attach_desc[co_in].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  auto dp_in = RenderPass::DEPTHSTENCIL;
  VkFormat depth_format; 
  rokz::FindDepthFormat (depth_format ,  physdev); 
  render_pass.attach_desc[dp_in] = {}; 
  render_pass.attach_desc[dp_in].format = depth_format;
  render_pass.attach_desc[dp_in].samples = VK_SAMPLE_COUNT_1_BIT;
  render_pass.attach_desc[dp_in].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  render_pass.attach_desc[dp_in].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  render_pass.attach_desc[dp_in].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  render_pass.attach_desc[dp_in].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  render_pass.attach_desc[dp_in].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  render_pass.attach_desc[dp_in].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


  //VkAttachmentReference color_attachment_ref{};
  render_pass.attach_ref[co_in] = {};
  render_pass.attach_ref[co_in].attachment = co_in; // index
  render_pass.attach_ref[co_in].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  render_pass.attach_ref[dp_in] = {};
  render_pass.attach_ref[dp_in].attachment = dp_in;
  render_pass.attach_ref[dp_in].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


  // SUBPASS,  VkSubpassDescription                 
  render_pass.subpass_descs.resize (1);
  render_pass.subpass_descs[0] = {};
  render_pass.subpass_descs[0].pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
  render_pass.subpass_descs[0].colorAttachmentCount    = 1;
  render_pass.subpass_descs[0].pColorAttachments       = &render_pass.attach_ref[co_in] ;
  render_pass.subpass_descs[0].inputAttachmentCount    = 0;
  render_pass.subpass_descs[0].pInputAttachments       = nullptr;
  render_pass.subpass_descs[0].pDepthStencilAttachment = &render_pass.attach_ref[dp_in]; //nullptr;
  render_pass.subpass_descs[0].preserveAttachmentCount = 0;
  render_pass.subpass_descs[0].pPreserveAttachments    = nullptr;
  render_pass.subpass_descs[0].pResolveAttachments     = nullptr;
  render_pass.subpass_descs[0].flags = 0 ;
  //
  render_pass.dependancy = {};
  render_pass.dependancy.srcSubpass    = VK_SUBPASS_EXTERNAL;
  render_pass.dependancy.dstSubpass    = 0;
  render_pass.dependancy.srcStageMask  =  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  render_pass.dependancy.srcAccessMask = 0;
  render_pass.dependancy.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  render_pass.dependancy.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
  
  // CREATEINFO. gets passed back out
  render_pass.create_info = {}; 
  render_pass.create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass.create_info.attachmentCount = 2; // color + depthstencil
  render_pass.create_info.pAttachments = render_pass.attach_desc;
  render_pass.create_info.subpassCount = 1;
  render_pass.create_info.pSubpasses = &render_pass.subpass_descs[0];
  render_pass.create_info.dependencyCount = 1;
  render_pass.create_info.pDependencies = &render_pass.dependancy;
  render_pass.create_info.pNext = nullptr; 
  //
  if (vkCreateRenderPass(device, &render_pass.create_info, nullptr, &render_pass.handle) != VK_SUCCESS) {
    printf ("[FAILED] %s create render pass\n", __FUNCTION__);
    return false; 
  }

  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateGraphicsPipelineLayout (
    VkPipelineLayout&            pipeline_layout,
    VkPipelineLayoutCreateInfo&  create_info, 
    const VkDescriptorSetLayout& desc_set_layout, 
    const VkDevice&              device)
{
  // PIPELINE LAYOUT CREATE INFO << mostly empty for now
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

  create_info.setLayoutCount         = 1;            
  create_info.pSetLayouts            = &desc_set_layout;         


  create_info.setLayoutCount         = 1;            
  create_info.pSetLayouts            = &desc_set_layout;         
  //printf ("NOTE: %s [Descriptor Set Layout INACTIVE]\n", __FUNCTION__); 

  create_info.pushConstantRangeCount = 0;    
  create_info.pPushConstantRanges = nullptr; 

  if (vkCreatePipelineLayout (device, &create_info, nullptr, &pipeline_layout) != VK_SUCCESS) {
    printf("FAILED _create pipeline layout_\n");
    return false;
  }

  return true;
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
// bool rokz::CreateRenderPass(VkRenderPass &render_pass,
//                             VkRenderPassCreateInfo &create_info,
//                             VkFormat swapchain_format, const VkDevice &device) {


//   // COLOR ATTACHMENT
//   VkAttachmentDescription color_attachment{};
//   color_attachment.format = swapchain_format ;
//   color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
//   color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//   color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//   color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//   color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//   color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//   color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
//   // 
//   VkAttachmentReference color_attachment_ref{};
//   color_attachment_ref.attachment = 0;
//   color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//   // SUBPASS 
//   VkSubpassDescription subpass{};
//   subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

//   subpass.colorAttachmentCount = 1;
//   subpass.pColorAttachments = &color_attachment_ref;

//   subpass.inputAttachmentCount = 0;
//   subpass.pInputAttachments = nullptr;

//   subpass.pDepthStencilAttachment = nullptr;

//   subpass.preserveAttachmentCount = 0;
//   subpass.pPreserveAttachments = nullptr;

//   subpass.pResolveAttachments = nullptr;
//   subpass.flags = 0 ; 

//   // CREATEINFO. gets passed back out
//   create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//   create_info.attachmentCount = 1;
//   create_info.pAttachments = &color_attachment;
//   create_info.subpassCount = 1;
//   create_info.pSubpasses = &subpass;

//   if (vkCreateRenderPass(device, &create_info, nullptr, &render_pass) != VK_SUCCESS) {
//     printf ("failed to create render pass!\n");
//     return false; 
//   }

//   return true;
// }

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------xs
bool rokz::CreateFramebuffers (
    std::vector<VkFramebuffer>&           framebuffers,
    std::vector<VkFramebufferCreateInfo>& create_infos,
    const RenderPass&                   render_pass, 
    const VkExtent2D                      swapchain_ext, 
    const std::vector<VkImageView>&       image_views, 
    const VkImageView&                    depth_imageview, 
    const VkDevice&                       device) {

  framebuffers.resize (image_views.size()); 
  create_infos.resize (image_views.size()); 
  

  for (size_t i = 0; i < image_views.size(); i++) {

    VkImageView attachments[] = {image_views[i], depth_imageview};

    create_infos[i]  = {}; 
    create_infos[i].sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    create_infos[i].renderPass = render_pass.handle;
    create_infos[i].attachmentCount = 2; // color + depthstencil
    create_infos[i].pAttachments = attachments;
    create_infos[i].width = swapchain_ext.width;
    create_infos[i].height = swapchain_ext.height;
    create_infos[i].layers = 1;

    if (vkCreateFramebuffer(device, &create_infos[i], nullptr, &framebuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create framebuffer!");
    }
  }

  return true; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateCommandPool (VkCommandPool&            command_pool,
                             VkCommandPoolCreateInfo&  create_info,
                             const QueueFamilyIndices& queue_family_inds, 
                             const VkDevice& device ) {
  // 
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  create_info.queueFamilyIndex = queue_family_inds.graphics.value ();

  if (vkCreateCommandPool(device, &create_info, nullptr, &command_pool) != VK_SUCCESS) {
    printf ("failed to create command pool!");
    return false; 
  }

  return true; 
}

// bool rokz::CreateVertexBuffer (BufferStruc& buffstruc, 
//                                size_t sizeof_elem,
//                                size_t num_elem, 
//                                const VkDevice& device,
//                                const VkPhysicalDevice& physdev) {

//   buffstruc.create_info = {};
//   buffstruc.create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//   buffstruc.create_info.size = sizeof_elem *  num_elem; 
    
//   buffstruc.create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
//   buffstruc.create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

//   if (vkCreateBuffer(device, &buffstruc.create_info, nullptr, &buffstruc.handle) != VK_SUCCESS) {
//     printf ("[%s] VB create failed", __FUNCTION__);
//     return false; 
//   }

//   VkMemoryRequirements vb_mem_reqs;
//   vkGetBufferMemoryRequirements(device, buffstruc.handle, &vb_mem_reqs);

//   uint32_t mem_type_filter = 0;
//   VkMemoryPropertyFlags mem_prop_flags; 
//   rokz::FindMemoryType (mem_type_filter, mem_prop_flags, physdev); 

//   buffstruc.alloc_info = {}; 
//   buffstruc.alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//   buffstruc.alloc_info.allocationSize = vb_mem_reqs.size;
//   buffstruc.alloc_info.memoryTypeIndex =
//     rokz::FindMemoryType(vb_mem_reqs.memoryTypeBits,
//                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//                          physdev);

//   if (vkAllocateMemory(device, &buffstruc.alloc_info, nullptr, &buffstruc.mem) != VK_SUCCESS) {
//     printf ("[%s]  VB  mem-alloc failed\n", __FUNCTION__);
//     return false;
//   }

//   if (VK_SUCCESS != vkBindBufferMemory (device, buffstruc.handle, buffstruc.mem, 0)) {
//     printf ("[%s]  VB memory bind failed\n", __FUNCTION__);
//     return false;
//   }
  
//   return true; 
// }


// void createVertexBuffer() {
//     VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

//     VkBuffer stagingBuffer;
//     VkDeviceMemory stagingBufferMemory;
//     createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

//     void* data;
//     vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
//         memcpy(data, vertices.data(), (size_t) bufferSize);
//     vkUnmapMemory(device, stagingBufferMemory);

//     createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
// }

// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateCommandBuffer(VkCommandBuffer &command_buffer,
                               VkCommandBufferAllocateInfo& create_info, 
                               const VkCommandPool &command_pool,
                               const VkDevice &device) {

  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  create_info.commandPool = command_pool;
  create_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  create_info.commandBufferCount = 1;

  if (vkAllocateCommandBuffers(device, &create_info, &command_buffer) != VK_SUCCESS) {
    printf ("failed to allocate command buffers!");
    return false; 
  }
  printf ("BAI %s\n", __FUNCTION__); 
  return true; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::RecordCommandBuffer(VkCommandBuffer &command_buffer,
                               const VkPipeline pipeline,
                               const VkBuffer& vertex_buffer, 
                               const VkExtent2D &ext2d,
                               const VkFramebuffer &framebuffer,
                               const RenderPass &render_pass,
                               const VkDevice &device) {

  VkCommandBufferBeginInfo begin_info {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = 0;                  // 
  begin_info.pInheritanceInfo = nullptr; // 

  if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
     printf ("failed to begin recording command buffer!");
     return false; 
  }
  // begin command list
  VkRenderPassBeginInfo pass_info{};
  VkClearValue clear_values[2] = {};
  clear_values[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  clear_values[1].depthStencil = {1.0f, 0};
  
  pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  pass_info.renderPass = render_pass.handle; 
  pass_info.framebuffer = framebuffer; 
  pass_info.renderArea.offset = {0, 0};
  pass_info.renderArea.extent = ext2d;
  pass_info.clearValueCount = 2; 
  pass_info.pClearValues = clear_values; 

  vkCmdBeginRenderPass (command_buffer, &pass_info, VK_SUBPASS_CONTENTS_INLINE);

  vkCmdBindPipeline (command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(ext2d.width);
  viewport.height = static_cast<float>(ext2d.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(command_buffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = ext2d;
  vkCmdSetScissor(command_buffer, 0, 1, &scissor);


  VkBuffer vertex_buffers[] = {vertex_buffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);

  
  vkCmdDraw (command_buffer, 3, 1, 0, 0);
  
  vkCmdEndRenderPass(command_buffer);

  // end 
  if (vkEndCommandBuffer (command_buffer) != VK_SUCCESS) {
    printf ("failed to record command buffer!");
    return false; 
  }
  
  //printf ("BAI %s\n", __FUNCTION__); 
  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::RecordCommandBuffer_indexed (VkCommandBuffer        &command_buffer,
                                        const Pipeline&        pipeline,
                                        const VkDescriptorSet& desc_set, 
                                        const VkBuffer&        vertex_buffer, 
                                        const VkBuffer&        index_buffer, 
                                        const VkExtent2D &ext2d,
                                        const VkFramebuffer &framebuffer,
                                        const RenderPass &render_pass,
                                        const VkDevice &device) {

  VkCommandBufferBeginInfo begin_info {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = 0;                  // 
  begin_info.pInheritanceInfo = nullptr; // 

  if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
     printf ("failed to begin recording command buffer!");
     return false; 
  }
  // begin command list
  VkRenderPassBeginInfo pass_info{};
  VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}}; 
  pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  pass_info.renderPass = render_pass.handle; 
  pass_info.framebuffer = framebuffer; 
  pass_info.renderArea.offset = {0, 0};
  pass_info.renderArea.extent = ext2d;
  pass_info.clearValueCount = 1; 
  pass_info.pClearValues = &clear_color; 

  vkCmdBeginRenderPass (command_buffer, &pass_info, VK_SUBPASS_CONTENTS_INLINE);

  vkCmdBindPipeline (command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.handle);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(ext2d.width);
  viewport.height = static_cast<float>(ext2d.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(command_buffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = ext2d;
  vkCmdSetScissor(command_buffer, 0, 1, &scissor);


  VkBuffer vertex_buffers[] = {vertex_buffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);


  vkCmdBindIndexBuffer(command_buffer, index_buffer, 0, VK_INDEX_TYPE_UINT16);

  vkCmdBindDescriptorSets (command_buffer,
                           VK_PIPELINE_BIND_POINT_GRAPHICS,
                           pipeline.layout.handle,
                           0,
                           1,
                           &desc_set, //&descriptorSets[currentFrame],
                           0,
                           nullptr);
  
  vkCmdDrawIndexed (command_buffer, 12, 1, 0, 0, 0);
  
  vkCmdEndRenderPass(command_buffer);

  // end 
  if (vkEndCommandBuffer (command_buffer) != VK_SUCCESS) {
    printf ("failed to record command buffer!");
    return false; 
  }
  
  //printf ("BAI %s\n", __FUNCTION__); 
  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateSyncObjs (SyncStruc&      sync,
                           SyncCreateInfo& create_info,
                           const VkDevice& device) {

  create_info.semaphore = {};
  create_info.semaphore.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  create_info.semaphore.pNext = nullptr;
  
  create_info.fence.sType ={}; 
  create_info.fence.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  create_info.fence.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  create_info.fence.pNext = nullptr;
  if (vkCreateSemaphore(device, &create_info.semaphore, nullptr, &sync.image_available_sem) != VK_SUCCESS
      || vkCreateSemaphore(device, &create_info.semaphore, nullptr, &sync.render_fnished_sem) != VK_SUCCESS
      || vkCreateFence(device, &create_info.fence, nullptr, &sync.in_flight_fen) != VK_SUCCESS)
    {
      printf ("failed to create semaphores!");
      return false; 
    }
  
  printf ("BAI %s\n", __FUNCTION__); 
  return true; 
}



// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
void rokz::GetDeviceQueue (VkQueue* que, uint32_t fam_ind, const VkDevice& device) { 
  const uint32_t que_index = 0; // diffrnt than que_fam_ind
  return vkGetDeviceQueue (device, fam_ind, que_index, que);
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::RecreateSwapchain(VkSwapchainKHR &swapchain,
                       VkSwapchainCreateInfoKHR &swapchaincreateinfo,
                       std::vector<VkImage> &swapchain_images,
                       std::vector<VkFramebuffer> &framebuffers,
                       std::vector<VkFramebufferCreateInfo> &create_infos,
                       RenderPass &render_pass,
                       std::vector<VkImageView> &image_views,
                       VkImageView& depth_imageview,
                       VkSurfaceKHR &surf,
                       VkPhysicalDevice &physdev,
                       VkDevice &dev,
                       GLFWwindow *glfwin) {

  int width = 0, height = 0;
  glfwGetFramebufferSize(glfwin, &width, &height);

  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(glfwin, &width, &height);
    glfwWaitEvents();
  }
  
  vkDeviceWaitIdle (dev);

  CleanupSwapchain (framebuffers, image_views, swapchain, dev);
  bool swapchain_res = CreateSwapchain (swapchain, swapchaincreateinfo, surf, physdev, dev, glfwin); 
  bool imageviews_res = CreateImageViews (image_views, swapchain_images, swapchaincreateinfo.imageFormat, dev);
  bool framebuffers_res = CreateFramebuffers (framebuffers, create_infos, render_pass, swapchaincreateinfo.imageExtent, image_views, depth_imageview,  dev);
  return (swapchain_res &&imageviews_res && framebuffers_res); 
}




// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
void rokz::CleanupSwapchain(std::vector<VkFramebuffer> &framebuffers,
                            std::vector<VkImageView> &image_views,
                            VkSwapchainKHR &swapchain,
                            const VkDevice& device) {

  for (auto fb : framebuffers) {
    vkDestroyFramebuffer (device, fb, nullptr); 
  }

  for (auto imageview : image_views) {
    vkDestroyImageView(device, imageview, nullptr);
  }

  vkDestroySwapchainKHR(device, swapchain, nullptr);
  
}


// ---------------------------------------------------------------------
// DESTROY ALL THE THINGS
// ---------------------------------------------------------------------
void rokz::Cleanup(VkPipeline &pipeline,
                   std::vector<VkFramebuffer> &framebuffers,
                   VkSwapchainKHR &swapchain,
                   BufferStruc&        vbstruc, 
                   VkSurfaceKHR &surf,
                   VkCommandPool &command_pool,
                   std::vector<SyncStruc>& syncs, 
                   std::vector<ShaderModule> &shader_modules,
                   VkPipelineLayout &pipeline_layout,
                   RenderPass &render_pass,
                   std::vector<VkImageView> &image_views,
                   GLFWwindow *w,
                   VkDevice &dev, VkInstance &inst) {


  //    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

  CleanupSwapchain (framebuffers, image_views, swapchain, dev);

  
  DestroyBuffer (vbstruc, dev); 
  
  vkDestroyPipeline (dev, pipeline, nullptr);
  vkDestroySurfaceKHR (inst, surf, nullptr);
  vkDestroyCommandPool ( dev, command_pool, nullptr);

  for (size_t i = 0; i < syncs.size (); ++i) {
    vkDestroySemaphore(dev, syncs[i].image_available_sem, nullptr);
    vkDestroySemaphore(dev, syncs[i].render_fnished_sem, nullptr);
    vkDestroyFence(dev, syncs[i].in_flight_fen, nullptr);
  }

  for (auto shmod : shader_modules) {
    vkDestroyShaderModule (dev, shmod.handle, nullptr); 
  }
  


  vkDestroyPipelineLayout(dev, pipeline_layout, nullptr);
  vkDestroyRenderPass (dev, render_pass.handle, nullptr); 
  
  vkDestroyDevice (dev, nullptr); 
  vkDestroyInstance(inst, nullptr);

  
  glfwDestroyWindow(w);
}
