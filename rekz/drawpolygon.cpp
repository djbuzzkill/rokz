
#include "darkrootgarden.h"
#include "rekz/dark_types.h"

#include "dark_obj_pipeline.h"
#include <vulkan/vulkan_core.h>


using namespace rekz;
// should rename darkpolygon.cpp --> darwpolugon.cpp
// -------------------------------------------------------------------------
// a DrawSequence should not directly own data but only represent
// draw instruction + how to  setup  input
// -------------------------------------------------------------------------
struct PolygonDraw : public rokz::DrawSequence {

  PolygonDraw (const rekz::PolygonData& d, const std::vector<rokz::Buffer>& objres) : polyd (d), objres_buffs(objres) {
  }

  virtual    ~PolygonDraw () { }
  virtual int Prep        (const shared_globals& , const pipeline_assembly& pa, const rokz::Device& device);
  virtual int Exec        (VkCommandBuffer comb, const pipeline_assembly& pa, const std::vector<VkDescriptorSet>& ds);
  
protected:

  const rekz::PolygonData&  polyd;
  const std::vector<rokz::Buffer>& objres_buffs;
  
}; // PolygonDraw

// -------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------
rokz::DrawSequence::Ref rekz::CreatePolygonDraw (const rekz::PolygonData& d, const std::vector<rokz::Buffer>& objres)
{
  return std::make_shared<PolygonDraw> (d, objres);
}

// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
int PolygonDraw::Prep (const shared_globals& globals, const pipeline_assembly& pa, const rokz::Device& device) {


  
  //polyd.vma_poly_uniforms[globals.current_frame]
  
  // update uniform buffer 
  // SceneObjParam

  if (PolygonParam* obj = reinterpret_cast<PolygonParam*> (rokz::cx::MappedPointer ( objres_buffs[globals.current_frame] ))) {
      //    if (PolygonParam* obj = reinterpret_cast<PolygonParam*> (rokz::cx::MappedPointer ( polyd.vma_poly_uniforms[globals.current_frame] ))) {
    glm::vec3 va, vb;
    rekz::unit_angle_xz (va, 5.0 * globals.sim_time ); 
    rekz::unit_angle_xz (vb, 5.0 * globals.sim_time + darkroot::kPi); 

    glm::mat4 model0 =  glm::translate (glm::mat4(1.0f),  va + glm::vec3 (0.0, 0.5, -6.0));
    glm::mat4 model1 =  glm::translate (glm::mat4(1.0f),  vb + glm::vec3 (0.0, -0.5,-6.0));
    //for (size_t i = 0; i < kSceneObjCount; ++i) {
    obj[0].modelmat = glm::rotate(model0, polyd.obj_theta[0], glm::vec3(0.0f, -1.0f, 0.0f));
    obj[1].modelmat = glm::rotate(model1, globals.sim_time * glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  }
  
  return 0;
}



// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
int PolygonDraw::Exec (VkCommandBuffer command_buffer, const pipeline_assembly& pa, const std::vector<VkDescriptorSet>& descrsets) {


  const rekz::PolyMesh& darkmesh = rekz::platonic::Octohedron ();
  // ext2D  used to come from -> swapchain.ci.imageExtent
  //    VkViewport viewport{};
  //    viewport.x = 0.0f;
  //    viewport.y = 0.0f;
  //    viewport.width = static_cast<float>(ext2d.width);
  //    viewport.height = static_cast<float>(ext2d.height);
  //    viewport.minDepth = 0.0f;
  //    viewport.maxDepth = 1.0f;
        
  //    VkRect2D scissor{};
  //    scissor.offset = {0, 0};
  //    scissor.extent = ext2d;
  vkCmdBindPipeline (command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pa.pipeline.handle);

  vkCmdSetViewport(command_buffer, 0, 1, &pa.pipeline.state.viewport.vps[0].viewport);

  vkCmdSetScissor(command_buffer, 0, 1, &pa.pipeline.state.viewport.vps[0].scissor);

  //VK_POLYGON_MODE_FILL = 0,
  //vkCmdSetPolygonModeEXT (command_buffer, VK_POLYGON_MODE_LINE); 
  
  vkCmdBindDescriptorSets (command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pa.plo,
                           0, descrsets.size(), &descrsets[0], 0, nullptr);

  VkBuffer vertex_buffers[] = {polyd.vb_device.handle};
  VkDeviceSize offsets[] = {0};

  vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);
  vkCmdBindIndexBuffer(command_buffer, polyd.ib_device.handle, 0, VK_INDEX_TYPE_UINT16);

  const uint32_t num_test_objects =  2; 
  for (uint32_t i = 0; i < num_test_objects; ++i) {

    rekz::PushConstants pcs {};
    pcs.resourceID = i; 
    pcs._unused_01 = i; 
    pcs._unused_02 = i; 
    pcs._unused_03 = i; 

    const VkShaderStageFlags shader_stages =
      VK_SHADER_STAGE_VERTEX_BIT ; //| VK_SHADER_STAGE_FRAGMENT_BIT;

    vkCmdPushConstants (command_buffer,
                        pa.plo, 
                        shader_stages,
                        0,
                        sizeof(rekz::PushConstants),
                        &pcs);

    vkCmdDrawIndexed (command_buffer, darkmesh.indices.size(), 1, 0, 0, 0);
  }

  return 0;
}



