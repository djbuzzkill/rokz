

#include "rokz.h"


#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

//#include <vulkan/vulkan.hpp>


const bool kEnableValidationLayers = true;
  //#endif


const std::vector<const char*> device_extensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::vector<const char*> validation_layers  = {
  "VK_LAYER_KHRONOS_validation"
};

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
GLFWwindow* rokz::CreateWindow_glfw (GLFWwindow*& w) {
  
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
  w = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);

  vkCreateXcbSurfaceKHR; 
  glfwGetX11Display; 
 
  return w; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CheckValidationSupport (const std::vector<const char*>& val_layers) {

  printf ("%s\n", __FUNCTION__); 

  uint32_t layer_count  = 0;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
  
  std::vector<VkLayerProperties> available_layers(layer_count);

  vkEnumerateInstanceLayerProperties(&layer_count, &available_layers[0]);

  for (const char* layer_name : val_layers) {
    printf ("   LOOKING FOR[%s]\n", layer_name);
    bool layer_found = false;

    for (const auto& layerProperties : available_layers) {
      if (strcmp(layer_name, layerProperties.layerName) == 0) {
        layer_found = true;
        break;
      }
    }
    if (!layer_found)
      return false; 
  }

  printf ("..ALL LAYERS FOUND\n"); 
  return true ;
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
    
      VkBool32 present_support = VK_FALSE;
      vkGetPhysicalDeviceSurfaceSupportKHR (physdev, ique, surf, &present_support);
      
      if (present_support) {
        inds.present = ique;

        assert (inds.present.has_value ());

        printf ("found PRESENT queue;\n"); 
      }
      
      if (que_fam_props[ique].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        printf ("found GRAPHICS queue;\n");
        inds.graphics = ique;

        assert (inds.graphics.has_value()); 
      }
      
      // if (que_fam_props[ique].queueFlags & VK_QUEUE_PRESENT_BIT) {
      //   inds.present = ique;
      // }
      
      if (AllQueuesAvailable (inds)) {
        //if (inds.graphics.has_value()) { 
        printf ("BREAK;\n"); 
        break;
    }
  }

  
  return inds;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device) {
  printf ("%s\n", __FUNCTION__);

  uint32_t exts_count;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &exts_count, nullptr);

  std::vector<VkExtensionProperties> available_exts(exts_count);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &exts_count, &available_exts[0]);

  
  std::set<std::string> req_exts (&device_extensions[0], &device_extensions[device_extensions.size()]);

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
rokz::SwapchainSupportInfo& rokz::QuerySwapchainSupport (rokz::SwapchainSupportInfo& deets,
                                                         const VkSurfaceKHR&         surf,
                                                         const VkPhysicalDevice&     dev) {

  printf ("%s\n", __FUNCTION__);
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR (dev, surf, &deets.capabilities);

  uint32_t fmt_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surf, &fmt_count, nullptr);
  printf ("fmt_count[%u]\n", fmt_count); 
  
  if (fmt_count != 0) {
    deets.formats.resize(fmt_count);
    VkResult res = vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surf, &fmt_count, &deets.formats[0]);
  }

  uint32_t present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surf, &present_mode_count,  nullptr);
  printf ("present_mode_count[%u]\n", present_mode_count); 

  if (present_mode_count != 0) {
    deets.present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surf, &present_mode_count, &deets.present_modes[0]);
  }

  printf ("LEAVING:%s", __FUNCTION__);
  return deets;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool IsDeviceSuitable (rokz::QueueFamilyIndices& outind, const VkSurfaceKHR& surf, const VkPhysicalDevice& physdev) {

  printf ("%s\n", __FUNCTION__); 

  VkPhysicalDeviceProperties devprops{};
  VkPhysicalDeviceFeatures   devfeatures{}; 

  bool swapchain_adequate = false;
  bool exts_supported = CheckDeviceExtensionSupport (physdev);
  

  if (exts_supported) {
    printf ("[exts_supported]\n"); 
  }
  
  rokz::SwapchainSupportInfo swapchain_supp_info {};

  // swapchain_supp_info.capabilities = {};
  // swapchain_supp_info.formats = {};
  // swapchain_supp_info.present_modes = {};

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
  vkGetPhysicalDeviceFeatures  (physdev, &devfeatures);
  // bool device_is_suitable =
  //   devprops.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
  //   && devfeatures.geometryShader;
  return all_queues_available
    && exts_supported
    && swapchain_adequate; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::SelectPhysicalDevice (VkPhysicalDevice&         physdev,
                                 rokz::QueueFamilyIndices& queueind,
                                 const VkSurfaceKHR&       surf,
                                 const VkInstance&         inst) {
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

  for (int idev = 0; idev < device_count; ++idev) {

    rokz::QueueFamilyIndices curqueinds {}; 

    if (IsDeviceSuitable (curqueinds, surf, physdevs[idev])) {
      printf ("YES --> IsDeviceSuitable(%i)\n", idev); 
      // assert (curqueinds.present.has_value());
      // assert (curqueinds.graphics.has_value());

      queueind = curqueinds; 
      printf ("found PHYSICAL DEVICE\n");
      physdev = physdevs[idev];
      printf ("found PHYSICAL DEVICE\n");
      return true; 
    }
    else {
      printf ("!! ELSE --> IsDeviceSuitable(%i)\n", idev); 
    }

  }

  return false; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
std::vector<const char*>& GetRequiredExtensions (std::vector<const char*>&  exts) {
  exts.clear ();

  // GLFW REQ'D EXTENSIONS
  uint32_t glfw_ext_count = 0;
  const char **glfw_exts;
  glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_ext_count);
  printf ("glfw_ext_count[%u]\n" , glfw_ext_count); 

  for  (int i = 0; i < glfw_ext_count; ++i) {
    printf ("req_extent(%i)[%s]\n", i, glfw_exts[i] );
    exts.push_back ( glfw_exts[i] ); 
  }
  return exts; 
}

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
  GetRequiredExtensions (req_exts);  

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


rokz::Glob& rokz::Default (rokz::Glob& g) {
  g.queue_priority = 1.0f;
  g.app_info            = {};
  g.device_features     = {};
  g.create_info.instance= {};
  g.create_info.device  = {};
  g.create_info.queue   = {};
  g.create_info.swapchain= {};
  g.create_info.imageview = {}; 
  g.queue_fams = {}; 
  g.queue_fams.graphics.reset();
  g.queue_fams.present.reset();

  return g; 
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
    swapchaincreateinfo.queueFamilyIndexCount = 0;     // Optional
    swapchaincreateinfo.pQueueFamilyIndices = nullptr; // Optional
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
  
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateImageViews (std::vector<VkImageView>&  swapchain_imageviews,
                            const std::vector<VkImage>& swapchain_images,
                            VkFormat                    surf_fmt, 
                            const VkDevice& dev) {

  swapchain_imageviews.resize (swapchain_images.size()); 
  
  for (size_t i = 0; i < swapchain_imageviews.size(); i++) {

    VkImageViewCreateInfo createinfo = {};

    Default (createinfo, swapchain_images[i], surf_fmt); 

    VkResult res = vkCreateImageView (dev, &createinfo, nullptr, &swapchain_imageviews[i]);
    if (res != VK_SUCCESS) {
      return false; 
    }
  }

  return true;   
}




// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreatePipelineLayout () {

  return false; 
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkShaderModule& rokz::CreateShaderModule(VkShaderModule& shader_module, const rokz::bytearray& code, const VkDevice& dev) {

  VkShaderModuleCreateInfo create_info {};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = code.size();
  create_info.pCode = reinterpret_cast<const uint32_t *>(&code[0]);

  if (vkCreateShaderModule(dev, &create_info, nullptr, &shader_module) != VK_SUCCESS) {
    printf ("failed to create shader module!\n");
    //throw std::runtime_error("failed to create shader module!");
  }

  return shader_module; 
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateGraphicsPipeline (VkPipelineLayout& pipeline_layout,
                                   std::vector<VkShaderModule>& shader_modules,
                                   const VkExtent2D& swapchain_extent,
                                   const VkDevice& device) {
  
  //rokz::From_file (vertbin, "/home/djbuzzkill/owenslake/data/shader/basic_vert.spv");
  std::string vert_file =  "/home/djbuzzkill/owenslake/data/shader/basic_vert.spv" ;
  bytearray      vertbin;
  VkShaderModule vertmod; 
  From_file(vertbin, vert_file);
  rokz::CreateShaderModule (vertmod,  vertbin, device); 
  shader_modules.push_back (vertmod); 
  // VERT SHADER 
  VkPipelineShaderStageCreateInfo vert_shader_stage_info {};
  vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vert_shader_stage_info.module = vertmod;
  vert_shader_stage_info.pSpecializationInfo = nullptr; 
  vert_shader_stage_info.pName = "main";
  //vkDestroyShaderModule(glob.device, vertmod, nullptr);

  // FRAG SHADER 
  std::string frag_file =   "/home/djbuzzkill/owenslake/data/shader/basic_frag.spv" ;
  bytearray      fragbin;
  VkShaderModule fragmod; 
  From_file(fragbin, frag_file );
  rokz::CreateShaderModule (fragmod,  fragbin, device); 
  shader_modules.push_back (fragmod); 
  //
  VkPipelineShaderStageCreateInfo frag_shader_stage_info {};
  frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  frag_shader_stage_info.module = fragmod;
  frag_shader_stage_info.pSpecializationInfo = nullptr; 
  frag_shader_stage_info.pName = "main";

  //
  VkPipelineShaderStageCreateInfo shader_stages[] = {
    vert_shader_stage_info,
    frag_shader_stage_info
  };

  // VERTEX INPUT
  VkPipelineVertexInputStateCreateInfo vertex_input_state_info{};
  vertex_input_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_input_state_info.vertexBindingDescriptionCount = 0;
  vertex_input_state_info.pVertexBindingDescriptions = nullptr; // Optional
  vertex_input_state_info.vertexAttributeDescriptionCount = 0;
  vertex_input_state_info.pVertexAttributeDescriptions = nullptr; // Optional


  
  // INPUT ASSEMBLY STATE
  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  // DYNAMIC STATE 
  std::vector<VkDynamicState> dynamic_states = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR};

  VkPipelineDynamicStateCreateInfo dynamic_state_create_info {};
  dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state_create_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
  dynamic_state_create_info.pDynamicStates = &dynamic_states[0];

  // VIEWPORT 
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)swapchain_extent.width;
  viewport.height = (float)swapchain_extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  // SCISSOR RECT
  VkRect2D scissor; 
  scissor.offset = {0, 0};
  scissor.extent = swapchain_extent;

  // VkPipelineViewportStateCreateInfo
  VkPipelineViewportStateCreateInfo viewport_state_create_info{};
  viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state_create_info.viewportCount = 1;
  
  viewport_state_create_info.pViewports = &viewport;
  viewport_state_create_info.scissorCount = 1;
  viewport_state_create_info.pScissors = &scissor;

  // RASTERIZATION STATE .. VkPipelineRasterizationStateCreateInfo
 VkPipelineRasterizationStateCreateInfo rasterizer{};
 rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
 rasterizer.depthClampEnable = VK_FALSE;
 rasterizer.rasterizerDiscardEnable = VK_FALSE;
 rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
 rasterizer.lineWidth = 1.0f;
 rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
 rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
 rasterizer.depthBiasEnable = VK_FALSE;
 rasterizer.depthBiasConstantFactor = 0.0f; // Optional
 rasterizer.depthBiasClamp = 0.0f;          // Optional
 rasterizer.depthBiasSlopeFactor = 0.0f;    // Optional

 // MULTI SAMPLING
 VkPipelineMultisampleStateCreateInfo multisampling{};
 multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
 multisampling.sampleShadingEnable = VK_FALSE;
 multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
 multisampling.minSampleShading = 1.0f;          // Optional
 multisampling.pSampleMask = nullptr;            // Optional
 multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
 multisampling.alphaToOneEnable = VK_FALSE;      // Optional

 // DEPTH/STENCIL 
 VkPipelineDepthStencilStateCreateInfo pipeline_depth_stencil_create_info {};


 // COLOR BLENDING
 VkPipelineColorBlendAttachmentState color_blend_attachment{};
 color_blend_attachment.colorWriteMask =
     VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
     VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
 color_blend_attachment.blendEnable = VK_FALSE;
 color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
 color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
 color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;             // Optional
 color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
 color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
 color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;             // Optional
 // Create Info
 VkPipelineColorBlendStateCreateInfo color_blending_create_info{};
 color_blending_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
 color_blending_create_info.logicOpEnable = VK_FALSE;
 color_blending_create_info.logicOp = VK_LOGIC_OP_COPY; // Optional
 color_blending_create_info.attachmentCount = 1;
 color_blending_create_info.pAttachments = &color_blend_attachment;
 color_blending_create_info.blendConstants[0] = 0.0f; // Optional
 color_blending_create_info.blendConstants[1] = 0.0f; // Optional
 color_blending_create_info.blendConstants[2] = 0.0f; // Optional
 color_blending_create_info.blendConstants[3] = 0.0f; // Optional


 // PIPELINE LAYOUT CREATE INFO
 VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
 pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
 pipeline_layout_create_info.setLayoutCount = 0;            // Optional
 pipeline_layout_create_info.pSetLayouts = nullptr;         // Optional
 pipeline_layout_create_info.pushConstantRangeCount = 0;    // Optional
 pipeline_layout_create_info.pPushConstantRanges = nullptr; // Optional

 if (vkCreatePipelineLayout(device, &pipeline_layout_create_info, nullptr, &pipeline_layout) != VK_SUCCESS) {
   printf ("failed to create pipeline layout!\n");
   return false; 
 }

 return true; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::Initialize (Glob& glob) {


  return false; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
void rokz::GetDeviceQueue (VkQueue* que, uint32_t fam_ind, const VkDevice& device) { 
  const uint32_t que_index = 0; // diffrnt than que_fam_ind
  return vkGetDeviceQueue (device, fam_ind, que_index, que);
}

// ---------------------------------------------------------------------
// DESTROY ALL THE THINGS
// ---------------------------------------------------------------------
void rokz::Cleanup(VkSwapchainKHR &swapchain,
                   VkSurfaceKHR &surf,
                   std::vector<VkShaderModule>& shader_modules,
                   VkPipelineLayout& pipeline_layout, 
                   std::vector<VkImageView>& image_views, 
                   GLFWwindow* w ,
                   VkDevice& dev,
                   VkInstance &inst) {

  vkDestroySwapchainKHR(dev, swapchain, nullptr);
  vkDestroySurfaceKHR (inst, surf, nullptr);

  for (auto shmod : shader_modules) {
    vkDestroyShaderModule (dev, shmod, nullptr); 
  }
  
  for (auto imageview : image_views) {
    vkDestroyImageView(dev, imageview, nullptr);
  }
  vkDestroyPipelineLayout(dev, pipeline_layout, nullptr);

  vkDestroyDevice (dev, nullptr); 
  vkDestroyInstance(inst, nullptr);

  
  glfwDestroyWindow(w);
}

