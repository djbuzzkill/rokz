
#ifndef ROKZ_CONTEXT_H
#define ROKZ_CONTEXT_H


#include "common.h"
#include "rokz_types.h"

#include "defaults.h"


//namespace rokz::inner; 

namespace rokz {

  namespace cx { 
    // move everything so far into core

    QueueFamilyIndices&   FindQueueFamilies (QueueFamilyIndices& queue_fams, const VkSurfaceKHR& surf, const VkPhysicalDevice& physdev);
  

  
    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    SwapchainSupportInfo& QuerySwapchainSupport (rokz::SwapchainSupportInfo& si,
                                                 VkSurfaceKHR surface,
                                                 VkPhysicalDevice device); 



    // ----------------------------------------------------------------------
    //
    // ----------------------------------------------------------------------
    bool SelectPhysicalDevice (PhysicalDevice& physdev, const VkSurfaceKHR& surf, const VkInstance& inst); 

    VkSwapchainCreateInfoKHR& CreateInfo_default (VkSwapchainCreateInfoKHR&    ci, 
                                                  VkSurfaceKHR                 surface,
                                                  const std::vector<uint32_t>&       family_indices, 
                                                  const VkExtent2D&            extent, 
                                                  const SwapchainSupportInfo&  swapchain_support_info, 
                                                  const PhysicalDevice&        physdev); 
    // like above but no phys dev
    VkSwapchainCreateInfoKHR& CreateInfo_default (VkSwapchainCreateInfoKHR&   ci, 
                                                        VkSurfaceKHR                surface,
                                                        const std::vector<uint32_t>&      family_indices,
                                                        const VkExtent2D&           extent, 
                                                        const SwapchainSupportInfo& swapchain_support_info);

  
    VkDeviceQueueCreateInfo&  CreateInfo (VkDeviceQueueCreateInfo& info, uint32_t que_fam_index, float* q_priority);
    
    VkDeviceCreateInfo&       CreateInfo (VkDeviceCreateInfo& info,
                                          std::vector<const char*>& vls, 
                                          std::vector<std::string>& vstrs, 

                                          std::vector<const char*>& dxs,
                                          std::vector<std::string>& dxstrs, 

                                          const std::vector<VkDeviceQueueCreateInfo>& quecreateinfo,
                                          const VkPhysicalDeviceFeatures* devfeats); 


    VkDeviceCreateInfo&       CreateInfo (VkDeviceCreateInfo&       info,
                                          const void*               next, 
                                          std::vector<const char*>& vls, 
                                          std::vector<std::string>& vstrs, 

                                          std::vector<const char*>& dxs,
                                          std::vector<std::string>& dxstrs, 

                                          const std::vector<VkDeviceQueueCreateInfo>& quecreateinfo,
                                          const VkPhysicalDeviceFeatures* devfeats); 
    // ------------------------------------------------------------------
    //
    // ------------------------------------------------------------------
    VkApplicationInfo&    AppInfo_default (VkApplicationInfo& app_info); 
    VkInstanceCreateInfo& CreateInfo      (VkInstanceCreateInfo& ci,
                                           std::vector<const char*>& required_extensions, 
                                           std::vector<std::string>& extstrs, 
                                           std::vector<const char*>& vls, 
                                           std::vector<std::string>& vstrs, 
                                           const VkApplicationInfo&  app_info); 

    int                   CreateInstance (VkInstance& instance, const VkInstanceCreateInfo& ci); 
    // -------------------------------------------------------------------------------------------
    //                                             
    // -------------------------------------------------------------------------------------------
    bool               CreateLogicalDevice (VkDevice* device, const VkDeviceCreateInfo* createinfo, const VkPhysicalDevice& physdev);

    void               GetDeviceQueue (VkQueue* que, uint32_t fam_ind, const VkDevice& device); 
    VkPresentModeKHR   ChooseSwapPresentMode (const std::vector<VkPresentModeKHR>& available_modes);
    VkExtent2D         ChooseSwapExtent (const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* win);
  
    bool               CreateSurface (VkSurfaceKHR* surf, GLFWwindow* glfwin, const VkInstance& inst);
    // -------------------------------------------------------------------------
    //
    // -------------------------------------------------------------------------
    bool               CreateSwapchain (Swapchain& swapchain, const Device& device);
    bool               CheckDeviceExtensionSupport (const VkPhysicalDevice& device); 
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat (const std::vector<VkSurfaceFormatKHR>& available_formats);
    bool               GetSwapChainImages (std::vector<Image> &swapchain_images, const Swapchain& swapchain, const VkDevice& dev);