#ifdef DARKROOT_HIDE_UPDATE_DARK_UNIFORMS
// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
void update_dark_uniforms (Glob& glob, uint32_t current_frame, double dt) {
  //static auto startTime = std::chrono::high_resolution_clock::now();
  glob.sim_time += dt;
  //  printf ( " - %s(dt:%f, sim_time:%f)\n", __FUNCTION__, dt, glob.sim_time);

  
  float sim_timef = glob.sim_time;
  float dtF = static_cast <float> (dt);
  float asp = (float)glob.swapchain_group.swapchain.ci.imageExtent.width / (float)glob.swapchain_group.swapchain.ci.imageExtent.height;
    
  glm::mat4  posmat =   glm::translate  (glm::mat4(1.0), glm::vec3 (0.0, .5, -5.0));
  // printf ("m0 * v0 = <%f, %f, %f, %f>  \n",  v0.x, v0.y, v0.z, v0.w); 
  // printf ("v1 * m0 = <%f, %f, %f, %f>  \n",  v1.x, v1.y, v1.z, v1.w); 
  // printf ("m[3][0]=%f | m[3][1]=%f | m[3][2]=%f  \n",  m0[3][0], m0[3][1], m0[3][2] ); 
  rokz::MVPTransform mats; 
  mats.model = glm::rotate(posmat, sim_timef * glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));

  
  mats.view = glm::rotate (glm::mat4(1), glob.view_orie.theta, glm::vec3(0.0f, 1.0f, 0.0f)) * 
    //glm::rotate (glm::mat4(1), glob.view_orie.phi, glm::vec3(1.0f, 0.0f, 0.0f)) *
    glm::translate (glm::mat4(1.0), glm::vec3 (0.0, .5, -5.0)); 

  // mats.view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  mats.proj  = glm::perspective(glm::radians(45.0f), asp , 1.0f, 20.0f);
  mats.proj[1][1] *= -1;

  // MVPTransform
  memcpy (rokz::cx::MappedPointer (glob.polyd.vma_uniform_buffs[current_frame]), &mats, rokz::kSizeOf_MVPTransform); 

  // SceneObjParam
  if (SceneObjParam* obj = reinterpret_cast<SceneObjParam*> (rokz::cx::MappedPointer (glob.polyd.vma_objparam_buffs[current_frame]))) {
  
    glm::vec3 va, vb;
    unit_angle_xz (va, 5.0 * sim_timef ); 
    unit_angle_xz (vb, 5.0 * sim_timef + kPi); 

    glm::mat4 model0 =  glm::translate (glm::mat4(1.0f),  va + glm::vec3 (0.0, 0.5, -6.0));
    glm::mat4 model1 =  glm::translate (glm::mat4(1.0f),  vb + glm::vec3 (0.0, -0.5,-6.0));

    glob.polyd.obj_theta[0] += mouse_dx * dtF * darkroot::k2Pi;
    
    //for (size_t i = 0; i < kSceneObjCount; ++i) {
    obj[0].modelmat = glm::rotate(model0, glob.polyd.obj_theta[0], glm::vec3(0.0f, -1.0f, 0.0f));
//obj[0].modelmat = glm::rotate(model0, sim_timef * glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    obj[1].modelmat = glm::rotate(model1, sim_timef * glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
  }
}
#endif






//
// ?? a different DrawSequence subtype for each pipeline,
// yes: ex:  textured drawvs wireframe draw but can share same data  
struct DrawPolyWireframe : public rokz::DrawSequence
{

public:
  
  DrawPolyWireframe  (const rekz::PolygonData& d) : polyd (d) {
  }

  virtual ~DrawPolyWireframe () {
  }

  virtual int Prep (const shared_globals& shared, const pipeline_assembly& pa, const rokz::Device& device) {
    return 0;
  }
  virtual int Exec (VkCommandBuffer comb, const pipeline_assembly& pa, const std::vector<VkDescriptorSet>& ds) {
    return 0;
  }

protected:

  const rekz::PolygonData& polyd;
  
}; 

// -------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------
std::shared_ptr<rokz::DrawSequence> rekz::CreatePolygonWireframe (const rekz::PolygonData& d) {

  return std::make_shared<DrawPolyWireframe> (d);
  
}

