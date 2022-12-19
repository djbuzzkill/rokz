
#include "context.h"
#include "window.h"


const bool kEnableValidationLayers = true;
  //#endif

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
const std::vector<const char*> kDeviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


// const std::vector<const char*>& rokz::GetDeviceExtensionNames () {
//   return kDeviceExtensions; 
// }


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
rokz::SwapchainSupportInfo& rokz::QuerySwapchainSupport (rokz::SwapchainSupportInfo& si, VkSurfaceKHR surf, VkPhysicalDevice device) {

  printf ("%s\n", __FUNCTION__);
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR (device, surf, &si.capabilities);

  uint32_t format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surf, &format_count, nullptr);
  printf ("fmt_count[%u]\n", format_count); 
  
  if (format_count != 0) {
    si.formats.resize(format_count);
    VkResult res = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surf, &format_count, &si.formats[0]);
  }

  uint32_t present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surf, &present_mode_count,  nullptr);
  printf ("present_mode_count[%u]\n", present_mode_count); 

  if (present_mode_count != 0) {
    si.present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surf, &present_mode_count, &si.present_modes[0]);
  }

  return si;
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------

VkApplicationInfo& rokz::AppInfo_default (VkApplicationInfo& app_info) {
  app_info.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "ROKZ";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName      = "null";
  app_info.engineVersion    = VK_MAKE_VERSION(1, 3, 0);
  app_info.apiVersion       = VK_API_VERSION_1_3;
  return app_info;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------

VkInstanceCreateInfo& rokz::CreateInfo (VkInstanceCreateInfo& ci,
                                        std::vector<const char*>& required_extensions, 
                                        const VkApplicationInfo&  app_info) {


  printf ("%s VkInstanceCreateInfo \n", __FUNCTION__); 
  printf ( "REQUIRED EXTENSIONS:\n");
  
  rokz::GetRequiredExtensionNames (required_extensions);  
  for (size_t i = 0; i < required_extensions.size (); ++i)
    printf  ("  %s\n", required_extensions[i]);

  ci.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  ci.pApplicationInfo = &app_info;

  ci.enabledExtensionCount = required_extensions.size();
  ci.ppEnabledExtensionNames = &required_extensions[0];
  ci.pNext = nullptr; 

  printf  ("ED E\n"); 
  
  if (kEnableValidationLayers && CheckValidationSupport (validation_layers)) {
    printf ("ENABLE VALIDATION LAYERS\n"); 
    ci.enabledLayerCount   = validation_layers.size(); 
    ci.ppEnabledLayerNames = &validation_layers[0]; 
    // !! SETUP up additional output cb handling...
  }
  else {
    printf ("VALIDATION LAYERS ARE DISABLED\n"); 
    ci.enabledLayerCount   = 0;
    ci.ppEnabledLayerNames = nullptr;
  }
  printf  ("SDRGED E\n"); 
  // 
  return ci;
}


//
//
//
int rokz::CreateInstance (VkInstance& instance, const VkInstanceCreateInfo& ci) {
  using namespace rokz;
  // 

  // CREATEINSTANCE
  printf ("vkCreateInstance() \n"); 
  if (vkCreateInstance (&ci, nullptr, &instance) != VK_SUCCESS) {
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





// int rokz::CreateInstance (VkInstance& instance, VkApplicationInfo& app_info, VkInstanceCreateInfo& inst_info) {

//   // VkApplicationInfo
//   app_info.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//   app_info.pApplicationName = "ROKZ";
//   app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//   app_info.pEngineName      = "no_engine";
//   app_info.engineVersion    = VK_MAKE_VERSION(1, 1, 0);
//   app_info.apiVersion       = VK_API_VERSION_1_1;
//   //glob.app_info.pNext = nullptr; 

//   // VkInstanceCreateInfo
//   std::vector<const char*> req_exts; 
//   rokz::GetRequiredExtensionNames (req_exts);  

//   inst_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//   inst_info.pApplicationInfo = &app_info;
//   // 
//   inst_info.enabledExtensionCount = req_exts.size();
//   inst_info.ppEnabledExtensionNames = &req_exts[0];
//   inst_info.pNext = nullptr; 


//   if (kEnableValidationLayers && CheckValidationSupport (validation_layers)) {
//     printf ("ENABLE VALIDATION LAYERS\n"); 
//     inst_info.enabledLayerCount   = validation_layers.size(); 
//     inst_info.ppEnabledLayerNames = &validation_layers[0]; 
//     // !! SETUP up additional output cb handling...
//   }
//   else {
//     printf ("VALIDATION LAYERS ARE DISABLED\n"); 
//     inst_info.enabledLayerCount   = 0;
//     inst_info.ppEnabledLayerNames = nullptr;
//   }

//   // CREATEINSTANCE
//   printf ("vkCreateInstance() \n"); 
//   if (vkCreateInstance (&inst_info, nullptr, &instance) != VK_SUCCESS) {
//     printf("failed to create instance!");
//     return __LINE__;
//   }
  
//   printf("WoooooOOOooooOoooooooOOoOoOOOoooOoOOOOOOO!!!1\n");
//   // ENUMERATEINSTANCEEXTENSIONPROPERTIES
//   uint32_t ext_count = 0;
//   vkEnumerateInstanceExtensionProperties (nullptr, &ext_count, nullptr);

//   printf("   num extensions[%u]\n", ext_count);
//   std::vector<VkExtensionProperties> extensions(ext_count);
//   vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, extensions.data());

//   std::cout << "available extensions:\n";
//   for (const auto &extension : extensions) {
//     std::cout << '\t' << extension.extensionName << '\n';
//   }
  
//   return 0;
// }







VkDeviceQueueCreateInfo& rokz::CreateInfo (VkDeviceQueueCreateInfo& info,
                                           uint32_t que_fam_index,
                                           float* q_priority){

  printf ("%s VkDeviceQueueCreateInfo [%u]\n", __FUNCTION__, que_fam_index);
  
  info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; 
  info.flags            = 0; 
  info.queueFamilyIndex = que_fam_index ; // glob.queue_fams.graphics.value();

  info.queueCount       = 1; 
  info.pQueuePriorities = q_priority;

  return info; 
}


VkDeviceCreateInfo& rokz::CreateInfo (VkDeviceCreateInfo&       info,
                                      const std::vector<VkDeviceQueueCreateInfo>&  queuecreateinfos,
                                      VkPhysicalDeviceFeatures* devfeats) {

  printf ("%s VkDeviceCreateInfo\n", __FUNCTION__);

  const std::vector<const char*>& device_extensions = kDeviceExtensions;
  
  info.sType                 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  info.enabledExtensionCount = static_cast<uint32_t> (device_extensions.size ());
  info.ppEnabledExtensionNames= &device_extensions[0]; 
  info.queueCreateInfoCount  = queuecreateinfos.size();   
  info.pQueueCreateInfos     = &queuecreateinfos[0]; /// &glob.create_info.queue;
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
      printf ("%s return [f]\n", __FUNCTION__);
      return f;
    }
  }

  printf ("%s return [0]\n", __FUNCTION__);
  return available_formats[0]; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkPresentModeKHR rokz::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &available_modes) {


  printf ("%s return [0]\n", __FUNCTION__);
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
// nu
// ---------------------------------------------------------------------
// VkSwapchainCreateInfoKHR& rokz::Default (VkSwapchainCreateInfoKHR& info,
//                                          const VkSurfaceKHR&       surf) {
  
//   info.sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
//   info.surface = surf;

//   return info; 
// }
VkSwapchainCreateInfoKHR& rokz::CreateInfo_default (VkSwapchainCreateInfoKHR&   ci, 
                                                    std::vector<uint32_t>&  family_indices,
                                                    VkSurfaceKHR                surface,
                                                    const VkExtent2D&           extent, 
                                                    const SwapchainSupportInfo& swapchain_support_info, 
                                                    const PhysicalDevice&       physdev) {
  printf ("%s\n", __FUNCTION__);

  VkSurfaceFormatKHR swap_surface_format = ChooseSwapSurfaceFormat (swapchain_support_info.formats);
  VkPresentModeKHR   present_mode        = ChooseSwapPresentMode   (swapchain_support_info.present_modes);
  //VkExtent2D         extent              = ChooseSwapExtent        (swapchain_support_info.capabilities, win.glfw_window);
  
  uint32_t image_count = swapchain_support_info.capabilities.minImageCount + 1; 
  if (swapchain_support_info.capabilities.maxImageCount > 0 && image_count > swapchain_support_info.capabilities.maxImageCount) {
    image_count = swapchain_support_info.capabilities.maxImageCount;
  }

  ci = {};
  ci.sType         = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  ci.surface       = surface;
  ci.minImageCount = image_count;
  ci.imageFormat   = swap_surface_format.format;
  ci.imageColorSpace = swap_surface_format.colorSpace;
  ci.imageExtent = extent;
  ci.imageArrayLayers = 1;
  ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  // QueueFamilyIndices que_fam_inds; 
  // FindQueueFamilies (que_fam_inds, surface, physdev);
  family_indices.clear (); 
  family_indices.push_back (physdev.family_indices.graphics.value());
  family_indices.push_back (physdev.family_indices.present.value ());
 
  if (physdev.family_indices.graphics != physdev.family_indices.present) {
    printf ("[VK_SHARING_MODE_CONCURRENT]\n");
    ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    ci.queueFamilyIndexCount = family_indices.size();;
    ci.pQueueFamilyIndices = &family_indices[0];
  } else {
    printf ("[VK_SHARING_MODE_EXCLUSIVE]\n");
    ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    ci.queueFamilyIndexCount = 0;     
    ci.pQueueFamilyIndices = nullptr; 
  }

  ci.preTransform   = swapchain_support_info.capabilities.currentTransform;
  ci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  ci.presentMode    = present_mode; 
  ci.clipped        = VK_TRUE;
  ci.oldSwapchain   = VK_NULL_HANDLE;

  return ci;
}

// ---------------------------------------------------------------------
bool rokz::CreateSwapchain (Swapchain& swapchain, const Device& device) {

  printf ("%s\n", __FUNCTION__);

  if (vkCreateSwapchainKHR (device.handle, &swapchain.ci, nullptr, &swapchain.handle) != VK_SUCCESS) {
    printf ("failed to create swap chain!\n");
    printf ("LEAVING[FALSE] %s\n", __FUNCTION__);
    return false; 
  }

  printf ("LEAVING[TRUE] %s\n", __FUNCTION__);
  return true;


}

// ---------------------------------------------------------------------
// bool CreateSwapchain (VkSwapchainKHR& swapchain, 
//                       VkSwapchainCreateInfoKHR& swapchaincreateinfo,
//                       const VkSurfaceKHR& surf,
//                       const VkPhysicalDevice& physdev, 
//                       const VkDevice& dev, 
//                       GLFWwindow* glfwin) {

//   using namespace rokz;
//   printf ("%s", __FUNCTION__);

//   rokz::SwapchainSupportInfo swapchain_supp_info {};
//   QuerySwapchainSupport (swapchain_supp_info, surf, physdev); 
  
//   Default (swapchaincreateinfo, surf); 
//   VkSurfaceFormatKHR swap_surface_format = ChooseSwapSurfaceFormat (swapchain_supp_info.formats);
//   VkPresentModeKHR   present_mode   = ChooseSwapPresentMode   (swapchain_supp_info.present_modes);
//   VkExtent2D         extent         = ChooseSwapExtent        (swapchain_supp_info.capabilities, glfwin);
  

//   uint32_t image_count = swapchain_supp_info.capabilities.minImageCount + 1; 
//   if (swapchain_supp_info.capabilities.maxImageCount > 0 && image_count > swapchain_supp_info.capabilities.maxImageCount) {
//     image_count = swapchain_supp_info.capabilities.maxImageCount;
//   }

//   swapchaincreateinfo.minImageCount = image_count;
//   swapchaincreateinfo.imageFormat = swap_surface_format.format;
//   swapchaincreateinfo.imageColorSpace = swap_surface_format.colorSpace;
//   swapchaincreateinfo.imageExtent = extent;
//   swapchaincreateinfo.imageArrayLayers = 1;
//   swapchaincreateinfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  
//   QueueFamilyIndices que_fam_inds; 
//   FindQueueFamilies (que_fam_inds, surf, physdev); 

//   uint32_t fam_inds[] = {

//     que_fam_inds.graphics.value(),
//     que_fam_inds.present.value (),
    
//   }; 
  
//   if (que_fam_inds.graphics != que_fam_inds.present) {
//     printf ("[VK_SHARING_MODE_CONCURRENT]\n");
//     swapchaincreateinfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
//     swapchaincreateinfo.queueFamilyIndexCount = 2;
//     swapchaincreateinfo.pQueueFamilyIndices = fam_inds;
//   } else {
//     printf ("[VK_SHARING_MODE_EXCLUSIVE]\n");
//     swapchaincreateinfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
//     swapchaincreateinfo.queueFamilyIndexCount = 0;     
//     swapchaincreateinfo.pQueueFamilyIndices = nullptr; 
//   }

//   swapchaincreateinfo.preTransform   = swapchain_supp_info.capabilities.currentTransform;
//   swapchaincreateinfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

//   swapchaincreateinfo.presentMode    = present_mode; 
//   swapchaincreateinfo.clipped        = VK_TRUE;
//   swapchaincreateinfo.oldSwapchain   = VK_NULL_HANDLE;

//   if (vkCreateSwapchainKHR (dev, &swapchaincreateinfo, nullptr, &swapchain) != VK_SUCCESS) {
//     printf ("failed to create swap chain!\n");
//     printf ("LEAVING[FALSE] %s\n", __FUNCTION__);
//     return false; 
//   }

//   printf ("LEAVING[TRUE] %s\n", __FUNCTION__);
//   return true;
// }

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
                            const Device&               device) {
  printf ("%s\n", __FUNCTION__); 
  
  swapchain_imageviews.resize (swapchain_images.size()); 
  
  for (size_t i = 0; i < swapchain_imageviews.size(); i++) {
    //Init (createinfo, VK_IMAGE_ASPECT_COLOR_BIT, swapchain_images[i] ); 
    //    Default (createinfo, swapchain_images[i], surf_fmt); 
    rokz::Image image_temp {};
    image_temp.handle = swapchain_images[i];
    image_temp.ci.format = surf_fmt;
    
    ImageView imagev = {};
    imagev.handle = swapchain_imageviews[i];
    Init (imagev.ci, VK_IMAGE_ASPECT_COLOR_BIT, image_temp); 
    
    if (!CreateImageView (imagev, imagev.ci, device.handle)) {
       printf ("[FAILED] %s create imageview \n", __FUNCTION__); 
    }

    swapchain_imageviews[i] = imagev.handle;
    
    // VkResult res = vkCreateImageView (dev, &ci, nullptr, &swapchain_imageviews[i]);
    // if (res != VK_SUCCESS) {
    //   printf ("[FAILED] %s create imageview \n", __FUNCTION__); 
    //   return false; 
    // }
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
bool rokz::CreateRenderPass (RenderPass&             render_pass,
                             VkFormat                swapchain_format,
                             VkSampleCountFlagBits   msaa_samples, 
                             const VkDevice&         device,
                             const VkPhysicalDevice& physdev) {
  //printf ("%s\n", __FUNCTION__); 

  // COLOR ATTACHMENT | VkAttachmentDescription 
  auto co_in = RenderPass::COLOR; 
  render_pass.attach_desc[co_in] = {}; 
  render_pass.attach_desc[co_in].format         = swapchain_format ;
  render_pass.attach_desc[co_in].samples        = msaa_samples, // VK_SAMPLE_COUNT_1_BIT; // msaa samples
  render_pass.attach_desc[co_in].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  render_pass.attach_desc[co_in].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
  render_pass.attach_desc[co_in].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  render_pass.attach_desc[co_in].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  render_pass.attach_desc[co_in].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  render_pass.attach_desc[co_in].finalLayout    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // when msaa is used otherwise -> VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; 

  // DEPTHSTENCIL ATTACHMENT | VkAttachmentDescription 
  auto dp_in = RenderPass::DEPTHSTENCIL;
  VkFormat depth_format; 
  rokz::FindDepthFormat (depth_format, physdev); 
  render_pass.attach_desc[dp_in] = {}; 
  render_pass.attach_desc[dp_in].format         = depth_format;
  render_pass.attach_desc[dp_in].samples        = msaa_samples; // VK_SAMPLE_COUNT_1_BIT;
  render_pass.attach_desc[dp_in].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  render_pass.attach_desc[dp_in].storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  render_pass.attach_desc[dp_in].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  render_pass.attach_desc[dp_in].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  render_pass.attach_desc[dp_in].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  render_pass.attach_desc[dp_in].finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  // COLOR RESOLVE ATTACHMENT | VkAttachmentDescription 
  auto cr_in = RenderPass::CORESOLV; 
  render_pass.attach_desc[cr_in] = {}; 
  render_pass.attach_desc[cr_in].format         = swapchain_format ;
  render_pass.attach_desc[cr_in].samples        = VK_SAMPLE_COUNT_1_BIT; // msaa samples
  render_pass.attach_desc[cr_in].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  render_pass.attach_desc[cr_in].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
  render_pass.attach_desc[cr_in].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  render_pass.attach_desc[cr_in].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  render_pass.attach_desc[cr_in].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  render_pass.attach_desc[cr_in].finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


  //VkAttachmentReference color_attachment_ref{};
  render_pass.attach_ref[co_in] = {};
  render_pass.attach_ref[co_in].attachment = co_in; // index
  render_pass.attach_ref[co_in].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  // depth
  render_pass.attach_ref[dp_in] = {};
  render_pass.attach_ref[dp_in].attachment = dp_in;
  render_pass.attach_ref[dp_in].layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  // not used yet
  render_pass.attach_ref[cr_in] = {};
  render_pass.attach_ref[cr_in].attachment = cr_in; // index
  render_pass.attach_ref[cr_in].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  
  // SUBPASS,  VkSubpassDescription                 
  render_pass.subpass_descs.resize (1);
  render_pass.subpass_descs[0] = {};
  render_pass.subpass_descs[0].pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
  render_pass.subpass_descs[0].colorAttachmentCount    = 1;
  render_pass.subpass_descs[0].pColorAttachments       = &render_pass.attach_ref[co_in];  // co_in, [cr_in] for msaa
  render_pass.subpass_descs[0].inputAttachmentCount    = 0;
  render_pass.subpass_descs[0].pInputAttachments       = nullptr;
  render_pass.subpass_descs[0].pDepthStencilAttachment = &render_pass.attach_ref[dp_in]; //nullptr;
  render_pass.subpass_descs[0].preserveAttachmentCount = 0;
  render_pass.subpass_descs[0].pPreserveAttachments    = nullptr;
  render_pass.subpass_descs[0].pResolveAttachments     = &render_pass.attach_ref[cr_in];
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
  render_pass.create_info.attachmentCount = 3; // color + depthstencil + color resolv
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

//
bool rokz::CreateFramebuffer (VkFramebuffer&           framebuffers,
                              VkFramebufferCreateInfo& create_infos,
                              const RenderPass&        render_pass, 
                              const VkExtent2D         swapchain_ext, 
                              VkImageView&             image_views, 
                              const VkImageView&       msaa_color_imageview, 
                              const VkImageView&       depth_imageview, 
                              const VkDevice&          device) {
  return false;
}
    
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------xs
bool rokz::CreateFramebuffers (
    std::vector<VkFramebuffer>&           framebuffers,
    std::vector<VkFramebufferCreateInfo>& create_infos,
    const RenderPass&                     render_pass, 
    const VkExtent2D                      swapchain_ext, 
    const std::vector<VkImageView>&       image_views,
    const VkImageView&                    msaa_color_imageview, 
    const VkImageView&                    depth_imageview, 
    const Device&                         device) {

  printf ("[%s]\n", __FUNCTION__);
  framebuffers.resize (image_views.size()); 
  create_infos.resize (image_views.size()); 
  
  for (size_t i = 0; i < image_views.size(); i++) {

    VkImageView attachments[] = {msaa_color_imageview, depth_imageview, image_views[i] };

    create_infos[i] = {}; 
    create_infos[i].sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    create_infos[i].renderPass = render_pass.handle;
    create_infos[i].attachmentCount = 3; // color + depthstencil + colresolv
    create_infos[i].pAttachments = attachments;
    create_infos[i].width = swapchain_ext.width;
    create_infos[i].height = swapchain_ext.height;
    create_infos[i].layers = 1;

    if (vkCreateFramebuffer(device.handle, &create_infos[i], nullptr, &framebuffers[i]) != VK_SUCCESS) {
      printf ("[FAILED] %s create framebuffer\n", __FUNCTION__);
      return false;
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
  //clear_values[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  
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

  VkClearValue clear_values[3] = {};
  clear_values[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  clear_values[1].depthStencil = {1.0f, 0};
  clear_values[2].color = {{0.0f, 0.0f, 0.0f, 1.0f}};


  pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  pass_info.renderPass = render_pass.handle; 
  pass_info.framebuffer = framebuffer; 
  pass_info.renderArea.offset = {0, 0};
  pass_info.renderArea.extent = ext2d;
  pass_info.clearValueCount = 3; 
  pass_info.pClearValues = clear_values; 

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
  printf ("%s\n", __FUNCTION__);
  const uint32_t que_index = 0; // diffrnt than que_fam_ind
  printf ("%s EXIT\n", __FUNCTION__);
  return vkGetDeviceQueue (device, fam_ind, que_index, que);
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::RecreateSwapchain(Swapchain&                               swapchain,
                             std::vector<VkImage>&                    swapchain_images,
                             std::vector<VkFramebuffer>&              framebuffers,
                             std::vector<VkFramebufferCreateInfo>&    create_infos,
                             RenderPass&                              render_pass,
                             std::vector<VkImageView>&                image_views,
                             //VkImageView& depth_imageview,

                             Image&           depth_image, 
                             ImageView&       depth_imageview,
                     
                             Image&           multisamp_color_image, 
                             ImageView&       multisamp_color_imageview,

                             //VkSurfaceKHR&     surface,
                             //VkPhysicalDevice& physdev,
                             //const SwapchainSupportInfo& swapchain_support_info,
                             const Device&     device,
                             const VmaAllocator& allocator,
                             GLFWwindow *  glfwin) {

  printf ("%s\n", __FUNCTION__);

  int width = 0, height = 0;
  glfwGetFramebufferSize(glfwin, &width, &height);

  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(glfwin, &width, &height);
    glfwWaitEvents();
  }
  
  vkDeviceWaitIdle (device.handle);

  CleanupSwapchain (framebuffers, image_views,
                    depth_image, depth_imageview,
                    multisamp_color_image,
                    multisamp_color_imageview,
                    swapchain, device, allocator);

  //CreateInfo_default (swapchain.ci, surf, extent, swapchain_support_info, 
  bool swapchain_res    = CreateSwapchain (swapchain, device);
  
  bool imageviews_res   = CreateImageViews (image_views,
                                            swapchain_images,
                                            swapchain.ci.imageFormat,
                                            device);
  //
  bool framebuffers_res = CreateFramebuffers (framebuffers,
                                              create_infos,
                                              render_pass,
                                              swapchain.ci.imageExtent,
                                              image_views,
                                              multisamp_color_imageview.handle,
                                              depth_imageview.handle,
                                              device);

  return (swapchain_res &&imageviews_res && framebuffers_res); 
}




// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
void rokz::CleanupSwapchain (std::vector<VkFramebuffer> &framebuffers,
                             std::vector<VkImageView> &image_views,

                             Image&          depth_image, 
                             ImageView&      depth_imageview,

                             Image&          multisamp_color_image, 
                             ImageView&      multisamp_color_imageview,

                             Swapchain&          swapchain,
                             const Device&       device,
                             const VmaAllocator& allocator) {

  for (auto fb : framebuffers) {
    vkDestroyFramebuffer (device.handle, fb, nullptr); 
  }

  for (auto imageview : image_views) {
    vkDestroyImageView(device.handle, imageview, nullptr);
  }

  Destroy (depth_image, allocator);
  Destroy (depth_imageview, device.handle);

  Destroy (multisamp_color_image, allocator);
  Destroy (multisamp_color_imageview, device.handle);

  vkDestroySwapchainKHR(device.handle, swapchain.handle, nullptr);
  
}


// ---------------------------------------------------------------------
// DESTROY ALL THE THINGS
// ---------------------------------------------------------------------
void rokz::Cleanup(VkPipeline &pipeline,

                   std::vector<VkFramebuffer> &framebuffers,

                   Swapchain&                  swapchain,
                   VkSurfaceKHR&               surf,
                   VkCommandPool&              command_pool,
                   std::vector<SyncStruc>&     syncs, 
                   std::vector<ShaderModule>&  shader_modules,
                   VkPipelineLayout&           pipeline_layout,
                   RenderPass&                 render_pass,
                   std::vector<VkImageView>&   image_views,

                   Image&      depth_image, 
                   ImageView&  depth_imageview,

                   Image&      multisamp_color_image, 
                   ImageView&  multisamp_color_imageview,

                   GLFWwindow*   w,
                   Device&       device,
                   VmaAllocator& allocator, 
                   VkInstance&   inst) {


  //    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

  CleanupSwapchain (framebuffers,
                    image_views,

                    depth_image, 
                    depth_imageview,

                    multisamp_color_image, 
                    multisamp_color_imageview,

                    swapchain,
                    device, 
                    allocator
                    );
   
  vkDestroyPipeline (device.handle, pipeline, nullptr);
  vkDestroySurfaceKHR (inst, surf, nullptr);
  vkDestroyCommandPool (device.handle, command_pool, nullptr);

  for (size_t i = 0; i < syncs.size (); ++i) {
    vkDestroySemaphore(device.handle, syncs[i].image_available_sem, nullptr);
    vkDestroySemaphore(device.handle, syncs[i].render_fnished_sem, nullptr);
    vkDestroyFence (device.handle, syncs[i].in_flight_fen, nullptr);
  }

  for (auto shmod : shader_modules) {
    vkDestroyShaderModule (device.handle, shmod.handle, nullptr); 
  }

  vkDestroyPipelineLayout (device.handle, pipeline_layout, nullptr);
  vkDestroyRenderPass (device.handle, render_pass.handle, nullptr); 
  
  vkDestroyDevice (device.handle, nullptr); 
  vkDestroyInstance(inst, nullptr);

  
  glfwDestroyWindow(w);
}





// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CheckDeviceExtensionSupport (const VkPhysicalDevice& physdev) {
  printf ("%s\n", __FUNCTION__);

  uint32_t exts_count;
  vkEnumerateDeviceExtensionProperties(physdev, nullptr, &exts_count, nullptr);

  std::vector<VkExtensionProperties> available_exts(exts_count);
  vkEnumerateDeviceExtensionProperties(physdev, nullptr, &exts_count, &available_exts[0]);

  
  std::set<std::string> req_exts ( &kDeviceExtensions[0], &kDeviceExtensions[kDeviceExtensions.size()] );

  // try {
  
  printf ("looking for extensions:\n"); 
  for (const auto& e : req_exts) {
    printf ("  .. %s\n", e.c_str ()); 
  }
  
  printf("available extensions:\n");
  for (const auto &extension : available_exts) {
    printf("  .. %s\n", extension.extensionName);
    req_exts.erase(extension.extensionName);
  }
    
  // printf ("LEAVING %s (empty:%s)\n", __FUNCTION__, req_exts.empty()? "TRUE" : "FALSE");
  return req_exts.empty();
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool AllQueuesAvailable (const rokz::QueueFamilyIndices& inds) {
  printf ("%s\n", __FUNCTION__); 
  return inds.graphics && inds.present; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
rokz::QueueFamilyIndices& rokz::FindQueueFamilies (rokz::QueueFamilyIndices& inds, const VkSurfaceKHR& surf, const VkPhysicalDevice& physdev) {
  printf ("%s\n", __FUNCTION__); 

  uint32_t que_count  = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physdev, &que_count, nullptr);

  std::vector<VkQueueFamilyProperties> que_fam_props (que_count);

  vkGetPhysicalDeviceQueueFamilyProperties (physdev, &que_count, &que_fam_props[0]);

  for (uint32_t ique = 0; ique < que_count; ++ique) {

    rokz::QueueFamilyIndices cur_inds;
    VkBool32 present_support = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR (physdev, ique, surf, &present_support);
      
    if (present_support) {
      inds.present = ique;
      assert (inds.present.has_value ());
      printf ("found PRESENT queue;\n"); 
    }
      
    if (que_fam_props[ique].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      inds.graphics = ique;
      assert (inds.graphics.has_value()); 
      printf ("found GRAPHICS queue;\n");
    }
      
    if (AllQueuesAvailable (cur_inds)) {
      printf ("ALL THE Q's\n"); 
      inds = cur_inds;
      break;
    }
  }

  return inds;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool IsDeviceSuitable (rokz::QueueFamilyIndices& outind, VkPhysicalDeviceProperties& devprops, const VkSurfaceKHR& surf, const VkPhysicalDevice& physdev) {

  using namespace rokz;
  printf ("%s\n", __FUNCTION__); 

  VkPhysicalDeviceFeatures   devfeatures{}; 

  bool swapchain_adequate = false;
  bool exts_supported = CheckDeviceExtensionSupport (physdev);

  if (exts_supported) {
    printf ("[exts_supported]\n"); 
  }
  
  rokz::SwapchainSupportInfo swapchain_supp_info {};
  swapchain_supp_info.capabilities = {};
  swapchain_supp_info.formats = {};
  swapchain_supp_info.present_modes = {};

  bool all_queues_available = 
    AllQueuesAvailable (rokz::FindQueueFamilies (outind, surf, physdev)); 
  
  if (all_queues_available) {
    printf ("[all_queues_available]\n"); 
  }
  
  if (exts_supported) {
    rokz::QuerySwapchainSupport(swapchain_supp_info, surf, physdev);
    // if (swapchain_supp_info.formats.empty())
    //   printf ("!![swapchain_supp_info.formats.empty]\n"); 
    // if (swapchain_supp_info.present_modes.empty())
    //   printf ("!![swapchain_supp_info.present_modes.empty]\n"); 
    swapchain_adequate = !swapchain_supp_info.formats.empty() && !swapchain_supp_info.present_modes.empty();
  }

  if (swapchain_adequate) {
    printf ("[swapchain_adequate]\n"); 
  }
  else {
    printf ("[swapchain_inadequate]\n"); 
  }
  
  vkGetPhysicalDeviceProperties(physdev, &devprops);

  // FORCE ANISTOTROPY
  devfeatures.samplerAnisotropy = VK_TRUE;
  vkGetPhysicalDeviceFeatures  (physdev, &devfeatures);
  // bool device_is_suitable =
  //   devprops.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
  //   && devfeatures.geometryShader;

  if (devfeatures.samplerAnisotropy) {
    printf ("[SUPPORTED] -> Anisotropy\n"); 
  }

  return all_queues_available
    && exts_supported
    && swapchain_adequate
    && devfeatures.samplerAnisotropy; 
}

  
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------

//  rokz::SelectPhysicalDevice (glob.physical_device, glob.surface, glob.instance);

bool rokz::SelectPhysicalDevice (PhysicalDevice& physdev, const VkSurfaceKHR& surf, const VkInstance& inst) {

  using namespace rokz;
  //
  printf ("%s\n", __FUNCTION__); 

  unsigned device_count = 0;
  vkEnumeratePhysicalDevices (inst, &device_count, nullptr);
  if (device_count == 0) {
    printf ("..0 devices\n");
    return false; 
  }

  std::vector<VkPhysicalDevice> physdevs ( device_count, VK_NULL_HANDLE); 
  vkEnumeratePhysicalDevices(inst, &device_count, &physdevs[0]);

  for (unsigned idev = 0; idev < device_count; ++idev) {

    rokz::QueueFamilyIndices curqueinds {}; 
    physdev.properties = {}; 
    if (IsDeviceSuitable (curqueinds,  physdev.properties, surf, physdevs[idev])) {
      printf ("YES --> IsDeviceSuitable(%i)\n", idev); 
      // assert (curqueinds.present.has_value());
      // assert (curqueinds.graphics.has_value());
      physdev.handle = physdevs[idev];
      physdev.family_indices =  curqueinds; 
      printf ("found PHYSICAL DEVICE\n");
      return true; 
    }
    else {
      printf ("!! ELSE --> IsDeviceSuitable(%i)\n", idev); 
    }

  }

  return false; 
}
