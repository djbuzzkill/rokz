
#include "utility.h"


using namespace rokz; 
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::ut::CheckValidationSupport (const std::vector<std::string>& vlayers) {

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
std::vector<std::string>& rokz::ut::GetRequiredExtensionNames (std::vector<std::string>& extstrs) {
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
bool rokz::ut::FindMemoryType (uint32_t& type_index, uint32_t type_filter, VkMemoryPropertyFlags prop_flags, const VkPhysicalDevice& physdev) {
    
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
bool rokz::ut::FindSupportedFormat (VkFormat&                    out,
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
bool rokz::ut::FindDepthFormat (VkFormat& outfmt,  const VkPhysicalDevice& physdev) {

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
bool rokz::ut::HasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkSampleCountFlagBits rokz::ut::MaxUsableSampleCount  (VkPhysicalDevice physdev) {
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



const std::map<VkResult, const char*>  kVulkanResult_ERROR_STRINGS = {

{ VK_SUCCESS,                                            "VK_SUCCESS" },                                          //  Command successfully completed
{ VK_NOT_READY,                                          "VK_NOT_READY" },                                        //  A fence or query has not yet completed
{ VK_TIMEOUT,                                            "VK_TIMEOUT" },                                          //  A wait operation has not completed in the specified time
{ VK_EVENT_SET,                                          "VK_EVENT_SET" },                                        //  An event is signaled
{ VK_EVENT_RESET,                                        "VK_EVENT_RESET" },                                      //  An event is unsignaled
{ VK_INCOMPLETE,                                         "VK_INCOMPLETE" },                                       //  A return array was too small for the result
{ VK_SUBOPTIMAL_KHR,                                     "VK_SUBOPTIMAL_KHR" },                                   //  A swapchain no longer matches the surface properties exactly, but can still be used to present to the surface successfully.
{ VK_THREAD_IDLE_KHR,                                    "VK_THREAD_IDLE_KHR" },                                  //  A deferred operation is not complete but there is currently no work for this thread to do at the time of this call.
{ VK_THREAD_DONE_KHR,                                    "VK_THREAD_DONE_KHR" },                                  //  A deferred operation is not complete but there is no work remaining to assign to additional threads.
{ VK_OPERATION_DEFERRED_KHR,                             "VK_OPERATION_DEFERRED_KHR" },                           //  A deferred operation was requested and at least some of the work was deferred.
{ VK_OPERATION_NOT_DEFERRED_KHR,                         "VK_OPERATION_NOT_DEFERRED_KHR" },                       //  A deferred operation was requested and no operations were deferred.
{ VK_PIPELINE_COMPILE_REQUIRED,                          "VK_PIPELINE_COMPILE_REQUIRED" },                        //  A requested pipeline creation would have required compilation, but the application requested compilation to not be performed.
{ VK_ERROR_OUT_OF_HOST_MEMORY,                           "VK_ERROR_OUT_OF_HOST_MEMORY" },                         //  A host memory allocation has failed.
{ VK_ERROR_OUT_OF_DEVICE_MEMORY,                         "VK_ERROR_OUT_OF_DEVICE_MEMORY" },                       //  A device memory allocation has failed.
{ VK_ERROR_INITIALIZATION_FAILED,                        "VK_ERROR_INITIALIZATION_FAILED" },                      //  Initialization of an object could not be completed for implementation-specific reasons.
{ VK_ERROR_DEVICE_LOST,                                  "VK_ERROR_DEVICE_LOST" },                                //  The logical or physical device has been lost. See Lost Device
{ VK_ERROR_MEMORY_MAP_FAILED,                            "VK_ERROR_MEMORY_MAP_FAILED" },                          //  Mapping of a memory object has failed.
{ VK_ERROR_LAYER_NOT_PRESENT,                            "VK_ERROR_LAYER_NOT_PRESENT" },                          //  A requested layer is not present or could not be loaded.
{ VK_ERROR_EXTENSION_NOT_PRESENT,                        "VK_ERROR_EXTENSION_NOT_PRESENT" },                      //  A requested extension is not supported.
{ VK_ERROR_FEATURE_NOT_PRESENT,                          "VK_ERROR_FEATURE_NOT_PRESENT" },                        //  A requested feature is not supported.
{ VK_ERROR_INCOMPATIBLE_DRIVER,                          "VK_ERROR_INCOMPATIBLE_DRIVER" },                        //  The requested version of Vulkan is not supported by the driver or is otherwise incompatible for implementation-specific reasons.
{ VK_ERROR_TOO_MANY_OBJECTS,                             "VK_ERROR_TOO_MANY_OBJECTS" },                           //  Too many objects of the type have already been created.
{ VK_ERROR_FORMAT_NOT_SUPPORTED,                         "VK_ERROR_FORMAT_NOT_SUPPORTED" },                       //  A requested format is not supported on this device.
{ VK_ERROR_FRAGMENTED_POOL,                              "VK_ERROR_FRAGMENTED_POOL" },                            //  A pool allocation has failed due to fragmentation of the pool’s memory. This must only be returned if no attempt to allocate host or device memory was made to accommodate the new allocation. This should be returned in preference to VK_ERROR_OUT_OF_POOL_MEMORY, but only if the implementation is certain that the pool allocation failure was due to fragmentation.
{ VK_ERROR_SURFACE_LOST_KHR,                             "VK_ERROR_SURFACE_LOST_KHR" },                           //  A surface is no longer available.
{ VK_ERROR_NATIVE_WINDOW_IN_USE_KHR,                     "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR" },                   //  The requested window is already in use by Vulkan or another API in a manner which prevents it from being used again.
{ VK_ERROR_OUT_OF_DATE_KHR,                              "VK_ERROR_OUT_OF_DATE_KHR" },                            //  A surface has changed in such a way that it is no longer compatible with the swapchain, and further presentation requests using the swapchain will fail. Applications must query the new surface properties and recreate their swapchain if they wish to continue presenting to the surface.
{ VK_ERROR_INCOMPATIBLE_DISPLAY_KHR,                     "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR" },                   //  The display used by a swapchain does not use the same presentable image layout, or is incompatible in a way that prevents sharing an image.
{ VK_ERROR_INVALID_SHADER_NV,                            "VK_ERROR_INVALID_SHADER_NV" },                          //  One or more shaders failed to compile or link. More details are reported back to the application via VK_EXT_debug_report if enabled.
{ VK_ERROR_OUT_OF_POOL_MEMORY,                           "VK_ERROR_OUT_OF_POOL_MEMORY" },                         //  A pool memory allocation has failed. This must only be returned if no attempt to allocate host or device memory was made to accommodate the new allocation. If the failure was definitely due to fragmentation of the pool, VK_ERROR_FRAGMENTED_POOL should be returned instead.
{ VK_ERROR_INVALID_EXTERNAL_HANDLE,                      "VK_ERROR_INVALID_EXTERNAL_HANDLE" },                    //  An external handle is not a valid handle of the specified type.
{ VK_ERROR_FRAGMENTATION,                                "VK_ERROR_FRAGMENTATION" },                              //  A descriptor pool creation has failed due to fragmentation.
{ VK_ERROR_INVALID_DEVICE_ADDRESS_EXT,                   "VK_ERROR_INVALID_DEVICE_ADDRESS_EXT" },                 //  A buffer creation failed because the requested address is not available.
{ VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS,               "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS" },             //  A buffer creation or memory allocation failed because the requested address is not available. A shader group handle assignment failed because the requested shader group handle information is no longer valid.
{ VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT,          "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT" },        //  An operation on a swapchain created with VK_FULL_SCREEN_EXCLUSIVE_APPLICATION_CONTROLLED_EXT failed as it did not have exclusive full-screen access. This may occur due to implementation-dependent reasons, outside of the application’s control.
{ VK_ERROR_COMPRESSION_EXHAUSTED_EXT,                    "VK_ERROR_COMPRESSION_EXHAUSTED_EXT" },                  //  An image creation failed because internal resources required for compression are exhausted. This must only be returned when fixed-rate compression is requested.
// { VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR,                "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR" },              //  The requested VkImageUsageFlags are not supported.
// { VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR,       "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR" },     //  The requested video picture layout is not supported.
// { VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR,    "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR" },  //  A video profile operation specified via VkVideoProfileInfoKHR::videoCodecOperation is not supported.
// { VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR,       "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR" },     //  Format parameters in a requested VkVideoProfileInfoKHR chain are not supported.
// { VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR,        "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR" },      //  Codec-specific parameters in a requested VkVideoProfileInfoKHR chain are not supported.
// { VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR,          "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR" },        //  The specified video Std header version is not supported.
// { VK_ERROR_UNKNOWN,                                      "VK_ERROR_UNKNOWN" },                                    //  An unknown error has occurred; either the application has provided invalid input, or an implementation failure has occurred.
 };

const char* kVULKAN_ERROR_NOT_FOUND = "ERROR STRING NOT FOUND"; 

const char* rokz::ut::VkResult_2_string (VkResult r) {
  
  if (kVulkanResult_ERROR_STRINGS.count (r)) {
    return kVulkanResult_ERROR_STRINGS.at(r);
  }
  return  kVULKAN_ERROR_NOT_FOUND; 
}


                                                      
