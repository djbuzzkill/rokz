//
#ifndef REKZ_ONSCREEN_DRAW_INCLUDE
#define REKZ_ONSCREEN_DRAW_INCLUDE

#include "rekz.h"
#include "onscreen_data.h"
#include "rokz/draw_sequence.hpp"
#include <vulkan/vulkan_core.h>

namespace rekz { namespace onscreen {

    using namespace rokz;

    rokz::DrawSequence::Ref CreateDrawText (const onscreen::Data& dat, const Vec<VkDescriptorSet>& descriptorsets); 

  }}


#endif
