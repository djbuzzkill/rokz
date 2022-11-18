
#ifndef ROKZ_FUNCS_INCLUDE
#define ROKZ_FUNCS_INCLUDE


#include "common.h"


namespace rokz {

  bool
  Initialize (Glob& glob);

  // DEFAULTS
  VkSwapchainCreateInfoKHR&
  Default (VkSwapchainCreateInfoKHR& info, const VkSurfaceKHR& surf); 

  Glob& 
  Default (Glob& g); 
  
  VkDeviceQueueCreateInfo& 
  Default (VkDeviceQueueCreateInfo& info, uint32_t que_fam_index, float* q_priority);

  VkDeviceCreateInfo&
  Default (VkDeviceCreateInfo& info,
           VkDeviceQueueCreateInfo* quecreateinfo,
           VkPhysicalDeviceFeatures* devfeats); 
  
  QueueFamilyIndices&
  FindQueueFamilies (QueueFamilyIndices& queue_fams,
                     const VkSurfaceKHR& surf,
                     const VkPhysicalDevice& physdev);

  SwapchainSupportInfo&
  QuerySwapchainSupport (SwapchainSupportInfo& deets,
                         const VkSurfaceKHR& surf,
                         const VkPhysicalDevice& dev); 
  
  GLFWwindow*
  CreateWindow_glfw (GLFWwindow*& w);

  bool
  CheckValidationSupport (const std::vector<const char*>& validation_layers);

  int
  CreateInstance (Glob& glob);
  
  bool
  SelectPhysicalDevice (VkPhysicalDevice& physdev,
                        QueueFamilyIndices& queueind,
                        const VkSurfaceKHR& surf,
                        const VkInstance& inst);

  bool
  CreateLogicalDevice (VkDevice*                 device,
                       const VkDeviceCreateInfo* createinfo,
                       const VkPhysicalDevice&   physdev);

  void
  GetDeviceQueue (VkQueue* que, uint32_t fam_ind, const VkDevice& device); 

  void
  Cleanup (VkSwapchainKHR& swapchain,
           VkSurfaceKHR& surf,
           std::vector<VkImageView>& image_views, 
           GLFWwindow* w,
           VkDevice& dev,
           VkInstance &inst);

  VkSurfaceFormatKHR
  ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);

  VkPresentModeKHR
  ChooseSwapPresentMode  (const std::vector<VkPresentModeKHR>& available_modes);

  VkExtent2D
  ChooseSwapExtent       (const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* win);
  
  // TODO
  bool
  CreateSurface   (VkSurfaceKHR* surf, GLFWwindow* glfwin , const VkInstance& inst);

  bool
  CreateSwapchain (VkSwapchainKHR&         swapchain, 
                        VkSwapchainCreateInfoKHR& swapchaincreateinfo,
                        const VkSurfaceKHR&     surf,
                        const VkPhysicalDevice& physdev, 
                        const VkDevice&         dev, 
                        GLFWwindow*             glfwin);

  bool
  GetSwapChainImages (std::vector<VkImage> &swapchain_images,
                           VkSwapchainKHR& swapchain,
                           const VkDevice& dev);

  bool CreateImageViews(std::vector<VkImageView> &swapchain_imageviews,
                        const std::vector<VkImage> &swapchain_images,
                        VkFormat                    surf_fmt, 
                        const VkDevice&             dev); 


  void
  CreateRenderPass ();

  bool
  CreatePipeline   (); 

  bool
  CreateFrameBuffers ();

  bool
  CreateCommandPool (); 
  
  bool
  CreateCommandBuffer ();

  bool
  CreateSyncObjects ();

} // namespace rokz

#endif
