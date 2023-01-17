
#include "context.h"

#include "window.h"
#include "buffer.h"
#include "image.h"
#include "pipeline.h"
#include "framebuffer.h"
#include "rokz/shared_types.h"
#include "shader.h"
#include "window.h"
#include "descriptor.h"
#include <vulkan/vulkan_core.h>


const bool kEnableValidationLayers = true;
  //#endif


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
const std::vector<const char*> kDeviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME, 
  VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
  //"VK_EXT_descriptor_indexing", 

};


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

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
rokz::SwapchainSupportInfo& rokz::cx::QuerySwapchainSupport (rokz::SwapchainSupportInfo& si, VkSurfaceKHR surf, VkPhysicalDevice device) {

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
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surf, &present_mode_count, &si.present_modes[0]);
  }

  return si;
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------

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

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------

VkInstanceCreateInfo& rokz::cx::CreateInfo (VkInstanceCreateInfo& ci,
                                        std::vector<const char*>& required_extensions,
                                        std::vector<std::string>& extstrs,
                                        std::vector<const char*>& vls,
                                        std::vector<std::string>& vstrs,
                                        const VkApplicationInfo&  app_info) {

  printf ("%s VkInstanceCreateInfo \n", __FUNCTION__); 
  
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


//
//
//
int rokz::cx::CreateInstance (VkInstance& instance, const VkInstanceCreateInfo& ci) {
  // CREATEINSTANCE
  printf (" -> %s...", __FUNCTION__); 

  if (vkCreateInstance (&ci, nullptr, &instance) != VK_SUCCESS) {
    printf("[FAILED] create instance\n");
    return __LINE__;
  }
 
  printf("WoooooOOOooooOoooooooOOoOoOOOoooOoOOOOOOO!!!1\n");
  // ENUMERATEINSTANCEEXTENSIONPROPERTIES
  uint32_t ext_count = 0;
  vkEnumerateInstanceExtensionProperties (nullptr, &ext_count, nullptr);

  printf("Number of Available Extensions[%u]\n", ext_count);
  std::vector<VkExtensionProperties> extensions(ext_count);
  vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, &extensions[0]);

  for (const auto &extension : extensions) {
    printf ("  %s\n", extension.extensionName); 
  }
  
  return 0;
}

// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
VkDeviceQueueCreateInfo& rokz::cx::CreateInfo (VkDeviceQueueCreateInfo& info,
                                           uint32_t que_fam_index,
                                           float* q_priority){

  printf ("%s VkDeviceQueueCreateInfo [%u]\n", __FUNCTION__, que_fam_index);
  
  info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; 
  info.pNext = nullptr;
  info.flags            = 0; 
  info.queueFamilyIndex = que_fam_index ; // glob.queue_fams.graphics.value();

  info.queueCount       = 1; 
  info.pQueuePriorities = q_priority;

  return info; 
}

// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
VkDeviceCreateInfo& rokz::cx::CreateInfo (VkDeviceCreateInfo&       info,
                                      std::vector<const char*>& vls, std::vector<std::string>& vstrs,
                                      std::vector<const char*>& dxs, std::vector<std::string>& dxstrs,
                                      const std::vector<VkDeviceQueueCreateInfo>&  queuecreateinfos,
                                      VkPhysicalDeviceFeatures* devfeats) {

  printf ("%s VkDeviceCreateInfo\n", __FUNCTION__);

  dxstrs.clear (); 
  for (auto& dx : GetDeviceExtensionNames (dxstrs)) dxs.push_back ( dx.c_str() ); 
    
  vls.clear ();
  for (auto& vl : GetValidationLayers (vstrs)) vls.push_back (vl.c_str()); 
  
  info.sType                 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  info.pNext                 = nullptr;
  info.enabledExtensionCount = dxs.size ();  //static_cast<uint32_t> (device_extensions.size ());
  info.ppEnabledExtensionNames= &dxs[0]; 

  info.queueCreateInfoCount  = queuecreateinfos.size();   
  info.pQueueCreateInfos     = &queuecreateinfos[0]; /// &glob.create_info.queue;
  info.pEnabledFeatures      = devfeats; // &glob.device_features;
  info.enabledLayerCount     = vls.size();   
  info.ppEnabledLayerNames   = &vls[0]; 
  return info;
}


