
#include "context.hpp"
#include "utility.hpp"

#include "window.hpp"
#include "buffer.hpp"
#include "image.hpp"
#include "pipeline.hpp"
#include "framebuffer.hpp"
//#include "rokz/shared_types.h"
#include "shader.hpp"
#include "window.hpp"
#include "descriptor.hpp"
#include "allocation.hpp"
#include "command.hpp"
#include <vulkan/vulkan_core.h>


const bool kEnableValidationLayers = true;
  //#endif


// ----------------------------------------------------------------------------------------------
//                          
// ----------------------------------------------------------------------------------------------
const std::vector<const char*> kDeviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME, 
  VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
  VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME, 
  VK_EXT_EXTENDED_DYNAMIC_STATE_3_EXTENSION_NAME, 
  //"VK_EXT_descriptor_indexing", 
};

//#define VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME "VK_EXT_extended_dynamic_state2"

std::vector<std::string>& GetDeviceExtensionNames (std::vector<std::string>& dxstrs){

  dxstrs.clear (); 
  for (auto s : kDeviceExtensions) 
    dxstrs.push_back (s); 
    
  return dxstrs; 
}
// const std::vector<const char*>& rokz::GetDeviceExtensionNames () {
//   return kDeviceExtensions; 
// }


static const std::vector<std::string> kValidationLayers  = {
  "VK_LAYER_KHRONOS_validation",
};


std::vector<std::string>& GetValidationLayers (std::vector<std::string>& vstrs) {
  vstrs.clear ();
  for  (size_t i = 0; i < kValidationLayers.size (); ++i  ) {
    vstrs.push_back (kValidationLayers [i] ); 
  }
  return vstrs; 
}

// ----------------------------------------------------------------------------------------------
//                          
// ----------------------------------------------------------------------------------------------
rokz::SwapchainInfo& rokz::cx::QuerySwapchainInfo (SwapchainInfo& si, VkSurfaceKHR surf, VkPhysicalDevice device) {

  printf ("%s\n", __FUNCTION__);
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR (device, surf, &si.capabilities);

  uint32_t format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surf, &format_count, nullptr);
  printf ("surface format count [%u]\n", format_count); 
  
  if (format_count != 0) {
    si.formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surf, &format_count, &si.formats[0]);
  }
  
  uint32_t present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surf, &present_mode_count,  nullptr);
  printf ("present mode count [%u]\n", present_mode_count); 

  if (present_mode_count != 0) {
    si.present_modes.resize(present_mode_count);
    // just use the index 0
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surf, &present_mode_count, &si.present_modes[0]);
  }

  return si;
}

// ------------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------------
VkApplicationInfo& rokz::cx::AppInfo_default (VkApplicationInfo& app_info) {

  static const char* kAppName  = "ROKZ";
  static const char* kEngineName = "null";

  app_info.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pNext = nullptr;
  app_info.pApplicationName = kAppName;
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName      = kEngineName;
  app_info.engineVersion    = VK_MAKE_VERSION(1, 3, 0);
  app_info.apiVersion       = VK_API_VERSION_1_3; 
  return app_info;
}

// ------------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------------
VkInstanceCreateInfo& rokz::cx::CreateInfo (VkInstanceCreateInfo& ci,
                                            Vec<const char*>& required_extensions,
                                            Vec<std::string>& extstrs,
                                            Vec<const char*>& vls,
                                            Vec<std::string>& vstrs,
                                            const VkApplicationInfo&  app_info) {

  printf ("%s -> VkInstanceCreateInfo \n", __FUNCTION__); 
  
  required_extensions.clear (); 
  for (auto& xs : ut::GetRequiredExtensionNames (extstrs))
    required_extensions.push_back (xs.c_str());

  ci = {}; 
  ci.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  ci.pNext            = nullptr; 
  ci.pApplicationInfo = &app_info;

  ci.enabledExtensionCount   = required_extensions.size();
  ci.ppEnabledExtensionNames = &required_extensions[0];

  vls.clear (); 
  for (auto& vs : GetValidationLayers (vstrs)) vls.push_back (vs.c_str()); 

  if (kEnableValidationLayers && ut::CheckValidationSupport (vstrs)) {
    printf ("ENABLE VALIDATION LAYERS\n"); 
    ci.enabledLayerCount   = vls.size(); 
    ci.ppEnabledLayerNames = &vls[0]; 
    // !! SETUP up additional output cb handling...
  }
  else {
    printf ("VALIDATION LAYERS ARE DISABLED\n"); 
    ci.enabledLayerCount   = 0;
    ci.ppEnabledLayerNames = nullptr;
  }
  // 
  return ci;
}


