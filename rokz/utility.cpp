
#include "utility.h"


using namespace rokz; 


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
const std::vector<const char*> device_extensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


const std::vector<const char*>& rokz::GetDeviceExtensionNames () {
  return device_extensions; 
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
bool rokz::CheckDeviceExtensionSupport(const VkPhysicalDevice& physdev) {
  printf ("%s\n", __FUNCTION__);

  uint32_t exts_count;
  vkEnumerateDeviceExtensionProperties(physdev, nullptr, &exts_count, nullptr);

  std::vector<VkExtensionProperties> available_exts(exts_count);
  vkEnumerateDeviceExtensionProperties(physdev, nullptr, &exts_count, &available_exts[0]);

  
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

  printf ("LEAVING:%s\n", __FUNCTION__);
  return deets;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool IsDeviceSuitable (rokz::QueueFamilyIndices& outind, VkPhysicalDeviceProperties& devprops, const VkSurfaceKHR& surf, const VkPhysicalDevice& physdev) {

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

  devfeatures.samplerAnisotropy = VK_TRUE; // force anistotropy
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
bool rokz::SelectPhysicalDevice (VkPhysicalDevice&           physdev,
                                 VkPhysicalDeviceProperties& device_props,
                                 rokz::QueueFamilyIndices&   queueind,
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

  for (unsigned idev = 0; idev < device_count; ++idev) {

    rokz::QueueFamilyIndices curqueinds {}; 
    device_props = {}; 
    if (IsDeviceSuitable (curqueinds,  device_props, surf, physdevs[idev])) {
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
std::vector<const char*>& rokz::GetRequiredExtensionNames (std::vector<const char*>&  exts) {

  exts.clear ();

  // GLFW REQ'D EXTENSIONS
  uint32_t glfw_ext_count = 0;
  const char **glfw_exts;
  glfw_exts = glfwGetRequiredInstanceExtensions(&glfw_ext_count);
  printf ("glfw_ext_count[%u]\n" , glfw_ext_count); 

  for  (uint32_t  i = 0; i < glfw_ext_count; ++i) {
    printf ("req_extent(%i)[%s]\n", i, glfw_exts[i] );
    exts.push_back ( glfw_exts[i] ); 
  }
  return exts; 
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::FindMemoryType (uint32_t& type_index, uint32_t type_filter, VkMemoryPropertyFlags prop_flags, const VkPhysicalDevice& physdev) {
    
  VkPhysicalDeviceMemoryProperties mem_props;
  vkGetPhysicalDeviceMemoryProperties(physdev, &mem_props);

  for (uint32_t i = 0; i < mem_props.memoryTypeCount; i++) {
   
    if (type_filter & (1 << i) && (mem_props.memoryTypes[i].propertyFlags & prop_flags) == prop_flags ) {
      type_index = i; 
      return true;
    }
  }

  printf ("[%s] find memory type failed", __FUNCTION__);
  return false; 
}


//
bool rokz::FindSupportedFormat  (VkFormat& out, const std::vector<VkFormat>& candidates,
                           VkImageTiling tiling,
                           VkFormatFeatureFlags features, 
                           const VkPhysicalDevice& physdev) {


  for (VkFormat format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physdev, format, &props);

    if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
      out = format;
      return true;

    } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
      out = format;
      return true; 
    }  
  }

  printf ("[FAILED] %s find supported format", __FUNCTION__);
  return false; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::FindDepthFormat (VkFormat& outfmt,  const VkPhysicalDevice& physdev) {

  std::vector<VkFormat> candidates = 
    {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT};
  
  return FindSupportedFormat (outfmt,
                              candidates,
                              VK_IMAGE_TILING_OPTIMAL,
                              VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, 
                              physdev) ; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::HasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkSampleCountFlagBits rokz::MaxUsableSampleCount  (VkPhysicalDevice physdev) {
    VkPhysicalDeviceProperties phys_dev_props;
    vkGetPhysicalDeviceProperties (physdev, &phys_dev_props);

    VkSampleCountFlags counts = phys_dev_props.limits.framebufferColorSampleCounts & phys_dev_props.limits.framebufferDepthSampleCounts;
    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

    return VK_SAMPLE_COUNT_1_BIT;
}
