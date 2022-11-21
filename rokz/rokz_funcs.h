
#ifndef ROKZ_FUNCS_INCLUDE
#define ROKZ_FUNCS_INCLUDE


#include "common.h"
#include "utility.h"
#include "rokz_types.h"


namespace rokz {

  // ------------------------------------------------------------------
  //
  // ------------------------------------------------------------------
  bool Initialize (Glob& glob);

  VkSwapchainCreateInfoKHR& Default (VkSwapchainCreateInfoKHR& info, const VkSurfaceKHR& surf); 
  Glob&                     Default (Glob& g); 
  VkDeviceQueueCreateInfo&  Default (VkDeviceQueueCreateInfo& info, uint32_t que_fam_index, float* q_priority);
  VkDeviceCreateInfo&       Default (VkDeviceCreateInfo& info, VkDeviceQueueCreateInfo* quecreateinfo, VkPhysicalDeviceFeatures* devfeats); 
  // ------------------------------------------------------------------
  //
  // ------------------------------------------------------------------
  QueueFamilyIndices&       FindQueueFamilies (QueueFamilyIndices& queue_fams, const VkSurfaceKHR& surf, const VkPhysicalDevice& physdev);
  SwapchainSupportInfo&     QuerySwapchainSupport (SwapchainSupportInfo& deets, const VkSurfaceKHR& surf, const VkPhysicalDevice& dev); 
  
  GLFWwindow*        CreateWindow_glfw (GLFWwindow*& w);
  bool               CheckValidationSupport (const std::vector<const char*>& validation_layers);
  //int CreateInstance (Instance, appinfo, createinfo);

  int CreateInstance (VkInstance& instance, VkApplicationInfo& app_info, VkInstanceCreateInfo& inst_info);  
  int                CreateInstance (Glob& glob);
  bool               SelectPhysicalDevice (VkPhysicalDevice& physdev, QueueFamilyIndices& queueind, const VkSurfaceKHR& surf, const VkInstance& inst);
  bool               CreateLogicalDevice (VkDevice* device, const VkDeviceCreateInfo* createinfo, const VkPhysicalDevice& physdev);

  void               GetDeviceQueue (VkQueue* que, uint32_t fam_ind, const VkDevice& device); 
  VkSurfaceFormatKHR ChooseSwapSurfaceFormat (const std::vector<VkSurfaceFormatKHR>& available_formats);
  VkPresentModeKHR   ChooseSwapPresentMode (const std::vector<VkPresentModeKHR>& available_modes);
  VkExtent2D         ChooseSwapExtent (const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* win);
  
  bool               CreateSurface (VkSurfaceKHR* surf, GLFWwindow* glfwin, const VkInstance& inst);
  bool               CreateSwapchain (VkSwapchainKHR& swapchain, VkSwapchainCreateInfoKHR& swapchaincreateinfo, const VkSurfaceKHR& surf, const VkPhysicalDevice& physdev, const VkDevice& dev, GLFWwindow* glfwin);

  bool RecreateSwapchain (VkSwapchainKHR&                       swapchain,
                          VkSwapchainCreateInfoKHR&             swapchaincreateinfo,
                          std::vector<VkImage>&                 swapchain_images, 
                          std::vector<VkFramebuffer>&           framebuffers,
                          std::vector<VkFramebufferCreateInfo>& create_infos,
                          RenderPass&                           render_pass, 
                          std::vector<VkImageView>&             image_views, 
                          VkSurfaceKHR&                         surf,
                          VkPhysicalDevice&                     physdev,
                          VkDevice& dev, GLFWwindow*            glfwin); 

  bool               GetSwapChainImages (std::vector<VkImage> &swapchain_images, VkSwapchainKHR& swapchain, const VkDevice& dev);
  bool               CreateImageViews(std::vector<VkImageView> &swapchain_imageviews, const std::vector<VkImage> &swapchain_images, VkFormat surf_fmt, const VkDevice& dev); 
  
