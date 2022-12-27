

#ifndef ROKZ_RENDER_H
#define ROKZ_RENDER_H


#include "common.h"
#include "shared_types.h"

namespace rokz { 

  VkResult AcquireFrame (Swapchain& swapchain, RenderSync& render_sync, uint32_t& image_index, const Device&  device); 
  bool PresentFrame (VkQueue& present_que, const Swapchain& swapchain, uint32_t& image_index, RenderSync& render_sync); 



}

#endif