// ------------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------------
VkDeviceQueueCreateInfo&
rokz::cx::CreateInfo (VkDeviceQueueCreateInfo& info, uint32_t que_fam_index, float* q_priority) {

  printf ("%s VkDeviceQueueCreateInfo [%u]\n", __FUNCTION__, que_fam_index);
  
  info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; 
  info.pNext = nullptr;
  info.flags            = 0; 
  info.queueFamilyIndex = que_fam_index ; // glob.queue_fams.graphics.value();

  info.queueCount       = 1; 
  info.pQueuePriorities = q_priority;

  return info; 
}

// ------------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------------
VkDeviceCreateInfo& rokz::cx::CreateInfo (VkDeviceCreateInfo& info,
                                          const void* next, 
                                          std::vector<const char*>& vls, std::vector<std::string>& vstrs,
                                          std::vector<const char*>& dxs, std::vector<std::string>& dxstrs,
                                          const std::vector<VkDeviceQueueCreateInfo>&  queuecreateinfos,
                                          const VkPhysicalDeviceFeatures* devfeats) {

  printf ("%s VkDeviceCreateInfo\n", __FUNCTION__);

  dxstrs.clear (); 
  for (auto& dx : GetDeviceExtensionNames (dxstrs)) dxs.push_back ( dx.c_str() ); 
    
  vls.clear ();
  for (auto& vl : GetValidationLayers (vstrs)) vls.push_back (vl.c_str()); 
  
  info.sType                 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  info.pNext                 = next;
  info.enabledExtensionCount = dxs.size ();  //static_cast<uint32_t> (device_extensions.size ());
  info.ppEnabledExtensionNames= &dxs[0]; 

  info.queueCreateInfoCount  = queuecreateinfos.size();   
  info.pQueueCreateInfos     = &queuecreateinfos[0]; /// &glob.create_info.queue;
  info.pEnabledFeatures      = devfeats; // &glob.device_features;
  info.enabledLayerCount     = vls.size();   
  info.ppEnabledLayerNames   = &vls[0]; 
  return info;
}
// ------------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------------
VkDeviceCreateInfo& rokz::cx::CreateInfo2 (VkDeviceCreateInfo& info,
                                          const VkPhysicalDeviceFeatures2* feat2,
                                          std::vector<const char*>& vls, std::vector<std::string>& vstrs,
                                          std::vector<const char*>& dxs, std::vector<std::string>& dxstrs,
                                          const std::vector<VkDeviceQueueCreateInfo>&  queuecreateinfos) {

  HERE("HAI"); 

  //  --> VK_KHR_swapchain
  //  --> enable extension: VK_KHR_dynamic_rendering
  //  --> enable extension: VK_EXT_extended_dynamic_state2
  dxstrs.clear (); 
  for (auto& dx : GetDeviceExtensionNames (dxstrs)) {
    printf (" --> enable extension: %s\n", dx.c_str() ); 
    dxs.push_back ( dx.c_str() );
  }
    
  vls.clear ();
  for (auto& vl : GetValidationLayers (vstrs)) { 
    printf (" --> validation layers: %s\n", vl.c_str() ); 
    vls.push_back (vl.c_str()); 
  }

  info.sType                 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  info.pNext                 = feat2;
  info.enabledExtensionCount = dxs.size ();  //static_cast<uint32_t> (device_extensions.size ());
  info.ppEnabledExtensionNames= &dxs[0]; 

  info.queueCreateInfoCount  = queuecreateinfos.size();   
  info.pQueueCreateInfos     = &queuecreateinfos[0]; /// &glob.create_info.queue;
  info.pEnabledFeatures      = VK_NULL_HANDLE; 
  info.enabledLayerCount     = vls.size();   
  info.ppEnabledLayerNames   = &vls[0]; 
  return info;
}




// -------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------
int rokz::cx::CreateInstance (VkInstance& instance, const VkInstanceCreateInfo& ci) {
  // CREATEINSTANCE
  printf (" -> %s...", __FUNCTION__); 

  if (vkCreateInstance (&ci, nullptr, &instance) != VK_SUCCESS) {
    printf("[FAILED] create instance\n");
    return __LINE__;
  }
 
  //printf("WoooooOOOooooOoooooooOOoOoOOOoooOoOOOOOOO!!!1\n");
  // ENUMERATEINSTANCEEXTENSIONPROPERTIES
  uint32_t ext_count = 0;
  vkEnumerateInstanceExtensionProperties (nullptr, &ext_count, nullptr);

  printf("[Instance Extensions] avaiable [%u] -> \n", ext_count);
  std::vector<VkExtensionProperties> extensions(ext_count);
  vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, &extensions[0]);

  for (const auto &extension : extensions) {
    printf ("  %s\n", extension.extensionName); 
  }
  
  return 0;
}

