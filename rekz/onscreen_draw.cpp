

#include "onscreen_draw.h"
#include "rekz/onscreen_pipe.h"
#include "rokz/shared_descriptor.h"
#include "rokz/utility.h"
#include <glm/matrix.hpp>
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
      vkCmdSetDepthCompareOp (comb, VK_COMPARE_OP_ALWAYS); 

      // printf ("viewport [x:%f, y:%f, w:%f, h:%f, mind:%f, maxd:%f]\n", 
      //         env.pipeline.state.viewport.vps[0].viewport.x,
      //         env.pipeline.state.viewport.vps[0].viewport.y,
      //         env.pipeline.state.viewport.vps[0].viewport.width, 
      //         env.pipeline.state.viewport.vps[0].viewport.height,
      //         env.pipeline.state.viewport.vps[0].viewport.minDepth, 
      //         env.pipeline.state.viewport.vps[0].viewport.maxDepth); 
      
      
      // printf ("scissor [x:%i, y:%i, w:%u, h:%u]\n", 
      //         env.pipeline.state.viewport.vps[0].scissor.offset.x, 
      //         env.pipeline.state.viewport.vps[0].scissor.offset.y, 

      //         env.pipeline.state.viewport.vps[0].scissor.extent.width,
      //         env.pipeline.state.viewport.vps[0].scissor.extent.height); 

      Vec<VkDescriptorSet> descrsets = { 
        dss[currentframe], 
      };

      vkCmdBindDescriptorSets (comb, VK_PIPELINE_BIND_POINT_GRAPHICS, env.layout, 
                               0, descrsets.size(), &descrsets[0], 0, nullptr);

      //VkBuffer vertex_buffers[] = {polyd.vb_device.handle};
      VkBuffer vertex_buffers[] = {data.geom->handle};
      VkDeviceSize offsets[] = { 0 };

      vkCmdBindVertexBuffers(comb, 0, 1, vertex_buffers, offsets);
      
      const float lt = 0.0f; 
      const float rt = 800.0f;
      const float bt = 600.0f;
      const float tp = 0.0f;
      const float nr = 0.0f;
      const float fr = 1000.0f;
        
      const uint32_t num_test_objects =  1; 
      for (uint32_t i = 0; i < num_test_objects; ++i) {
        rekz::onscreen::PushConstant pc {}; 
        pc.resource_id = i; 
        pc._unused_1   = i;
        pc._unused_2   = i; 
        pc._unused_3   = i; 
        pc.color    = glm::vec4 (0.1, 0.2, 0.8, 1.0f);
        pc.advance  = glm::vec4 (64.0f, 0, 0, 0);
        pc.position = glm::vec4 (0.0f, -132.0f, -1.0f, 0.0f) ;


        // pc.mat = glm::orthoRH_ZO (lt, rt, bt, tp , nr, fr);
        // pc.mat[1][1] *= -1.0; 
        // HERE("orthoRH_ZO"); 
        // ut::printmat (pc.mat);
        pc.mat = ut::orthographic_projection (lt, rt, bt, tp , nr, fr);   //mvpo->proj = glm::ortho (lt, rt, bt, tp); 
        HERE("orthographic_projection"); 
        ut::printmat (pc.mat);

         vkCmdPushConstants (comb, env.layout,
                            rekz::onscreen::PCStages, 0, sizeof(rekz::onscreen::PushConstant), &pc);

        // printf ("data.strings[i].size : %zu| %s \n",   data.strings[i].size (), data.strings[i].c_str());
        vkCmdDraw (comb, 4, data.strings[i].size (), 0, 0);

      }
      
      return 0;
    }

  }; 
  
  return  std::make_shared<osd_draw_text> (dat, descriptorsets); 

}
