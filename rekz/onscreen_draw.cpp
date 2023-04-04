

#include "onscreen_draw.h"
#include "rekz/onscreen_pipe.h"
#include <vulkan/vulkan_core.h>




using namespace rokz;
using descriptor::MVPTransform;


DrawSequence::Ref rekz::onscreen::CreateDrawText (const onscreen::Data& dat, const Vec<VkDescriptorSet>& descriptorsets) { 


  struct osd_draw_text : public DrawSequence  {
    
    const onscreen::Data&       data;
    const Vec<VkDescriptorSet>& dss;    

    osd_draw_text (const onscreen::Data& dat, const Vec<VkDescriptorSet>& dsets) :data (dat), dss (dsets) {
      }
    
    virtual int Prep (uint32_t currentframe, const RenderEnv& env, const rokz::Device& device) {

      uint8* ub = reinterpret_cast<uint8*> (rc::MappedPointer (data.ubs[currentframe])); 
      if ( !ub ) {
        HERE("failed to acquire mapped pointer");
        return __LINE__;
      }

      // this shouldnt be done here, it shoud come from Global Uniforms
      MVPTransform* mvp = (MVPTransform*) (ub + ut::offset_at (UB_sizes, 0));
        
      // 
      UBText* ubtext = (UBText*) (ub + ut::offset_at (UB_sizes, 1)); 

      //for (uint32 el =  0; el < kMaxCount; ++el) {
      for (uint32 el =  0; el < 1; ++el) {
        std::copy (data.strings[el].begin (), data.strings[el].end (), ubtext[el].text); 
      }
        
        

      

      // MVP, ubtext, 

      // ??? where does MVP get updated
      // ... overlay transforms should all go into global uniforms
      // ??? 

      // --------------- from drawpolygon ----------------
      // // update uniform buffer 
      // if (PolygonParam* obj = reinterpret_cast<PolygonParam*> (rc::MappedPointer (objbuffs[currentframe] ))) {
      //   //    if (PolygonParam* obj = reinterpret_cast<PolygonParam*> (rokz::cx::MappedPointer ( polyd.vma_poly_uniforms[globals.current_frame] ))) {
      //   glm::vec3 va, vb;
      //   rekz::unit_angle_xz (va, 5.0 * env.globals.sim_time ); 
      //   rekz::unit_angle_xz (vb, 5.0 * env.globals.sim_time + rekz::kPi); 

      //   //glm::mat4 model0 =  glm::translate (glm::mat4(1.0f),  va + glm::vec3 (0.0, -2.0, 0.0));
      //   glm::mat4 model0 =  glm::translate (glm::mat4(1.0f),  glm::vec3 (0.0, 0.0, 0.0));
      //   glm::mat4 model1 =  glm::translate (glm::mat4(1.0f),  glm::vec3 (2.0, 4.0, 0.0));
      //   //for (size_t i = 0; i < kSceneObjCount; ++i) {
      //   obj[0].modelmat = glm::rotate(model0, polyd.objrot[0].y, glm::vec3(0.0f, -1.0f, 0.0f));
      //   obj[1].modelmat = glm::rotate(model1, env.globals.sim_time * glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      // }
      return __LINE__;
    }


    virtual int Exec (VkCommandBuffer comb, uint32_t currentframe, const RenderEnv& env) {

      int res = __LINE__;

      const DescriptorMap& descrmap = env.descriptormap;
    
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
