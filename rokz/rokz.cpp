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
bool rokz::CheckValidationSupport (const std::vector<const char*>& validation_layers) {

  printf ("%s\n", __FUNCTION__); 

  uint32_t layer_count  = 0;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
  
  std::vector<VkLayerProperties> available_layers(layer_count);

  vkEnumerateInstanceLayerProperties(&layer_count, &available_layers[0]);

  for (const char* layer_name : validation_layers) {
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
//, const std::vector<const char*>& validation_layers
int rokz::CreateInstance(rokz::Glob& glob) {
  
  // VkApplicationInfo
  glob.app_info.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  glob.app_info.pApplicationName = "Hello Triangle";
  glob.app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  glob.app_info.pEngineName      = "No Engine";
  glob.app_info.engineVersion    = VK_MAKE_VERSION(1, 1, 0);
  glob.app_info.apiVersion       = VK_API_VERSION_1_1;
  //glob.app_info.pNext = nullptr; 

  // VkInstanceCreateInfo
  glob.create_info.instance.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  glob.create_info.instance.pApplicationInfo = &glob.app_info;

  // 
  std::vector<const char*> req_exts; 
  GetRequiredExtensions (req_exts);  
  glob.create_info.instance.enabledExtensionCount = req_exts.size();
  glob.create_info.instance.ppEnabledExtensionNames = &req_exts[0];
  glob.create_info.instance.pNext = nullptr; 


  if (kEnableValidationLayers && CheckValidationSupport (validation_layers)) {
    printf ("ENABLE VALIDATION LAYERS\n"); 
    glob.create_info.instance.enabledLayerCount   = validation_layers.size(); 
    glob.create_info.instance.ppEnabledLayerNames = &validation_layers[0]; 
    // !! SETUP up additional output cb handling...
  }
  else {
    printf ("VALIDATION LAYERS ARE DISABLED\n"); 
    glob.create_info.instance.enabledLayerCount   = 0;
    glob.create_info.instance.ppEnabledLayerNames = nullptr;
  }

  // CREATEINSTANCE
  printf ("vkCreateInstance() \n"); 
  if (vkCreateInstance (&glob.create_info.instance, nullptr, &glob.instance) != VK_SUCCESS) {
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
    printf ("LEAVING[FALSE] %s\n", __FUNCTION__);
    return false;
  }
  swapchain_images.resize(image_count);
  res = vkGetSwapchainImagesKHR (dev, swapchain, &image_count, &swapchain_images[0]);
  if (res != VK_SUCCESS) {
    printf ("LEAVING[FALSE] %s\n", __FUNCTION__);
    return false;
  }

  printf ("LEAVING[TRUE] %s\n", __FUNCTION__);
  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateImageViews(std::vector<VkImageView> &swapchain_imageviews) {
  
  return false;   
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreatePipeline (){

  return false; 
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
void rokz::Cleanup (VkSwapchainKHR& swapchain, VkSurfaceKHR& surf, GLFWwindow* w , VkDevice& dev, VkInstance &inst) {

  vkDestroySwapchainKHR(dev, swapchain, nullptr);
  vkDestroySurfaceKHR (inst, surf, nullptr); 
  vkDestroyDevice (dev, nullptr); 
  vkDestroyInstance(inst, nullptr);
  
  glfwDestroyWindow(w);
}

