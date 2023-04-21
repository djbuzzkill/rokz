
#ifndef ROKZ_FRAME_INCLUDE
#define ROKZ_FRAME_INCLUDE

#include "common.hpp"
#include "shared_types.hpp"
#include "rc_swapchain.hpp"

namespace rokz { namespace cx {


    /* -- order of things ----------------------------------------------------------

          +--->---+
          |       |                            
          |       V                            
          |   acquire_frame: signal:image_avaialble | wait_on:in_flight
          |       |
          |       V
          |   draw_begin: nada 
          ^       |
          |       V
          |   draw_end: wait:image_available | signal:render_finished |
          |             signal_on:in_flight | wait_for:COLOR_ATTACHMENT_OUTPUT
          |       |
          |       V
          |  present_Frame: wait:render_finished | 
          |       |                            
          |       |                            
          +---<---+

    ;; ------------------------------------------------------------------------- */

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


//
//        +-------------<-----------<--------------<------------ +           
//        |                                                      |           
//        V                                                      ^           
//    acquire_frame: signal:image_avaialble | wait_on:in_flight  |           
//        |                                                      |           
//        V                                                      ^           
//    draw_geom: wait:image_available  | signal:geombuffer       |           
//        |                                                      |           
//        V                                                      |           
//    draw_light: wait:geombuffer | signal:lighting_finished     |           
//        |                                                      ^           
//        V                                                      |           
//    draw_end: wait:lighting_finished | signal:render_finished  |
//        |          | wait_for:COLOR_ATTACHMENT_OUTPUT          |           
//        |                                                      |           
//        V                                                      ^           
//    present_Frame: wait:render_finished                        |           
//        |                                                      |           
//        |                                                      |           
//        +----------->--------------->------------>------------ +           
//

#endif
