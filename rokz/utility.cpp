
#include "utility.h"


using namespace rokz; 
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CheckValidationSupport (const std::vector<std::string>& vlayers) {

  printf ("%s\n", __FUNCTION__); 

  uint32_t layer_count  = 0;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
  
  std::vector<VkLayerProperties> available_layers(layer_count);

  vkEnumerateInstanceLayerProperties(&layer_count, &available_layers[0]);

  for (const std::string&  lname : vlayers) {
    printf ("   LOOKING FOR[%s]\n", lname.c_str());
    bool layer_found = false;

    for (const auto& layerProperties : available_layers) {
      if (strcmp(lname.c_str(), layerProperties.layerName) == 0) {
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
std::vector<std::string>& rokz::GetRequiredExtensionNames (std::vector<std::string>& extstrs) {
  // GLFW REQ'D EXTENSIONS
  uint32_t extcount = 0;
  const char **glfw_exts; 
  glfw_exts = glfwGetRequiredInstanceExtensions (&extcount);
  printf ("num glfw extensions[%u]\n" , extcount); 

  extstrs.clear ();
  for (uint32_t  i = 0; i < extcount; ++i) {
    printf ("req_extent(%i)[%s]\n", i, glfw_exts[i] );
    extstrs.push_back (glfw_exts[i]); 
  }

  return extstrs;
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

// -----------------------------------------------------------------------
//
// -----------------------------------------------------------------------
bool rokz::FindSupportedFormat (VkFormat&                    out,
                                const std::vector<VkFormat>& candidates,
                                VkImageTiling                tiling,
                                VkFormatFeatureFlags         features, 
                                const VkPhysicalDevice&      physdev) {

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
