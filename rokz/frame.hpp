
#ifndef ROKZ_FRAME_INCLUDE
#define ROKZ_FRAME_INCLUDE

#include "common.hpp"
#include "shared_types.hpp"
#include "rc_swapchain.hpp"

namespace rokz { namespace cx {
    
    //
    // -- no_frame_sync -- 
    VkResult AcquireFrame (VkSwapchainKHR& swapchain, uint32_t& image_index,
                           VkFence fen_in_flight,             // fence_sit
                           VkSemaphore sem_image_available,   // signal
                           const Device& device); 
    int  FrameDrawBegin (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index,
                         const VkRenderingInfo& ri, const Device& device);
    // -- no_frame_sync -- 
    int  FrameDrawEnd  (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index,
                        VkSemaphore sem_image_available, // wait 
                        VkSemaphore sem_render_finished, // wait/signal
                        VkFence fence_in_flight,         // fence_flag 
                        const Device& device); 

    // -- multi pass --
    // int  FrameDrawBegin (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index,
    //                      const VkRenderingInfo& ri, const Device& device);

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
