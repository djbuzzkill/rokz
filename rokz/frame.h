
#ifndef ROKZ_FRAME_INCLUDE
#define ROKZ_FRAME_INCLUDE

#include "common.h"
#include "shared_types.h"
#include "rc_swapchain.h"

namespace rokz { namespace rc {


    //VkResult          AcquireFrame   (rc::Swapchain& swapchain, FrameSync& render_sync, uint32_t& image_index, const Device&  device); 
    int               FrameDrawBegin (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index, const VkRenderingInfo& ri, const Device& device);
    int               FrameDrawEnd   (rc::SwapchainGroup& scg, VkCommandBuffer command_buffer, uint32_t image_index, const FrameSync& framesync, const Device& device);



    
  }}
#endif
