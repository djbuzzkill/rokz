
#ifndef ROKZ_FRAME_INCLUDE
#define ROKZ_FRAME_INCLUDE

#include "common.hpp"
#include "shared_types.hpp"
#include "rc_swapchain.hpp"

namespace rokz { namespace rc {


    //VkResult          AcquireFrame   (rc::Swapchain::Ref& swapchain, FrameSync& render_sync, uint32_t& image_index, const Device&  device); 
    int  FrameDrawBegin (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index,
                         const VkRenderingInfo& ri, const Device& device);

    int  FrameDrawEnd   (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer,
                         uint32_t image_index, const FrameSync& framesync, const Device& device);

    bool PresentFrame   (VkQueue present_que, const rc::Swapchain::Ref& swapchain, uint32_t& image_index,
                         const FrameSync& render_sync);
    
  }}
#endif