// -------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------
bool rokz::cx::CreateLogicalDevice (
    VkDevice*                 device,
    const VkDeviceCreateInfo* createinfo,
    const VkPhysicalDevice&   physdev) {

  printf ("%s", __FUNCTION__);
  
  if (VK_SUCCESS == vkCreateDevice (physdev, createinfo, nullptr, device)) {
    printf ("...SUCCESS \n"); 
    return true; 
  }

  printf ("...FAILED \n"); 
  return false; 
}

// -------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------
bool rokz::cx::CreateSurface (VkSurfaceKHR* surf, GLFWwindow* glfwin, const VkInstance& inst) {
  printf ("%s", __FUNCTION__);

  if (glfwCreateWindowSurface (inst, glfwin, nullptr, surf) != VK_SUCCESS) {
    printf ("...FAILED\n"); 
    return false; 
  }

  printf ("...SUCCESS\n"); 
  return true; 
}

// -------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------
VkSurfaceFormatKHR rokz::cx::ChooseSwapSurfaceFormat (const std::vector<VkSurfaceFormatKHR>& available_formats) {
  
  for (const auto& f : available_formats) {
    if (f.format == VK_FORMAT_B8G8R8A8_SRGB && f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      printf ("%s return [f]\n", __FUNCTION__);
      return f;
    }
  }

  printf ("%s return [0]\n", __FUNCTION__);
  return available_formats[0]; 
}

// -------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------
VkPresentModeKHR rokz::cx::ChooseSwapPresentMode (const std::vector<VkPresentModeKHR> &available_modes) {
  printf ("%s using ", __FUNCTION__);
  for (const auto& mode : available_modes) {
    if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
      printf ("VK_PRESENT_MODE_MAILBOX_KHR \n");
      return mode;
    }
  }
  
  printf ("VK_PRESENT_MODE_FIFO_KHR \n");
  return VK_PRESENT_MODE_FIFO_KHR;
}

// -------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------
VkExtent2D rokz::cx::ChooseSwapExtent (const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* win) {

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
    
    actual_extent.width  = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    
    return actual_extent;
  }
}




