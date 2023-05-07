
#ifndef ROKZ_FRAME_INCLUDE
#define ROKZ_FRAME_INCLUDE

#include "common.hpp"
#include "shared_types.hpp"
#include "rc_swapchain.hpp"

namespace rokz { namespace cx {


    bool PresentFrame   (VkQueue present_que, const VkPresentInfoKHR& pi); 

    // ----------------------------------------------------------------------------
    // 
    // ----------------------------------------------------------------------------
    VkPresentInfoKHR& PresentInfo  (VkPresentInfoKHR& pi, uint32_t& image_index, const std::vector<VkSwapchainKHR>& swapchains,  const std::vector<VkSemaphore>& signal_sems); 

    VkResult          AcquireFrame (VkSwapchainKHR& swapchain, FrameSync& render_sync, uint32_t& image_index, const Device&  device); 

    int  FrameDrawBegin (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index,
                         const VkRenderingInfo& ri, const Device& device);
    // 
    int  FrameDrawEnd   (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer,
                         uint32_t image_index, const FrameSync& framesync, const Device& device);

    bool PresentFrame   (VkQueue present_que, const rc::Swapchain::Ref& swapchain,
                         uint32_t& image_index, const FrameSync& render_sync);
    
    // -- multi pass --
    // int  FrameDrawBegin (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index,
    //                      const VkRenderingInfo& ri, const Device& device);

    //
    // -- no_frame_sync -- 
    int  FrameDrawEnd  (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index,
                        VkFence fence_in_flight, VkSemaphore sem_image_available, VkSemaphore sem_render_finished,
                        const Device& device); 
    //
    // -- no_frame_sync -- 
    bool PresentFrame (VkQueue present_que, uint32_t& image_index,
                       const rc::Swapchain::Ref& swapchain, const Vec<VkSemaphore>& waits) ; 
    //
    // -- no_frame_sync -- 
    VkResult AcquireFrame (VkSwapchainKHR& swapchain, uint32_t& image_index,
                           VkFence fen_in_flight, VkSemaphore sem_image_available, const Device&  device); 


    
    
  }}



#endif
