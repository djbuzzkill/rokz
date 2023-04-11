

#include "onscreen_draw.h"
#include "rekz/onscreen_pipe.h"
#include "rokz/shared_descriptor.hpp"
#include "rokz/utility.hpp"

#include <glm/matrix.hpp>
#include <vulkan/vulkan_core.h>




using namespace rokz;
using global_ub::MVPTransform;
//using UBText = global_ub::TextItem;

DrawSequence::Ref rekz::onscreen::CreateDrawText (const onscreen::Data& dat, const Vec<VkDescriptorSet>& descriptorsets) { 

  struct osd_draw_text : public DrawSequence  {
    //
    //const Vec<rc::Buffer::Ref>& ubs; 
    const onscreen::Data&       data;
    const Vec<VkDescriptorSet>& dss;    
    // -- construct -----------------------------------------------------
    osd_draw_text (const onscreen::Data& dat, //const Vec<rc::Buffer::Ref>& uniformbuffers,
                   const Vec<VkDescriptorSet>& dsets)
      : data (dat), dss (dsets) {
    }

    // -- prepare  -----------------------------------------------------
    virtual int Prep (uint32_t currentframe, const RenderEnv& env, const rokz::Device& device) {

      return 0;
    }

    // -- draw -----------------------------------------------------
    virtual int Exec (VkCommandBuffer comb, uint32_t currentframe, const RenderEnv& env) {
      int res = __LINE__;

      vkCmdBindPipeline (comb, VK_PIPELINE_BIND_POINT_GRAPHICS, env.pipeline.handle);
      // b/c these r dynamic state
      vkCmdSetViewport(comb, 0, 1, &env.pipeline.state.viewport.vps[0].viewport);
      vkCmdSetScissor (comb, 0, 1, &env.pipeline.state.viewport.vps[0].scissor);

      vkCmdSetDepthTestEnable (comb, VK_FALSE); 
      // vkCmdSetDepthCompareOp (comb, VK_COMPARE_OP_ALWAYS); 

      Vec<VkDescriptorSet> descrsets = { 
        dss[currentframe], 
      };

      vkCmdBindDescriptorSets (comb, VK_PIPELINE_BIND_POINT_GRAPHICS, env.layout, 
                               0, descrsets.size(), &descrsets[0], 0, nullptr);

      //VkBuffer vertex_buffers[] = {polyd.vb_device.handle};
      VkBuffer vertex_buffers[] = {data.geom->handle};
      VkDeviceSize offsets[] = { 0 };

      vkCmdBindVertexBuffers(comb, 0, 1, vertex_buffers, offsets);
        
      const uint32_t num_test_objects =  1; 

      for (uint32_t i = 0; i < num_test_objects; ++i) {
        for (size_t ich = 0; ich < data.strings[i].size (); ++ich) { 

          rekz::onscreen::PushConstant pc {}; 
          pc.resource_id = ich;
          pc.asciicode   = data.strings[i][ich];
          pc._unused_2   = i; 
          pc._unused_3   = i; 
          pc.color    = glm::vec4 (0.9, 0.4, 0.8, 1.0f);
          pc.advance  = glm::vec4 (16.0f, 0.0f, 0.0f, 0.0f);
          pc.position = glm::vec4 (32.0f, -64.0f, -1.0f, 0.0f);
          //     Y
          //     ^
          //     |
          //     |
          //     |
          //     |
          //     |
          // ----+------------> X
          //     | 
          //     |  [x] text n this quad, kinda awkward
          //     |
          vkCmdPushConstants (comb, env.layout, 
                              rekz::onscreen::PCStages, 0, sizeof(rekz::onscreen::PushConstant), &pc);

          vkCmdDraw (comb, 4, 1, 0, (uint32) ich);
        }
      }
      
      return 0;
    }

  }; 
  
  return  std::make_shared<osd_draw_text> (dat, descriptorsets); 

}
