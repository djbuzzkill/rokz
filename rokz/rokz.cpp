#include "rokz.h"


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
bool AllQueuesAvailable (const rokz::QueueFamilyIndices&  inds) {

  return inds.graphics.has_value() && inds.present.has_value(); 
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

    VkBool32 presentSupport = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR (physdev, ique, surf, &presentSupport);

    if (presentSupport) {
      inds.present = ique; 
    }
    
    if (que_fam_props[ique].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      inds.graphics = ique;
    }
    
    // if (que_fam_props[ique].queueFlags & VK_QUEUE_PRESENT_BIT) {
    //   inds.present = ique;
    // }
    
    if (inds.graphics.has_value()) { 
      printf ("BREAK;\n"); 
      break;
    }
  }
  
  return inds;
}



bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device) {

  uint32_t exts_count;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &exts_count, nullptr);

  std::vector<VkExtensionProperties> available_exts(exts_count);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &exts_count, &available_exts[0]);

  
  std::set<std::string> req_exts (&device_extensions[0], &device_extensions[device_extensions.size()]);

  printf ("looking for extensions:\n"); 
  for (const auto& e : req_exts) {
    printf ("  .. %s\n", e.c_str ()); 
  }
  
  printf ("available extensions:\n"); 
  for (const auto& extension : available_exts) {
    printf ("  .. %s\n", extension.extensionName); 
    req_exts.erase(extension.extensionName);
  }
  
  return req_exts.empty();
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool IsDeviceSuitable (rokz::QueueFamilyIndices& outind, const VkSurfaceKHR& surf, const VkPhysicalDevice& physdev) {

  printf ("%s\n", __FUNCTION__); 

  VkPhysicalDeviceProperties devprops{};
  VkPhysicalDeviceFeatures   devfeatures{}; 

  bool exts_supported = CheckDeviceExtensionSupport (physdev); 
  
  vkGetPhysicalDeviceProperties(physdev, &devprops);
  vkGetPhysicalDeviceFeatures(physdev, &devfeatures);

  bool device_is_suitable =
    devprops.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
    && devfeatures.geometryShader;

  return exts_supported && AllQueuesAvailable (rokz::FindQueueFamilies (outind, surf, physdev)); 
  // return device_is_suitable;  
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
    // error
  }

  std::vector<VkPhysicalDevice> physdevs ( device_count, VK_NULL_HANDLE); 
  vkEnumeratePhysicalDevices(inst, &device_count, &physdevs[0]);
  for (int idev = 0; idev < device_count; ++idev) {

    if (IsDeviceSuitable (queueind, surf, physdevs[idev] )) {
      physdev = physdevs[idev];
      printf ("found PHYSICAL DEVICE\n");
      return true; 
    }
  }

  return false; 
}




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
  
  printf("WOOOOO!!!1\n");
  
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
  g.app_info            = {};
  g.device_features     = {};
  g.create_info.instance= {};
  g.create_info.device  = {};
  g.create_info.queue   = {};
  return g; 
}  

VkDeviceQueueCreateInfo& rokz::Default (VkDeviceQueueCreateInfo& info, uint32_t que_fam_index, float* q_priority){

  info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; 
  info.queueFamilyIndex = que_fam_index ; // glob.queue_fams.graphics.value();
  info.queueCount       = 1; 
  info.pQueuePriorities = q_priority;

  return info; 
}

VkDeviceCreateInfo& rokz::Default (VkDeviceCreateInfo& info, VkDeviceQueueCreateInfo* quecreateinfo, VkPhysicalDeviceFeatures* devfeats) {
  
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
void rokz::GetDeviceQueue (VkQueue* que, uint32_t fam_ind, const VkDevice& device) { 
  const uint32_t que_index = 0; // diffrnt than que_fam_ind
  return vkGetDeviceQueue (device, fam_ind, que_index, que);
}
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
void rokz::Cleanup (VkSurfaceKHR& surf, GLFWwindow* w , VkDevice& dev, VkInstance &inst) {

  vkDestroySurfaceKHR (inst, surf, nullptr); 
  vkDestroyDevice (dev, nullptr); 
  vkDestroyInstance(inst, nullptr);
  
  glfwDestroyWindow(w);
}

