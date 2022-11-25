

#include "rokz.h"


#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include "rokz/rokz_funcs.h"

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

// kittyCAD API Token: 
// b78b74a3-2183-45f6-8bf1-4b996e9a825b
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
  g.create_info.renderpass = {};
  g.create_info.pipeline = {}; 

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
  printf ("%s\n", __FUNCTION__); 
  
  swapchain_imageviews.resize (swapchain_images.size()); 
  
  for (size_t i = 0; i < swapchain_imageviews.size(); i++) {

    VkImageViewCreateInfo createinfo = {};

    Default (createinfo, swapchain_images[i], surf_fmt); 

    VkResult res = vkCreateImageView (dev, &createinfo, nullptr, &swapchain_imageviews[i]);
    if (res != VK_SUCCESS) {
      return false; 
    }
  }

  printf ("BAI %s\n", __FUNCTION__); 
  return true;   
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkShaderModule& rokz::CreateShaderModule (VkShaderModule& shader_module, const rokz::bytearray& code, const VkDevice& dev) {

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
bool rokz::CreateShaderModules (
    std::vector<VkShaderModule> &shader_modules,
    std::vector<VkPipelineShaderStageCreateInfo> &shader_stage_create_infos, 
    const std::filesystem::path& fspath,
    const VkDevice& device) {
  
  printf ("%s\n", __FUNCTION__); 
  shader_stage_create_infos.resize (2);
  

  // VERT SHADER 
  std::filesystem::path vert_file_path  =  fspath / "data/shader/basic_vert.spv" ;
  printf ("[1] %s\n",   vert_file_path.string().c_str() );

  bytearray      vertbin;
  VkShaderModule vertmod; 
  From_file(vertbin, vert_file_path.string());
  if (!rokz::CreateShaderModule (vertmod,  vertbin, device))
    return false; 
  
  printf ("[2] %s\n", __FUNCTION__); 
  shader_modules.push_back (vertmod); 
  VkPipelineShaderStageCreateInfo& vert_shader_stage_info = shader_stage_create_infos[0];
  vert_shader_stage_info = {};   
  vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vert_shader_stage_info.module = vertmod;
  vert_shader_stage_info.pSpecializationInfo = nullptr; 
  vert_shader_stage_info.pName = "main";

  printf ("[3] %s\n", __FUNCTION__); 
  // FRAG SHADER
  std::filesystem::path frag_file_path = fspath /  "data/shader/basic_frag.spv" ;
  bytearray      fragbin;
  VkShaderModule fragmod; 
  From_file(fragbin, frag_file_path.string());
  if (!rokz::CreateShaderModule (fragmod,  fragbin, device))
    return false; 
  
  shader_modules.push_back (fragmod); 
  VkPipelineShaderStageCreateInfo& frag_shader_stage_info  = shader_stage_create_infos[1];
  frag_shader_stage_info = {}; 
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


bool rokz::CreateColorBlendState (VkPipelineColorBlendAttachmentState&   color_blend_attachment_state,
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
bool rokz::CreateGraphicsPipelineLayout(
    VkPipelineLayout&                  pipeline_layout,
    VkViewport&                        viewport,
    VkRect2D&                          scissor,
    CreateInfo&                        create_info,
    const VkExtent2D&                  swapchain_extent,
    const VkDevice&                    device)
{

  VkPipelineLayoutCreateInfo&                   pipeline_layout_create_info = create_info.pipeline_layout; 
  std::vector<VkPipelineShaderStageCreateInfo>& shader_stages_create_info = create_info.shader_stages;
  VkPipelineVertexInputStateCreateInfo&         vertex_input_state_info = create_info.vertex_input_state;
  VkPipelineInputAssemblyStateCreateInfo&       input_assembly = create_info.input_assembly; 
  VkPipelineRasterizationStateCreateInfo&       rasterizer = create_info.rasterizer;
  VkPipelineMultisampleStateCreateInfo&         multisampling = create_info.multisampling;
  VkPipelineDepthStencilStateCreateInfo&        pipeline_depth_stencil_create_info = create_info.pipeline_depth_stencil; 
  //VkPipelineColorBlendStateCreateInfo&          color_blending_create_info = create_info.color_blending;
  VkPipelineViewportStateCreateInfo&            viewport_state_create_info = create_info.viewport_state;

  
  // INPUT ASSEMBLY STATE
  input_assembly = {};
  input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly.primitiveRestartEnable = VK_FALSE;


  // VIEWPORT 
  viewport = {};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)swapchain_extent.width;
  viewport.height = (float)swapchain_extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  // SCISSOR RECT
  scissor = {}; 
  scissor.offset = {0, 0};
  scissor.extent = swapchain_extent;

  // VkPipelineViewportStateCreateInfo
  viewport_state_create_info = {};
  viewport_state_create_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state_create_info.viewportCount = 1;

  viewport_state_create_info.pViewports = &viewport;
  viewport_state_create_info.scissorCount = 1;
  viewport_state_create_info.pScissors = &scissor;

  // RASTERIZATION STATE .. VkPipelineRasterizationStateCreateInfo
  rasterizer = {};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 0.0f; 
  rasterizer.depthBiasClamp = 0.0f;          
  rasterizer.depthBiasSlopeFactor = 0.0f;    

  // MULTI SAMPLING
  multisampling = {};
  multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading = 1.0f;          
  multisampling.pSampleMask = nullptr;            
  multisampling.alphaToCoverageEnable = VK_FALSE; 
  multisampling.alphaToOneEnable = VK_FALSE;      

  // DEPTH/STENCIL
  pipeline_depth_stencil_create_info = {};

  // PIPELINE LAYOUT CREATE INFO << mostly empty for now
  pipeline_layout_create_info = {};
  pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout_create_info.setLayoutCount = 0;            
  pipeline_layout_create_info.pSetLayouts = nullptr;         
  pipeline_layout_create_info.pushConstantRangeCount = 0;    
  pipeline_layout_create_info.pPushConstantRanges = nullptr; 

  if (vkCreatePipelineLayout(device, &pipeline_layout_create_info, nullptr, &pipeline_layout) != VK_SUCCESS) {
    printf("failed to create pipeline layout!\n");
    return false;
  }

  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateGraphicsPipeline (
    VkPipeline&                   pipeline,
    VkGraphicsPipelineCreateInfo& pipeline_create_info,
    const CreateInfo&             create_info,
    const VkPipelineLayout&       pipeline_layout,
    const RenderPass&           render_pass,
    const VkDevice                device) {

  pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_create_info.stageCount = 2;
  pipeline_create_info.pStages = &create_info.shader_stages[0]; 
  pipeline_create_info.pVertexInputState = &create_info.vertex_input_state; ;
  pipeline_create_info.pInputAssemblyState = &create_info.input_assembly;
  pipeline_create_info.pViewportState = &create_info.viewport_state;
  pipeline_create_info.pRasterizationState = &create_info.rasterizer;
  pipeline_create_info.pMultisampleState = &create_info.multisampling;
  pipeline_create_info.pDepthStencilState = nullptr; // create_info.pipeline_depth_stencil;  
  pipeline_create_info.pColorBlendState = &create_info.color_blend; 
  pipeline_create_info.pDynamicState = &create_info.dynamic_state; 
  pipeline_create_info.layout = pipeline_layout; 
  pipeline_create_info.renderPass = render_pass.handle;
  pipeline_create_info.subpass = 0;
  pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE; 
  pipeline_create_info.basePipelineIndex = -1;              

  if (vkCreateGraphicsPipelines (device, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &pipeline) != VK_SUCCESS) {
    printf("failed to create graphics pipeline!");
    return false;
  }

  return true; 
  
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateRenderPass (RenderPass &render_pass, VkFormat swapchain_format, const VkDevice &device) {
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
  //VkAttachmentReference color_attachment_ref{};
  render_pass.attach_ref[co_in] = {};
  render_pass.attach_ref[co_in].attachment = co_in; // index
  render_pass.attach_ref[co_in].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  // SUBPASS, just 1 for now                 
  render_pass.subpass_descs.resize (1);
  render_pass.subpass_descs[co_in] = {};
  render_pass.subpass_descs[co_in].pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
  render_pass.subpass_descs[co_in].colorAttachmentCount    = 1;
  render_pass.subpass_descs[co_in].pColorAttachments       = render_pass.attach_ref ;
  render_pass.subpass_descs[co_in].inputAttachmentCount    = 0;
  render_pass.subpass_descs[co_in].pInputAttachments       = nullptr;
  render_pass.subpass_descs[co_in].pDepthStencilAttachment = nullptr;
  render_pass.subpass_descs[co_in].preserveAttachmentCount = 0;
  render_pass.subpass_descs[co_in].pPreserveAttachments    = nullptr;
  render_pass.subpass_descs[co_in].pResolveAttachments     = nullptr;
  render_pass.subpass_descs[co_in].flags = 0 ;
  //
  render_pass.dependancy = {};
  render_pass.dependancy.srcSubpass    = VK_SUBPASS_EXTERNAL;
  render_pass.dependancy.dstSubpass    = co_in;
  render_pass.dependancy.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  render_pass.dependancy.srcAccessMask = co_in;
  render_pass.dependancy.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  render_pass.dependancy.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  
  // CREATEINFO. gets passed back out
  render_pass.create_info = {}; 
  render_pass.create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass.create_info.attachmentCount = 1;
  render_pass.create_info.pAttachments = render_pass.attach_desc;
  render_pass.create_info.subpassCount = 1;
  render_pass.create_info.pSubpasses = &render_pass.subpass_descs[0];
  render_pass.create_info.dependencyCount = 1;
  render_pass.create_info.pDependencies = &render_pass.dependancy;
  render_pass.create_info.pNext = nullptr; 
  //
  if (vkCreateRenderPass(device, &render_pass.create_info, nullptr, &render_pass.handle) != VK_SUCCESS) {
    printf ("failed to create render pass!\n");
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
    const VkDevice&                       device) {

  framebuffers.resize (image_views.size()); 
  create_infos.resize (image_views.size()); 
  

  for (size_t i = 0; i < image_views.size(); i++) {

    VkImageView attachments[] = {image_views[i]};

    create_infos[i]  = {}; 
    create_infos[i].sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    create_infos[i].renderPass = render_pass.handle;
    create_infos[i].attachmentCount = 1;
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

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateBuffer (BufferStruc&            buffstruc, 
                         const VkDevice&         device,
                         const VkPhysicalDevice& physdev) {

  if (vkCreateBuffer (device, &buffstruc.create_info, nullptr, &buffstruc.handle) != VK_SUCCESS) {
    printf ("failed to create buffer!");
    return false; 
  }
  
  VkMemoryRequirements mem_reqs;
  vkGetBufferMemoryRequirements(device, buffstruc.handle, &mem_reqs);
    
  buffstruc.alloc_info = {}; 
  buffstruc.alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  buffstruc.alloc_info.allocationSize = mem_reqs.size;

  buffstruc.alloc_info.memoryTypeIndex =
    rokz::FindMemoryType (mem_reqs.memoryTypeBits, buffstruc.mem_prop_flags, physdev);

  if (vkAllocateMemory(device, &buffstruc.alloc_info, nullptr, &buffstruc.mem) != VK_SUCCESS) {
    printf ("failed to allocate buffer memory!");
    return false; 
  }

  if (VK_SUCCESS != vkBindBufferMemory (device, buffstruc.handle, buffstruc.mem, 0)) {
    printf ("[%s]  VB memory bind failed\n", __FUNCTION__);
    return false;
  }

  return true; 
} 


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
//
// ---------------------------------------------------------------------
bool rokz::CreateVertexBuffer (BufferStruc& buffstruc, 
                               size_t sizeof_elem,
                               size_t num_elem, 
                               const VkDevice& device,
                               const VkPhysicalDevice& physdev) {
  //
  buffstruc.create_info = {};
  buffstruc.create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffstruc.create_info.size  = sizeof_elem * num_elem; 
  buffstruc.create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT; 
  buffstruc.create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  
  buffstruc.mem_prop_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT; 
  //
  return rokz::CreateBuffer (buffstruc, device, physdev); 
}

// ---------------------------------------------------------------------
// transfer buffer
// ---------------------------------------------------------------------
bool rokz::CreateVertexBuffer_transfer (BufferStruc& buffstruc, 
                               size_t sizeof_elem,
                               size_t num_elem, 
                               const VkDevice& device,
                               const VkPhysicalDevice& physdev) {
  //
  buffstruc.create_info = {};
  buffstruc.create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffstruc.create_info.size  = sizeof_elem * num_elem; 
  buffstruc.create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT; // | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  buffstruc.create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  
  buffstruc.mem_prop_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT; 
  //
  return rokz::CreateBuffer (buffstruc, device, physdev); 
}

// ---------------------------------------------------------------------
//   device buffer
// ---------------------------------------------------------------------
bool rokz::CreateVertexBuffer_device (BufferStruc& buffstruc, 
                                      size_t sizeof_elem,
                                      size_t num_elem, 
                                      const VkDevice& device,
                                      const VkPhysicalDevice& physdev) {
  //
  buffstruc.create_info = {};
  buffstruc.create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffstruc.create_info.size  = sizeof_elem * num_elem; 
  buffstruc.create_info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  buffstruc.create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  
  buffstruc.mem_prop_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT; 
  //
  return rokz::CreateBuffer (buffstruc, device, physdev); 
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::MoveToBuffer_XB2DB  (BufferStruc& buff_dst, // device buffer
                                BufferStruc& buff_src, // user buffer, 
                                size_t size,
                                const VkCommandPool& command_pool, 
                                const VkQueue& que, 
                                const VkDevice& device) {

  if ( !(buff_src.create_info.usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT)) {
    printf ("buff_src is not TRANSFER_SRC "); 
    return false; 
  }

  if ( !(buff_dst.create_info.usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT)) {
    printf ("buff_dst is not TRANSFER_DST "); 
    return false; 
  }
  
  VkCommandBufferAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandPool = command_pool;
  alloc_info.commandBufferCount = 1;

  VkCommandBuffer cmdbuf;
  vkAllocateCommandBuffers(device, &alloc_info, &cmdbuf);

  VkCommandBufferBeginInfo begin_info{};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vkBeginCommandBuffer (cmdbuf, &begin_info);

  VkBufferCopy copy_desc{};
  copy_desc.srcOffset = 0; 
  copy_desc.dstOffset = 0; 
  copy_desc.size = size;

  vkCmdCopyBuffer(cmdbuf, buff_src.handle, buff_dst.handle, 1, &copy_desc);
  vkEndCommandBuffer(cmdbuf);    

  VkSubmitInfo submit_info{};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &cmdbuf;
    
  vkQueueSubmit(que, 1, &submit_info, VK_NULL_HANDLE);
  vkQueueWaitIdle(que);
  return false; 
}

// ---------------------------------------------------------------------
// lock/unlock vb
// ---------------------------------------------------------------------
bool rokz::MapBuffer (void** ptr, BufferStruc& vb, const VkDevice& device) { 

  assert (vb.mem_prop_flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT); 
  if (vb.mem_prop_flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
    return VK_SUCCESS == vkMapMemory (device, vb.mem, 0, vb.create_info.size, 0, ptr);
  }
  
  return false; 
}

// 
void rokz::UnmapBuffer (BufferStruc& vb, const VkDevice& device) {
  vkUnmapMemory (device, vb.mem);
}

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
bool rokz::MoveToBuffer_user_mem (BufferStruc& dst_vb, const void* src, size_t size, const VkDevice& device) {
  void* dst;

  if (VK_SUCCESS == vkMapMemory (device, dst_vb.mem, 0, dst_vb.create_info.size, 0, &dst)) {
    
    memcpy (dst, src, size);
    vkUnmapMemory (device, dst_vb.mem);
    return true; 
  }
  return false;
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
  VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}}; 
  pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  pass_info.renderPass = render_pass.handle; 
  pass_info.framebuffer = framebuffer; 
  pass_info.renderArea.offset = {0, 0};
  pass_info.renderArea.extent = ext2d;
  pass_info.clearValueCount = 1; 
  pass_info.pClearValues = &clear_color; 

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
bool rokz::FindMemoryType (uint32_t& type_filter, VkMemoryPropertyFlags prop_flags, const VkPhysicalDevice& physdev) {
    
  VkPhysicalDeviceMemoryProperties mem_props;
  vkGetPhysicalDeviceMemoryProperties(physdev, &mem_props);

  for (uint32_t i = 0; i < mem_props.memoryTypeCount; i++) {
   
    if (type_filter & (1 << i) && (mem_props.memoryTypes[i].propertyFlags & prop_flags) == prop_flags ) {
      type_filter = i; 
      return true;
    }
  }

  
  printf ("[%s] find memory type failed", __FUNCTION__);
  return false; 
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
//
// ---------------------------------------------------------------------
bool rokz::RecreateSwapchain(VkSwapchainKHR &swapchain,
                       VkSwapchainCreateInfoKHR &swapchaincreateinfo,
                       std::vector<VkImage> &swapchain_images,
                       std::vector<VkFramebuffer> &framebuffers,
                       std::vector<VkFramebufferCreateInfo> &create_infos,
                       RenderPass &render_pass,
                       std::vector<VkImageView> &image_views,
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
  bool framebuffers_res = CreateFramebuffers (framebuffers, create_infos, render_pass, swapchaincreateinfo.imageExtent, image_views, dev);
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
                   std::vector<VkShaderModule> &shader_modules,
                   VkPipelineLayout &pipeline_layout,
                   RenderPass &render_pass,
                   std::vector<VkImageView> &image_views,
                   GLFWwindow *w,
                   VkDevice &dev, VkInstance &inst) {



  CleanupSwapchain (framebuffers, image_views, swapchain, dev);

  vkDestroyBuffer (dev, vbstruc.handle, nullptr); 
  vkFreeMemory (dev, vbstruc.mem, nullptr);

  
  vkDestroyPipeline (dev, pipeline, nullptr);
  vkDestroySurfaceKHR (inst, surf, nullptr);
  vkDestroyCommandPool ( dev, command_pool, nullptr);

  for (size_t i = 0; i < syncs.size (); ++i) {
    vkDestroySemaphore(dev, syncs[i].image_available_sem, nullptr);
    vkDestroySemaphore(dev, syncs[i].render_fnished_sem, nullptr);
    vkDestroyFence(dev, syncs[i].in_flight_fen, nullptr);
  }

  for (auto shmod : shader_modules) {
    vkDestroyShaderModule (dev, shmod, nullptr); 
  }
  


  vkDestroyPipelineLayout(dev, pipeline_layout, nullptr);
  vkDestroyRenderPass (dev, render_pass.handle, nullptr); 
  
  vkDestroyDevice (dev, nullptr); 
  vkDestroyInstance(inst, nullptr);

  
  glfwDestroyWindow(w);
}
