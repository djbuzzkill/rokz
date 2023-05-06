
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

    //VkResult          AcquireFrame (Swapchain& swapchain, FrameSync& render_sync, uint32_t& image_index, const Device&  device); 
    VkResult          AcquireFrame (VkSwapchainKHR& swapchain, FrameSync& render_sync, uint32_t& image_index, const Device&  device); 

    
    //bool              PresentFrame (VkQueue present_que, const Swapchain& swapchain, uint32_t& image_index, const FrameSync& render_sync); 

    //VkResult          AcquireFrame   (rc::Swapchain::Ref& swapchain, FrameSync& render_sync, uint32_t& image_index, const Device&  device); 
    int  FrameDrawBegin (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index,
                         const VkRenderingInfo& ri, const Device& device);
    // 

    
    int  FrameDrawEnd   (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer,
                         uint32_t image_index, const FrameSync& framesync, const Device& device);

    bool PresentFrame   (VkQueue present_que, const rc::Swapchain::Ref& swapchain, uint32_t& image_index, const FrameSync& render_sync);
    
    
    // multi pass 
    // int  FrameDrawBegin (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index,
    //                      const VkRenderingInfo& ri, const Device& device);

    int  FrameDrawEnd  (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer,
                        uint32_t image_index,

                        VkFence     fence_in_flight, 
                        VkSemaphore sem_image_available,
                        VkSemaphore sem_render_finished,
                         
                        const Device& device); 

    bool PresentFrame (VkQueue present_que,
                       const rc::Swapchain::Ref& swapchain,
                       uint32_t& image_index,
                       const Vec<VkSemaphore>& waits) ; 


    VkResult AcquireFrame (VkSwapchainKHR& swapchain,
                           VkFence     fen_in_flight,
                           VkSemaphore sem_image_available,
                           //FrameSync& render_sync,
                           uint32_t& image_index, const Device&  device); 


    
    
  }}



#endif