// -------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------
VkSwapchainCreateInfoKHR& rokz::cx::CreateInfo_default (VkSwapchainCreateInfoKHR&   ci, 
                                                        VkSurfaceKHR                surface,
                                                        const std::vector<uint32_t>&      family_indices,
                                                        const VkExtent2D&           extent, 
                                                        const SwapchainInfo& swapchain_support_info, 
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
  ci.pNext         = nullptr;
  ci.surface       = surface;
  ci.minImageCount = image_count;
  ci.imageFormat   = swap_surface_format.format;
  ci.imageColorSpace = swap_surface_format.colorSpace;
  ci.imageExtent = extent;
  ci.imageArrayLayers = 1;
  ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  
  // QueueFamilyIndices que_fam_inds; 
  // FindQueueFamilies (que_fam_inds, surface, physdev);
  // family_indices.clear (); 
  // family_indices.push_back (physdev.family_indices.graphics.value());
  // family_indices.push_back (physdev.family_indices.present.value ());
 
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

VkSwapchainCreateInfoKHR& rokz::cx::CreateInfo_default (VkSwapchainCreateInfoKHR&   ci, 
                                                        VkSurfaceKHR                surface,
                                                        const std::vector<uint32_t>&      family_indices,
                                                        const VkExtent2D&           extent, 
                                                        const SwapchainInfo& swapchain_support_info) {
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
  ci.pNext         = nullptr;
  ci.surface       = surface;
  ci.minImageCount = image_count;
  ci.imageFormat   = swap_surface_format.format;
  ci.imageColorSpace = swap_surface_format.colorSpace;
  ci.imageExtent = extent;
  ci.imageArrayLayers = 1;
  ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  
  // QueueFamilyIndices que_fam_inds; 
  // FindQueueFamilies (que_fam_inds, surface, physdev);
  // family_indices.clear (); 
  // family_indices.push_back (physdev.family_indices.graphics.value());
  // family_indices.push_back (physdev.family_indices.present.value ());

  if (family_indices.size () > 1) {
    // if > 1, assume physdev.family_indices.graphics != physdev.family_indices.present)
    printf ("[VK_SHARING_MODE_CONCURRENT]\n");
    ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    ci.queueFamilyIndexCount = family_indices.size();
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

// ------------------------------------------------------------------------------------------
//                  
// ------------------------------------------------------------------------------------------
bool rokz::cx::CreateSwapchain (VkSwapchainKHR& swapchain, const VkSwapchainCreateInfoKHR& ci, const Device& device) {

  printf (" %s ", __FUNCTION__);

  if (vkCreateSwapchainKHR (device.handle, &ci, nullptr, &swapchain) != VK_SUCCESS) {
    printf (" --> LEAVING[FALSE] %s Create Swapchain\n", __FUNCTION__);
    return false; 
  }

  printf (" --> [SUCCESS]\n");
  return true;
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
VkCommandPoolCreateInfo& rokz::cx::CreateInfo ( VkCommandPoolCreateInfo& ci, uint32_t queue_family_index) {

  ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  ci.pNext = nullptr;
  ci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  ci.queueFamilyIndex = queue_family_index;

  return ci;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::cx::CreateCommandPool (VkCommandPool& command_pool, const VkCommandPoolCreateInfo& create_info, const VkDevice& device) {

  if (vkCreateCommandPool(device, &create_info, nullptr, &command_pool) != VK_SUCCESS) {
    printf ("failed to create command pool!");
    return false; 
  }

  return true; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
// bool rokz::CreateCommandPool (VkCommandPool&            command_pool,
//                              VkCommandPoolCreateInfo&  create_info,
//                              const QueueFamilyIndices& queue_family_inds, 
//                              const VkDevice& device ) {
//   // 
//   create_info = {};
//   create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
//   create_info.pNext = nullptr;
//   create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
//   create_info.queueFamilyIndex = queue_family_inds.graphics.value ();

//   if (vkCreateCommandPool(device, &create_info, nullptr, &command_pool) != VK_SUCCESS) {
//     printf ("failed to create command pool!");
//     return false; 
//   }

//   return true; 
// }

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------

VkCommandBufferAllocateInfo& rokz::cx::AllocateInfo (VkCommandBufferAllocateInfo& alloc_info, uint32_t commandbuffers,  const VkCommandPool& commandpool) 
{
  alloc_info = {};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.pNext = nullptr;
  alloc_info.commandPool = commandpool;
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandBufferCount =  commandbuffers;
  return alloc_info;
}

VkCommandBufferAllocateInfo& rokz::cx::AllocateInfo (VkCommandBufferAllocateInfo& alloc_info, const VkCommandPool& commandpool) 
{
  // alloc_info = {};
  // alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  // alloc_info.pNext = nullptr;
  // alloc_info.commandPool = commandpool;
  // alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  // alloc_info.commandBufferCount =  commandbuffers;
  // return alloc_info;


  return AllocateInfo (alloc_info, 1,  commandpool) ; 

}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::cx::CreateCommandBuffers (std::vector<VkCommandBuffer>& commandbuffers,
                               const VkCommandBufferAllocateInfo& alloc_info, 
                               const VkDevice &device) {

  if (vkAllocateCommandBuffers(device, &alloc_info, &commandbuffers[0]) != VK_SUCCESS) {
    printf ("failed to allocate command buffers!");
    return false; 
  }
  printf ("BAI %s\n", __FUNCTION__); 
  return true; 

}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::cx::CreateCommandBuffer(VkCommandBuffer &command_buffer,
                               const VkCommandBufferAllocateInfo& alloc_info, 
                               const VkDevice &device) {

  if (vkAllocateCommandBuffers(device, &alloc_info, &command_buffer) != VK_SUCCESS) {
    printf ("failed to allocate command buffers!");
    return false; 
  }
  printf ("BAI %s\n", __FUNCTION__); 
  return true; 
}



// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
void rokz::cx::GetDeviceQueue (VkQueue* que, uint32_t fam_ind, const VkDevice& device) { 
  //  printf ("%s\n", __FUNCTION__);
  const uint32_t que_index = 0; // diffrnt than que_fam_ind
  //  printf ("%s EXIT\n", __FUNCTION__);
  return vkGetDeviceQueue (device, fam_ind, que_index, que);
}





#ifdef ROKZ_HIDE_CREATERENDERPASS
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::CreateRenderPass (VkRenderPass &render_pass,
                            VkRenderPassCreateInfo &create_info,
                            VkFormat swapchain_format, const VkDevice &device) {


  // COLOR ATTACHMENT
  VkAttachmentDescription color_attachment{};
  color_attachment.format = swapchain_format ;
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  // 
  VkAttachmentReference color_attachment_ref{};
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  // SUBPASS 
  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_ref;

  subpass.inputAttachmentCount = 0;
  subpass.pInputAttachments = nullptr;

  subpass.pDepthStencilAttachment = nullptr;

  subpass.preserveAttachmentCount = 0;
  subpass.pPreserveAttachments = nullptr;

  subpass.pResolveAttachments = nullptr;
  subpass.flags = 0 ; 

  // CREATEINFO. gets passed back out
  create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  create_info.attachmentCount = 1;
  create_info.pAttachments = &color_attachment;
  create_info.subpassCount = 1;
  create_info.pSubpasses = &subpass;

  if (vkCreateRenderPass(device, &create_info, nullptr, &render_pass) != VK_SUCCESS) {
    printf ("failed to create render pass!\n");
    return false; 
  }

  return true;
}
#endif


#ifdef ROKZ_HIDE_CLEANUPSWAPCHAIN
// ---------------------------------------------------------------------
// nu
// ---------------------------------------------------------------------
void rokz::CleanupSwapchain (std::vector<Framebuffer>&   framebuffers,
                             std::vector<ImageView>&     fb_image_views,
                             rokz::Image&                msaa_color_image,
                             rokz::ImageView&            msaa_color_imageview,

                             rokz::Image&                depth_image,
                             rokz::ImageView&            depth_imageview,

                             rokz::Swapchain&            swapchain,
                             const rokz::Device&         device) {

  for (auto fb : framebuffers) {
    vkDestroyFramebuffer (device.handle, fb.handle, nullptr); 
  }

  for (auto fb_imageview : fb_image_views) {
    vkDestroyImageView(device.handle, fb_imageview.handle, nullptr);
  }

  rokz::cx::Destroy (msaa_color_image, device.allocator.handle);
  rokz::cx::Destroy (msaa_color_imageview, device.handle);

  rokz::cx::Destroy (depth_image, device.allocator.handle);
  rokz::cx::Destroy (depth_imageview, device.handle);

  vkDestroySwapchainKHR(device.handle, swapchain.handle, nullptr);
}
#endif



// -----------------------------------------------------------------------------------------------
//                              
// -----------------------------------------------------------------------------------------------
void Destroy (rokz::FrameSyncGroup& fsg, const rokz::Device& device) {
  

  for (size_t i = 0; i < fsg.syncs.size (); ++i) {
    vkDestroySemaphore(device.handle, fsg.syncs[i].image_available_sem, nullptr);
    vkDestroySemaphore(device.handle, fsg.syncs[i].render_finished_sem, nullptr);
    vkDestroyFence (device.handle, fsg.syncs[i].in_flight_fence, nullptr);
  }


}


// ---------------------------------------------------------------------
// DESTROY ALL THE THINGS
// ---------------------------------------------------------------------
void rokz::Cleanup (VkPipeline&                      pipeline,
                    std::vector<Framebuffer>&        framebuffers,
                    rokz::RenderPass&                render_pass,
                    VkSurfaceKHR&                    surf,
                    VkCommandPool&                   command_pool,
                    std::vector<rokz::FrameSync>&    fsyncs, 
                    std::vector<rokz::ShaderModule>& shader_modules,
                    VkPipelineLayout&                pipeline_layout,
                    GLFWwindow*                      w,
                    rokz::Device&                    device,
                    VkInstance&                      inst) {

  // vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
  // CleanupSwapchain (framebuffers, 
  //                    imageviews,

  //                    msaa_color_image,
  //                    msaa_color_imageview,

  //                    depth_image, 
  //                    depth_imageview,

  //                    swapchain,
  //                    device, 
  //                    allocator
  //                    );

  for (auto fb : framebuffers) { 
    vkDestroyFramebuffer (device.handle, fb.handle, nullptr);
  }

  vkDestroyPipeline (device.handle, pipeline, nullptr);
  vkDestroySurfaceKHR (inst, surf, nullptr);
  vkDestroyCommandPool (device.handle, command_pool, nullptr);

  for (size_t i = 0; i < fsyncs.size (); ++i) {
    vkDestroySemaphore(device.handle, fsyncs[i].image_available_sem, nullptr);
    vkDestroySemaphore(device.handle, fsyncs[i].render_finished_sem, nullptr);
    vkDestroyFence (device.handle, fsyncs[i].in_flight_fence, nullptr);
  }

  for (auto shmod : shader_modules) {
    vkDestroyShaderModule (device.handle, shmod.handle, nullptr); 
  }
  vkDestroyPipelineLayout (device.handle, pipeline_layout, nullptr);
  vkDestroyRenderPass (device.handle, render_pass.handle, nullptr); 

  vmaDestroyAllocator(device.allocator.handle);
  vkDestroyDevice    (device.handle, nullptr); 
  vkDestroyInstance  (inst, nullptr);
  glfwDestroyWindow(w);
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::cx::CheckDeviceExtensionSupport (const VkPhysicalDevice& physdev) {
  HERE("hai");

  uint32_t exts_count;
  vkEnumerateDeviceExtensionProperties(physdev, nullptr, &exts_count, nullptr);

  std::vector<VkExtensionProperties> available_exts(exts_count);
  vkEnumerateDeviceExtensionProperties(physdev, nullptr, &exts_count, &available_exts[0]);

  
  std::set<std::string> req_exts ( &kDeviceExtensions[0], &kDeviceExtensions[kDeviceExtensions.size()] );

  // try {
  
  printf ("looking for device extensions:\n"); 
  // for (const auto& e : req_exts) {
  //   printf ("  .. %s\n", e.c_str ()); 
  // }
  
  for (const auto &extension : available_exts) {
    
    if (req_exts.count (extension.extensionName))
      printf ( "> %s <-  available \n", extension.extensionName); 
    else
      printf ( " %s\n", extension.extensionName); 

      
    req_exts.erase(extension.extensionName);
  }
    
  // printf ("LEAVING %s (empty:%s)\n", __FUNCTION__, req_exts.empty()? "TRUE" : "FALSE");
  HERE("bai");
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
rokz::QueueFamilyIndices& rokz::cx::FindQueueFamilies (rokz::QueueFamilyIndices& inds, const VkSurfaceKHR& surf, const VkPhysicalDevice& physdev) {
  printf ("%s\n", __FUNCTION__); 

  uint32_t qcount  = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physdev, &qcount, nullptr);

  std::vector<VkQueueFamilyProperties> qfam_props (qcount);

  vkGetPhysicalDeviceQueueFamilyProperties (physdev, &qcount, &qfam_props[0]);

  for (uint32_t ique = 0; ique < qcount; ++ique) {

    rokz::QueueFamilyIndices cur_inds;
    VkBool32 present_support = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR (physdev, ique, surf, &present_support);
      
    if (present_support) {
      inds.present = ique;
      assert (inds.present.has_value ());
      printf ("found PRESENT queue;\n"); 
    }
      
    if (qfam_props[ique].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
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
bool IsDeviceSuitable (rokz::QueueFamilyIndices& outind,
                       VkPhysicalDeviceFeatures2&  features2, 
                       VkPhysicalDeviceProperties& devprops,
                       const VkSurfaceKHR& surf,
                       const VkPhysicalDevice& physdev) {

  using namespace rokz;
  printf ("%s\n", __FUNCTION__); 


  bool swapchain_adequate = false;
  bool exts_supported = rokz::cx::CheckDeviceExtensionSupport (physdev);

  // if (exts_supported) {
  //   printf ("[exts_supported]\n"); 
  // }
  
  rokz::SwapchainInfo swapchain_supp_info {};
  swapchain_supp_info.capabilities = {};
  swapchain_supp_info.formats = {};
  swapchain_supp_info.present_modes = {};

  bool all_queues_available = 
    AllQueuesAvailable (rokz::cx::FindQueueFamilies (outind, surf, physdev)); 
  
  if (all_queues_available) {
    printf ("ALL_QUEUES_AVAILABLE\n"); 
  }
  
  if (exts_supported) {
    rokz::cx::QuerySwapchainSupport(swapchain_supp_info, surf, physdev);
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


  ut::PrintPhysicalDeviceLimits (devprops.limits);

  // FORCE ANISTOTROPY
  //VkPhysicalDeviceFeatures devfeatures {};
  //
  //devfeatures.samplerAnisotropy = VK_TRUE;
  vkGetPhysicalDeviceFeatures2 (physdev, &features2);
  //vkGetPhysicalDeviceFeatures (physdev, &features2.features);

  // if (VK_TRUE == devfeatures.tessellationShader)
  //   HERE("features 1 tesslation_available:YES"); 

  // //VK_EXT_extended_dynamic_state2;

  //vkGetPhysicalDeviceFeatures2 (physdev, &features2);
  if (VK_TRUE == features2.features.tessellationShader)
    HERE("SUPPORTED | TESSELLATION_SHADER :YES"); 

  // bool device_is_suitable =
  //   devprops.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
  //   && devfeatures.geometryShader;
  if (features2.features.samplerAnisotropy) {
    HERE("SUPPORTED | ANISOTROPIC_SAMPLING\n"); 
  }

  return all_queues_available
    && exts_supported
    && swapchain_adequate
    && features2.features.samplerAnisotropy; 
}

  
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------

//  rokz::SelectPhysicalDevice (glob.physical_device, glob.surface, glob.instance);
bool rokz::cx::SelectPhysicalDevice (PhysicalDevice& physdev, const VkSurfaceKHR& surf, const VkInstance& inst) {

  using namespace rokz;
  //
  printf ("%s\n", __FUNCTION__); 

  uint32_t device_count = 0;
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
    if (IsDeviceSuitable (curqueinds,  physdev.features2, physdev.properties, surf, physdevs[idev])) {
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




// -------------------------------------------------------------------------------------------
// uses rokz::cx::CreateInstance (VkInstance& instance,...
// need to expose appinfo?
// -------------------------------------------------------------------------------------------
bool rokz::InitializeInstance (rokz::Instance& instance) {
  rokz::cx::AppInfo_default (instance.app_info);

  rokz::cx::CreateInfo (instance.ci,
                        instance.required_extensions,
                        instance.extension_strings,
                        instance.vals, instance.validation_strings, 
                        instance.app_info); 

  return rokz::cx::CreateInstance (instance.handle, instance.ci);
}

// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
bool rokz::InitializeDevice (rokz::Device&                    device,
                             const VkPhysicalDeviceFeatures2& features2, 
                             const rokz::PhysicalDevice&      physical_device,
                             const rokz::Instance&            instance) {

  device.priority.graphics = 1.0f;
  device.priority.present  = 1.0f;

  device.queue_ci.resize  (2);

  uint32 graphicsq = ~0;
  uint32 presentq  = ~0;
  if (!physical_device.family_indices.graphics || !physical_device.family_indices.present) {
    HERE("all commands queues needed");
    return false;
  }
  graphicsq = physical_device.family_indices.graphics.value ();
  presentq  = physical_device.family_indices.present.value  ();


  rokz::cx::CreateInfo (device.queue_ci[0],  graphicsq, &device.priority.graphics);
  rokz::cx::CreateInfo (device.queue_ci[1],  presentq , &device.priority.present);

  // device info
  //VkDeviceCreateInfo&       Default (VkDeviceCreateInfo& info, VkDeviceQueueCreateInfo* quecreateinfo, VkPhysicalDeviceFeatures* devfeats); 

  // * 01/15/2023 - use dynamic rendering pass 
  VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamic_rendering_feature {};
  dynamic_rendering_feature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
  dynamic_rendering_feature.pNext = nullptr;
  dynamic_rendering_feature.dynamicRendering = VK_TRUE;

  // * 03/29/2023 VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME
  //VkPhysicalDeviceFeatures2 features2 {};
  //VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT  

  

  rokz::cx::CreateInfo (device.ci, &dynamic_rendering_feature, 
                        device.vals, device.valstrs, device.dxs, device.dxstrs, 
                        device.queue_ci, &features2.features);

  rokz::cx::CreateLogicalDevice (&device.handle, &device.ci, physical_device.handle); 

  rokz::cx::CreateInfo             (device.command_pool.ci, physical_device.family_indices.graphics.value());
  rokz::cx::CreateCommandPool      (device.command_pool.handle, device.command_pool.ci, device.handle);

  // get queue handle
  rokz::cx::GetDeviceQueue (&device.queues.graphics, graphicsq, device.handle);
  rokz::cx::GetDeviceQueue (&device.queues.present,  presentq , device.handle);

  // VMA SECTION
  // VmaVulkanFunctions vulkanFunctions = {};
  // vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
  // vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

  // VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA 
  rokz::cx::CreateInfo (device.allocator.ci, instance, device, physical_device); 
  vmaCreateAllocator(&device.allocator.ci, &device.allocator.handle);

  printf ("%s BAI", __FUNCTION__);
  return true;

}

// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
bool rokz::InitializeDevice2 (rokz::Device&                      device,
                             const VkPhysicalDeviceFeatures2& features2, 
                             const rokz::PhysicalDevice&      physical_device,
                             const rokz::Instance&            instance) {
  using namespace rokz;
  device.priority.graphics = 1.0f;
  device.priority.present  = 1.0f;
  device.queue_ci.resize  (2);
  uint32 graphicsq = ~0;
  uint32 presentq  = ~0;
  if (!physical_device.family_indices.graphics || !physical_device.family_indices.present) {
    HERE("all commands queues needed");
    return false;
  }
  graphicsq = physical_device.family_indices.graphics.value ();
  presentq  = physical_device.family_indices.present.value  ();
  rokz::cx::CreateInfo (device.queue_ci[0],  graphicsq, &device.priority.graphics);
  rokz::cx::CreateInfo (device.queue_ci[1],  presentq , &device.priority.present);

  // device info
  //VkDeviceCreateInfo&       Default (VkDeviceCreateInfo& info, VkDeviceQueueCreateInfo* quecreateinfo, VkPhysicalDeviceFeatures* devfeats); 
  // * 01/15/2023 - use dynamic rendering pass 
  // VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamic_rendering_feature {};
  // dynamic_rendering_feature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
  // dynamic_rendering_feature.pNext = nullptr;
  // dynamic_rendering_feature.dynamicRendering = VK_TRUE;

  // * 03/29/2023 VK_EXT_EXTENDED_DYNAMIC_STATE_2_EXTENSION_NAME
  //VkPhysicalDeviceFeatures2 features2 {};
  //VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT  
  rokz::cx::CreateInfo2 (device.ci, &features2, 
                        device.vals, device.valstrs, device.dxs, device.dxstrs, device.queue_ci);

  rokz::cx::CreateLogicalDevice (&device.handle, &device.ci, physical_device.handle); 

  rokz::cx::CreateInfo             (device.command_pool.ci, physical_device.family_indices.graphics.value());
  rokz::cx::CreateCommandPool      (device.command_pool.handle, device.command_pool.ci, device.handle);
  // get queue handle
  rokz::cx::GetDeviceQueue (&device.queues.graphics, graphicsq, device.handle);
  rokz::cx::GetDeviceQueue (&device.queues.present,  presentq , device.handle);
  // VMA SECTION
  // VmaVulkanFunctions vulkanFunctions = {};
  // vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
  // vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;
  // VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA VMA 
  rokz::cx::CreateInfo (device.allocator.ci, instance, device, physical_device); 
  vmaCreateAllocator(&device.allocator.ci, &device.allocator.handle);

  printf ("%s BAI", __FUNCTION__);
  return true;
}
// -------------------------------------------------------------------------------------------
//                   
// -------------------------------------------------------------------------------------------
VkPhysicalDeviceFeatures2& rokz::ConfigureFeatures (VkPhysicalDeviceFeatures2& features2, const rokz::PhysicalDevice& physical_device) {

  HERE("this is all botched");

  features2.sType = physical_device.features2.sType;
  features2.pNext = physical_device.features2.pNext;

  features2.features.samplerAnisotropy  =  physical_device.features2.features.samplerAnisotropy  ;
  features2.features.tessellationShader =  physical_device.features2.features.tessellationShader ;
  // VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures separate_depth_stencil_layout_feature {}; 
  // separate_depth_stencil_layout_feature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES;
  // separate_depth_stencil_layout_feature.pNext = VK_NULL_HANDLE;
  // separate_depth_stencil_layout_feature.separateDepthStencilLayouts = VK_TRUE;

  // features2.pNext = &separate_depth_stencil_layout_feature;
  

  //separateDepthStencilLayouts

  
  return features2;
}

// ---^----------------------------------------------------------------------------------------
//    +-these r rufly the same function               
// ---v---------------------------------------------------------------------------------------
rokz::PhysicalDevice& rokz::ConfigureDevice (rokz::PhysicalDevice& physical_device, VkBool32 sampler_anisotropy) {
  HERE("this rly needs to be revised");
  // VkDeviceSize min_uniform_buffer_offset_alignment =
  //   rokz::ut::MinUniformBufferOffsetAlignment (physical_device);
  physical_device.features2.features.samplerAnisotropy  = sampler_anisotropy;
  physical_device.features2.features.tessellationShader = VK_TRUE;
  //VK_EXT_extended_dynamic_state2
  return physical_device; 
}

