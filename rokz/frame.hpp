//
#ifndef ROKZ_FRAME_INCLUDE
#define ROKZ_FRAME_INCLUDE

#include "common.hpp"
#include "shared_types.hpp"
#include "rc_swapchain.hpp"

namespace rokz { namespace cx {
    
    // ------------------------------------------------------------------------------------------------
    //
    // ------------------------------------------------------------------------------------------------
    VkResult AcquireFrame (VkSwapchainKHR& swapchain, uint32_t& image_index,
                           VkFence fen_in_flight,             // fence_sit
                           VkSemaphore sem_image_available,   // signal
                           const Device& device); 

    // ------------------------------------------------------------------------------------------------
    // dynamic_rendering
    // ------------------------------------------------------------------------------------------------
    int  FrameDrawingBegin (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index,
                            const VkRenderingInfo& ri, const Device& device);
    // -- no_frame_sync -- 
    int  FrameDrawingEnd  (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index,
                           VkSemaphore sem_image_available, // wait 
                           VkSemaphore sem_render_finished, // wait/signal
                           VkFence fence_in_flight,         // fence_flag 
                           const Device& device); 
    
    // ------------------------------------------------------------------------------------------------
    // Framebuffer+RenderPasse
    // -k-----------------------------------------------------------------------------------------------
    void BeginRenderPass (VkCommandBuffer cb,
                         VkRenderPass             renderpass, 
                         VkFramebuffer            framebuffer, 
                         VkSubpassContents        contents,
                         const VkRect2D&          ext2d, 
                         const Vec<VkClearValue>& clearvalues);
    
    void NextSubpass     (VkCommandBuffer cb, VkSubpassContents begin_contents); 

    bool EndRenderPass (VkCommandBuffer cb, VkQueue queue, VkSemaphore wait_sem,
                        VkSemaphore signal_sem, VkFence signal_fence);

    bool EndRenderPass   (VkCommandBuffer cb); 
    // ----------------------------------------------------------------------------
    // no FrameSync
    // ----------------------------------------------------------------------------
    bool PresentFrame (VkQueue present_que, uint32_t& image_index,
                       const rc::Swapchain::Ref& swapchain, const Vec<VkSemaphore>& waits) ; 

    VkPresentInfoKHR& PresentInfo  (VkPresentInfoKHR& pi, uint32_t& image_index,
                                    const std::vector<VkSwapchainKHR>& swapchains, VkSemaphore wait); 

    VkPresentInfoKHR& PresentInfo  (VkPresentInfoKHR& pi, uint32_t& image_index,
                                    const std::vector<VkSwapchainKHR>& swapchains,
                                    const std::vector<VkSemaphore>& waits); 
    // --
    bool PresentFrame   (VkQueue present_que, const VkPresentInfoKHR& pi); 

    
    
  }}



#endif