VkDeviceCreateInfo& rokz::cx::CreateInfo (VkDeviceCreateInfo&       info,
                                          const void* next, 
                                          std::vector<const char*>& vls, std::vector<std::string>& vstrs,
                                          std::vector<const char*>& dxs, std::vector<std::string>& dxstrs,
                                          const std::vector<VkDeviceQueueCreateInfo>&  queuecreateinfos,
                                          VkPhysicalDeviceFeatures* devfeats) {


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


// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
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

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::cx::CreateSurface (VkSurfaceKHR* surf, GLFWwindow* glfwin, const VkInstance& inst) {
  printf ("%s", __FUNCTION__);

  if (glfwCreateWindowSurface (inst, glfwin, nullptr, surf) != VK_SUCCESS) {
    printf ("...FAILED\n"); 
    return false; 
  }

  printf ("...SUCCESS\n"); 
  return true; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
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

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
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

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
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




// ---------------------------------------------------------------------
// nu
// ---------------------------------------------------------------------
VkSwapchainCreateInfoKHR& rokz::cx::CreateInfo_default (VkSwapchainCreateInfoKHR&   ci, 
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

// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
bool rokz::cx::CreateSwapchain (Swapchain& swapchain, const Device& device) {

  printf (" %s ", __FUNCTION__);

  if (vkCreateSwapchainKHR (device.handle, &swapchain.ci, nullptr, &swapchain.handle) != VK_SUCCESS) {
    printf (" --> LEAVING[FALSE] %s Create Swapchain\n", __FUNCTION__);
    return false; 
  }

  printf (" --> [SUCCESS]\n");
  return true;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::cx::GetSwapChainImages (std::vector<Image> &swapchain_images,
                               const Swapchain& swapchain,
                               const VkDevice& dev) {
  printf ("%s\n", __FUNCTION__);

  uint32_t image_count = 0; 

  VkResult res;
  res = vkGetSwapchainImagesKHR(dev, swapchain.handle, &image_count, nullptr);
  if (res != VK_SUCCESS) {
    printf ("LEAVING[FALSE] after image_count %s\n", __FUNCTION__);
    return false;
  }
  printf ( "no. swapchain images[%u]\n", image_count); 
  
  std::vector<VkImage> vk_images(image_count);
  res = vkGetSwapchainImagesKHR (dev, swapchain.handle, &image_count, &vk_images[0]);
  if (res != VK_SUCCESS) {
    printf ("LEAVING[FALSE] after swapchain images %s\n", __FUNCTION__);
    return false;
  }

  swapchain_images.resize(image_count);
  for (size_t i = 0; i < image_count; ++i) {
    // Createinfo for image from swapchain
    rokz::cx::CreateInfo (swapchain_images[i].ci, swapchain.ci); 
    swapchain_images[i].handle = vk_images[i];
  }
  
  printf ("LEAVING[TRUE] %s\n", __FUNCTION__);
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
bool rokz::cx::CreateCommandPool (VkCommandPool& command_pool,const VkCommandPoolCreateInfo& create_info, const VkDevice& device) {

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
bool rokz::cx::CreateRenderSync (RenderSync&      sync,
                             RenderSyncCreateInfo& create_info,
                             const VkDevice& device) {

  printf ("%s\n", __FUNCTION__);
  
  create_info.semaphore = {};
  create_info.semaphore.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  create_info.semaphore.pNext = nullptr;
  
  create_info.fence ={}; 
  create_info.fence.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  create_info.fence.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  create_info.fence.pNext = nullptr;
  if (vkCreateSemaphore(device, &create_info.semaphore, nullptr, &sync.image_available_sem) != VK_SUCCESS
      || vkCreateSemaphore(device, &create_info.semaphore, nullptr, &sync.render_finished_sem) != VK_SUCCESS
      || vkCreateFence(device, &create_info.fence, nullptr, &sync.in_flight_fen) != VK_SUCCESS)
    {
      printf ("[FAILED]--> create sync objects\n");
      return false; 
    }
  
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

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::RecreateSwapchain(Swapchain&                             swapchain,
                             std::vector<Image>&                    swapchain_images,

                             std::vector<Framebuffer>&              framebuffers,
                             std::vector<ImageView>&                imageviews,

                             // std::vector<VkFramebuffer>&              framebuffers,
                             // std::vector<VkFramebufferCreateInfo>&    create_infos,

                             RenderPass&                              render_pass,
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

  CleanupSwapchain (framebuffers, imageviews,
                    depth_image, depth_imageview,
                    multisamp_color_image,
                    multisamp_color_imageview,
                    swapchain, device, allocator);

  //CreateInfo_default (swapchain.ci, surf, extent, swapchain_support_info, 
  bool swapchain_res    = rokz::cx::CreateSwapchain (swapchain, device);
  bool imageviews_res   = rokz::cx::CreateImageViews (imageviews, swapchain_images, device);
  bool framebuffers_res = rokz::CreateFramebuffers (framebuffers,
                                              imageviews,
                                              render_pass,
                                              swapchain.ci.imageExtent,
                                              multisamp_color_imageview.handle,
                                              depth_imageview.handle,
                                              device);

  return (swapchain_res && imageviews_res && framebuffers_res); 
}



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
                             const rokz::Device&         device,
                             const VmaAllocator&         allocator) {

  for (auto fb : framebuffers) {
    vkDestroyFramebuffer (device.handle, fb.handle, nullptr); 
  }

  for (auto fb_imageview : fb_image_views) {
    vkDestroyImageView(device.handle, fb_imageview.handle, nullptr);
  }

  rokz::cx::Destroy (msaa_color_image, allocator);
  rokz::cx::Destroy (msaa_color_imageview, device.handle);

  rokz::cx::Destroy (depth_image, allocator);
  rokz::cx::Destroy (depth_imageview, device.handle);

  vkDestroySwapchainKHR(device.handle, swapchain.handle, nullptr);
}




void Destroy (rokz::FrameSequencing& fs, const rokz::Device& device) {
  

  for (size_t i = 0; i < fs.syncs.size (); ++i) {
    vkDestroySemaphore(device.handle, fs.syncs[i].image_available_sem, nullptr);
    vkDestroySemaphore(device.handle, fs.syncs[i].render_finished_sem, nullptr);
    vkDestroyFence (device.handle, fs.syncs[i].in_flight_fen, nullptr);
  }


}


// ---------------------------------------------------------------------
// DESTROY ALL THE THINGS
// ---------------------------------------------------------------------
void rokz::Cleanup (VkPipeline&                 pipeline,
              std::vector<Framebuffer>&         framebuffers,
              std::vector<ImageView>&           imageviews,

              rokz::Swapchain&                  swapchain,
              VkSurfaceKHR&                     surf,
              VkCommandPool&                    command_pool,
              std::vector<rokz::RenderSync>&     syncs, 
              std::vector<rokz::ShaderModule>&  shader_modules,
              VkPipelineLayout&                 pipeline_layout,
              rokz::RenderPass&                 render_pass,

                   rokz::Image&                      msaa_color_image,
              rokz::ImageView&                  msaa_color_imageview,

              rokz::Image&                      depth_image,
              rokz::ImageView&                  depth_imageview,

              GLFWwindow*                       w,
              rokz::Device&                     device,
              VmaAllocator&                     allocator, 
              VkInstance&                       inst) {

  //    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
  CleanupSwapchain (framebuffers, 
                     imageviews,

                     msaa_color_image,
                     msaa_color_imageview,

                     depth_image, 
                     depth_imageview,

                     swapchain,
                     device, 
                     allocator
                     );
   
  vkDestroyPipeline (device.handle, pipeline, nullptr);
  vkDestroySurfaceKHR (inst, surf, nullptr);
  vkDestroyCommandPool (device.handle, command_pool, nullptr);

  for (size_t i = 0; i < syncs.size (); ++i) {
    vkDestroySemaphore(device.handle, syncs[i].image_available_sem, nullptr);
    vkDestroySemaphore(device.handle, syncs[i].render_finished_sem, nullptr);
    vkDestroyFence (device.handle, syncs[i].in_flight_fen, nullptr);
  }

  for (auto shmod : shader_modules) {
    vkDestroyShaderModule (device.handle, shmod.handle, nullptr); 
  }
  vkDestroyPipelineLayout (device.handle, pipeline_layout, nullptr);
  vkDestroyRenderPass (device.handle, render_pass.handle, nullptr); 

  vmaDestroyAllocator(allocator);
  vkDestroyDevice    (device.handle, nullptr); 
  vkDestroyInstance  (inst, nullptr);
  glfwDestroyWindow(w);
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bool rokz::cx::CheckDeviceExtensionSupport (const VkPhysicalDevice& physdev) {
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
bool IsDeviceSuitable (rokz::QueueFamilyIndices& outind, VkPhysicalDeviceProperties& devprops, const VkSurfaceKHR& surf, const VkPhysicalDevice& physdev) {

  using namespace rokz;
  printf ("%s\n", __FUNCTION__); 


  bool swapchain_adequate = false;
  bool exts_supported = rokz::cx::CheckDeviceExtensionSupport (physdev);

  if (exts_supported) {
    printf ("[exts_supported]\n"); 
  }
  
  rokz::SwapchainSupportInfo swapchain_supp_info {};
  swapchain_supp_info.capabilities = {};
  swapchain_supp_info.formats = {};
  swapchain_supp_info.present_modes = {};

  bool all_queues_available = 
    AllQueuesAvailable (rokz::cx::FindQueueFamilies (outind, surf, physdev)); 
  
  if (all_queues_available) {
    printf ("[all_queues_available]\n"); 
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

  // FORCE ANISTOTROPY
  VkPhysicalDeviceFeatures devfeatures {};
  //
  devfeatures.samplerAnisotropy = VK_TRUE;
  vkGetPhysicalDeviceFeatures  (physdev, &devfeatures);
  // bool device_is_suitable =
  //   devprops.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
  //   && devfeatures.geometryShader;

  if (devfeatures.samplerAnisotropy) {
    printf ("[SUPPORTED] -> Anisotropic Sampling\n"); 
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


