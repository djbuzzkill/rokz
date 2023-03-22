
#include "darkrootgarden.h"
#include "rekz/dark_types.h"

#include "dark_obj_pipeline.h"
#include <vulkan/vulkan_core.h>


using namespace rekz;
// -------------------------------------------------------------------------
// draw instructions, pre draw
// -------------------------------------------------------------------------
struct PolygonDraw : public rokz::DrawSequence {
  //
  const rekz::PolygonData&         polyd;
  const rokz::DescriptorGroup&     object_descr;
  const std::vector<rokz::Buffer>& object_buffs;

  // 
  PolygonDraw (const rekz::PolygonData& d, const std::vector<rokz::Buffer>& objres, const rokz::DescriptorGroup& descg)
    : polyd (d), object_buffs(objres), object_descr (descg) {
  }

  virtual ~PolygonDraw () {
    // nada
  }
  
  // ------------------------------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------------------------------
  virtual int Prep (uint32_t currentframe, const RenderEnv& env, const rokz::Device& device) { 

    // update uniform buffer 
    if (PolygonParam* obj = reinterpret_cast<PolygonParam*> (rokz::cx::MappedPointer (object_buffs[currentframe]))) {
      //    if (PolygonParam* obj = reinterpret_cast<PolygonParam*> (rokz::cx::MappedPointer ( polyd.vma_poly_uniforms[globals.current_frame] ))) {
      glm::vec3 va, vb;
      rekz::unit_angle_xz (va, 5.0 * env.globals.sim_time ); 
      rekz::unit_angle_xz (vb, 5.0 * env.globals.sim_time + darkroot::kPi); 

      //glm::mat4 model0 =  glm::translate (glm::mat4(1.0f),  va + glm::vec3 (0.0, -2.0, 0.0));
      glm::mat4 model0 =  glm::translate (glm::mat4(1.0f),  glm::vec3 (0.0, 0.0, 0.0));
      glm::mat4 model1 =  glm::translate (glm::mat4(1.0f),  glm::vec3 (2.0, 4.0, 0.0));
      //for (size_t i = 0; i < kSceneObjCount; ++i) {
      obj[0].modelmat = glm::rotate(model0, polyd.objrot[0].y, glm::vec3(0.0f, -1.0f, 0.0f));

      obj[1].modelmat = glm::rotate(model1, env.globals.sim_time * glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    }
  
    return 0;
  }

  // ------------------------------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------------------------------
  virtual int Exec (VkCommandBuffer combuf, uint32_t currentframe, const RenderEnv& env) { 
  //virtual int Exec (VkCommandBuffer command_buffer, const shared_globals& globals, const pipeline_assembly& pa, const DescriptorMap& descrmap) {
    const DescriptorMap& descrmap = env.descriptormap;
    
    const rekz::platonic::Mesh& darkmesh = rekz::platonic::Octohedron ();
    vkCmdBindPipeline (combuf, VK_PIPELINE_BIND_POINT_GRAPHICS, env.pa.pipeline.handle);

    // b/c these r dynamic state
    vkCmdSetViewport(combuf, 0, 1, &env.pa.pipeline.state.viewport.vps[0].viewport);
    vkCmdSetScissor (combuf, 0, 1, &env.pa.pipeline.state.viewport.vps[0].scissor);

    //VK_POLYGON_MODE_FILL = 0,
    //vkCmdSetPolygonModeEXT (command_buffer, VK_POLYGON_MODE_LINE); 
    std::vector<VkDescriptorSet> descrsets;
    descrsets.push_back (descrmap.at ("Global"));
    descrsets.push_back (object_descr.descrsets[currentframe]); 
  
    vkCmdBindDescriptorSets (combuf, VK_PIPELINE_BIND_POINT_GRAPHICS, env.pa.plo,
                             0, descrsets.size(), &descrsets[0], 0, nullptr);

    //VkBuffer vertex_buffers[] = {polyd.vb_device.handle};
    VkBuffer vertex_buffers[] = {polyd.devicebuffer->handle};
    VkDeviceSize offsets[] = { polyd.vertexoffs };

    vkCmdBindVertexBuffers(combuf, 0, 1, vertex_buffers, offsets);
    vkCmdBindIndexBuffer  (combuf, polyd.devicebuffer->handle, polyd.indexoffs, VK_INDEX_TYPE_UINT16);

    const uint32_t num_test_objects =  2; 
    for (uint32_t i = 0; i < num_test_objects; ++i) {

      rekz::obz::PushConstant pcs {};
      pcs.resourceID = i; 
      pcs._unused_01 = i; 
      pcs._unused_02 = i; 
      pcs._unused_03 = i; 

      vkCmdPushConstants (combuf,
                          env.pa.plo, 
                          rekz::obz::PCStages, //   shader_stages,
                          0,
                          sizeof(rekz::obz::PushConstant),
                          &pcs);

      vkCmdDrawIndexed (combuf, darkmesh.indices.size(), 1, 0, 0, 0);
    }

    return 0;
  }

}; // PolygonDraw

// -------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------
rokz::DrawSequence::Ref rekz::CreatePolygonDraw (const PolygonData&               dat,
                                                 const std::vector<rokz::Buffer>& objres,
                                                 const rokz::DescriptorGroup&     descg)
{
  return std::make_shared<PolygonDraw> (dat, objres, descg);
}

