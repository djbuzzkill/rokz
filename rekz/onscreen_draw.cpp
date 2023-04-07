

#include "onscreen_draw.h"
#include "rekz/onscreen_pipe.h"
#include "rokz/shared_descriptor.h"
#include <numeric>
#include <vulkan/vulkan_core.h>




using namespace rokz;
using global_ub::MVPTransform;
using UBText = global_ub::TextItem;

DrawSequence::Ref rekz::onscreen::CreateDrawText (const onscreen::Data& dat, const Vec<VkDescriptorSet>& descriptorsets) { 

  struct osd_draw_text : public DrawSequence  {
    //
    //const Vec<rc::Buffer::Ref>& ubs; 
    const onscreen::Data&       data;
    const Vec<VkDescriptorSet>& dss;    
    // -- construct -----------------------------------------------------
    osd_draw_text (const onscreen::Data& dat, //const Vec<rc::Buffer::Ref>& uniformbuffers,
                   const Vec<VkDescriptorSet>& dsets)
      : data (dat), //ubs (uniformbuffers),
        dss (dsets) {
    }
    // -- prepare  -----------------------------------------------------
    virtual int Prep (uint32_t currentframe, const RenderEnv& env, const rokz::Device& device) {

      return 0;
    }

    // -- draw -----------------------------------------------------
    virtual int Exec (VkCommandBuffer comb, uint32_t currentframe, const RenderEnv& env) {
      int res = __LINE__;
      const rekz::platonic::Mesh& darkmesh = rekz::platonic::Octohedron ();
      vkCmdBindPipeline (comb, VK_PIPELINE_BIND_POINT_GRAPHICS, env.pipeline.handle);

      // b/c these r dynamic state
      vkCmdSetViewport(comb, 0, 1, &env.pipeline.state.viewport.vps[0].viewport);
      vkCmdSetScissor (comb, 0, 1, &env.pipeline.state.viewport.vps[0].scissor);

      std::vector<VkDescriptorSet> descrsets = {
        dss[currentframe], 
      };
  
      vkCmdBindDescriptorSets (comb, VK_PIPELINE_BIND_POINT_GRAPHICS, env.layout, 
                               0, descrsets.size(), &descrsets[0], 0, nullptr);

      //VkBuffer vertex_buffers[] = {polyd.vb_device.handle};
      VkBuffer vertex_buffers[] = {data.geom->handle};
      VkDeviceSize offsets[] = { 0 };

      vkCmdBindVertexBuffers(comb, 0, 1, vertex_buffers, offsets);
      //vkCmdBindIndexBuffer  (comb, polyd.devicebuffer->handle, polyd.indexoffs, VK_INDEX_TYPE_UINT16);
      
      const glm::vec2 string_start_pos (20.0, 20.0f); 
      const float x_advance = 32.0f; 
      const float y_advance = 0.0f; 

      const uint32_t num_test_objects =  1; 
      for (uint32_t i = 0; i < num_test_objects; ++i) {
        rekz::onscreen::PushConstant pc  {}; 
        pc.resource_id = i; 
        pc._unused_1   = i;
        pc._unused_2   = i; 
        pc._unused_3   = i; 

        pc.color    = glm::vec4 (0.1, 0.2, 0.3, 1.0f);
        pc.advance  = glm::vec2(x_advance, y_advance); 
        pc.position = string_start_pos; 

        vkCmdPushConstants (comb,
                            env.layout, 
                            rekz::onscreen::PCStages, //   shader_stages,
                            0,
                            sizeof(rekz::onscreen::PushConstant),
                            &pc);

        vkCmdDraw (comb, 4, data.strings[i].size (), 0, 0); 
      }
      
      return res;
    }

  }; 
  
  return  std::make_shared<osd_draw_text> (dat, descriptorsets); 

}