  VkShaderModule&    CreateShaderModule (VkShaderModule& shader_module, const bytearray& code, const VkDevice& dev); 
  bool               CreateShaderModules (std::vector<VkShaderModule>& shader_modules, std::vector<VkPipelineShaderStageCreateInfo> &shader_stage_create_infos, const std::filesystem::path& fspath, const VkDevice& device); 
  bool               CreateDynamicStates (std::vector<VkDynamicState>& dynamic_states, VkPipelineDynamicStateCreateInfo& dynamic_state_create_info); 
  bool               CreateColorBlendState (VkPipelineColorBlendAttachmentState& color_blend_attachment_state, VkPipelineColorBlendStateCreateInfo& color_blending_create_info); 
  //bool               CreateRenderPass (VkRenderPass& render_pass, VkRenderPassCreateInfo& create_info, VkFormat swapchain_format, const VkDevice& device); 

  bool               CreateRenderPass (RenderPass& render_pass,
                                       VkFormat swapchain_format,
                                       const VkDevice &device); 

  bool CreateGraphicsPipelineLayout (VkPipelineLayout&            pipeline_layout,
                                     VkViewport&                  viewport, 
                                     VkRect2D&                    scissor,
                                     CreateInfo&                  create_info,
                                     const VkExtent2D&            swapchain_extent,
                                     const VkDevice&              device); 

  bool CreateGraphicsPipeline (VkPipeline &pipeline,
                               VkGraphicsPipelineCreateInfo &pipeline_create_info,
                               const CreateInfo&            create_info, 
                               const VkPipelineLayout&      pipeline_layout,
                               const RenderPass&          render_pass,
                               const VkDevice               device); 

  bool CreateFramebuffers (std::vector<VkFramebuffer>&           framebuffers,
                           std::vector<VkFramebufferCreateInfo>& create_infos,
                           const RenderPass&                   render_pass, 
                           const VkExtent2D                      swapchain_ext, 
                           const std::vector<VkImageView>&       image_views, 
                           const VkDevice&                       device); 


  bool CreateCommandPool (VkCommandPool&            command_pool,
                          VkCommandPoolCreateInfo&  create_info,
                          const QueueFamilyIndices& queue_family_inds, const VkDevice& device);
  
  bool CreateCommandBuffer(VkCommandBuffer &command_buffer,
                           VkCommandBufferAllocateInfo& create_info, 
                           const VkCommandPool &command_pool,
                           const VkDevice &device); 


  bool RecordCommandBuffer(VkCommandBuffer &command_buffer,
                           const VkPipeline pipeline,
                           const VkExtent2D &ext2d,
                           const VkFramebuffer &framebuffer,
                           const RenderPass &render_pass,
                           const VkDevice &device) ; 
  
  bool CreateSyncObjs (SyncStruc& sync, SyncCreateInfo& create_info, const VkDevice& device);



  void CleanupSwapchain (std::vector<VkFramebuffer>& framebuffers, 
                         std::vector<VkImageView>&   image_views,
                         VkSwapchainKHR&             swapchain,
                         const VkDevice&             device); 

  
  void Cleanup (VkPipeline&                 pipeline,
                std::vector<VkFramebuffer>& framebuffers, 
                VkSwapchainKHR&             swapchain,
                VkSurfaceKHR&               surf,
                VkCommandPool&              command_pool, 
                std::vector<SyncStruc>&     syncs, 
                std::vector<VkShaderModule>& shader_modules,
                VkPipelineLayout&           pipeline_layout,
                RenderPass&                 render_pass,
                std::vector<VkImageView>&  image_views,
                GLFWwindow* w,
                VkDevice& dev,
                VkInstance &inst);


  // ------------------------------------------------------------------
  //
  // ------------------------------------------------------------------
  
  bool CreateFrameBuffers ();
  bool CreateCommandPool (); 
  bool CreateCommandBuffer ();
  bool CreateSyncObjects ();

} // namespace rokz

#endif
