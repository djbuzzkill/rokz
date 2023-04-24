
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

    bool PresentFrame   (VkQueue present_que, const rc::Swapchain::Ref& swapchain, uint32_t& image_index,
                         const FrameSync& render_sync);
    
  }}



#endif