    VkCommandPoolCreateInfo&     CreateInfo (VkCommandPoolCreateInfo& ci, uint32_t queue_family_index);
  // ---------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------
  VkCommandBufferAllocateInfo& AllocateInfo (VkCommandBufferAllocateInfo& alloc_info, uint32_t commandbuffers,  const VkCommandPool& commandpool); 
  VkCommandBufferAllocateInfo& AllocateInfo (VkCommandBufferAllocateInfo& alloc_info, const VkCommandPool& commandpool); 

  bool CreateCommandPool (VkCommandPool& command_pool, const VkCommandPoolCreateInfo&  create_info, const VkDevice& device);
  // bool CreateCommandPool (VkCommandPool&            command_pool,
  //                         VkCommandPoolCreateInfo&  create_info,
  //                         const QueueFamilyIndices& queue_family_inds, const VkDevice& device);
  
  bool CreateCommandBuffers (std::vector<VkCommandBuffer>&      commandbuffers,
                             const VkCommandBufferAllocateInfo& alloc_info, 
                             const VkDevice&                    device); 
  
  bool CreateCommandBuffer(VkCommandBuffer&                   command_buffer,
                           const VkCommandBufferAllocateInfo& create_info, 
                           const VkDevice&                    device); 




  } // cx


  VkPhysicalDeviceFeatures2& ConfigureFeatures  (VkPhysicalDeviceFeatures2& features2, const rokz::PhysicalDevice& physical_device);
  
  bool               InitializeInstance  (rokz::Instance& instance); 
  PhysicalDevice&    ConfigureDevice     (rokz::PhysicalDevice& physical_device, VkBool32 sampler_anisotropy); 
  bool               InitializeDevice    (rokz::Device& device, const rokz::PhysicalDevice& physical_device, const rokz::Instance& instance);
  bool               InitializeDevice    (rokz::Device& device, const VkPhysicalDeviceFeatures2& , const rokz::PhysicalDevice& physical_device, const rokz::Instance& instance);

  bool               InitializeSwapchain (rokz::SwapchainGroup& scg,
                                          const rokz::SwapchainSupportInfo& swapchain_support_info,
                                          const VkSurfaceKHR& surface,
                                          const VkExtent2D&   extent, 
                                          const rokz::PhysicalDevice& physdev,
                                          const rokz::Device& device) ; 

  // -----------------------------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------------------------
  bool               RecreateSwapchain (Swapchain&                          swapchain,
                                        std::vector<Image>&                 swapchain_images, 
                                        std::vector<Framebuffer>&           framebuffers,
                                        std::vector<ImageView>&             image_views, 

                                        RenderPass&                         render_pass, 
                                        Image&                              depth_image, 
                                        ImageView&                          depth_imageview,

                                        Image&                              multisamp_color_image, 
                                        ImageView&                          multisamp_color_imageview,

                                        const Device&                         device,
                                        const VmaAllocator&                   allocator,
                                        GLFWwindow*                           glfwin); 

  // -----------------------------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------------------------
  void CleanupSwapchain (std::vector<Framebuffer>&   framebuffers,
                         std::vector<ImageView>&     fb_image_views,

                         rokz::Image&                msaa_color_image,
                         rokz::ImageView&            msaa_color_imageview,

                         rokz::Image&                depth_image,
                         rokz::ImageView&            depth_imageview,

                         rokz::Swapchain&            swapchain,
                         const rokz::Device&         device); 

  // -----------------------------------------------------------------------------------------------
  // call this something like CleanupInstanceAndStuff
  // -----------------------------------------------------------------------------------------------
  void Cleanup (VkPipeline&                       pipeline,
                std::vector<Framebuffer>&         framebuffers,
                rokz::RenderPass&                 render_pass,
                VkSurfaceKHR&                     surface,
                VkCommandPool&                    command_pool,
                 std::vector<rokz::FrameSync>&    syncs, 
                std::vector<rokz::ShaderModule>&  shader_modules,
                VkPipelineLayout&                 pipeline_layout,
                GLFWwindow*                       win,
                rokz::Device&                     device,
                VkInstance&                       instance);

  // moved 

}
#endif
