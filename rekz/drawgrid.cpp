
#include "rekz.h"
#include "rekz/dark_types.h"
#include "rekz/grid_pipeline.h"
#include "rokz/draw_sequence.h"



struct DrawGrid : public rokz::DrawSequence {

  DrawGrid (const rekz::GridData& dat) : data (dat) {
  }

  virtual ~DrawGrid () {
  }
  
    // do crap before recording ("UpdateDescriptors()", etc)
  virtual int Prep (const shared_globals& globals, const pipeline_assembly& pa, const rokz::Device& device);
    // the draw sequence recording, mebe rename to DrawSeq::Rec() 
  virtual int Exec (VkCommandBuffer comb, const shared_globals& globals, const pipeline_assembly& pa, const DescriptorMap& descrmap); 
  
  const rekz::GridData& data;

};

// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
int DrawGrid::Prep (const shared_globals& globals, const pipeline_assembly& pa, const rokz::Device& device) {
  // nada 
  return 0; 
}

// -------------------------------------------------------------------------------------------
// draw sequence recording, mebe rename to DrawSeq::Rec() 
// -------------------------------------------------------------------------------------------
int DrawGrid::Exec (VkCommandBuffer comb, const shared_globals& globals, const pipeline_assembly& pa, const DescriptorMap& descrmap) {

  rekz::GridPushConstant push_consts = {};

  push_consts.z_color      = glm::vec4 (0.4, 0.7, 0.2, 1.0); 
  push_consts.x_color      = glm::vec4 (0.2, 0.3, 0.9, 1.0);
  push_consts.origin_color = glm::vec4 (0.2, 0.2, 0.2, 1.0);
  push_consts.xstep        = 1.0f;
  push_consts.zstep        = 1.0f;
  push_consts.xoffset      = 0.0f;  
  push_consts.zoffset      = 0.0f;  
  
  vkCmdBindPipeline (comb, VK_PIPELINE_BIND_POINT_GRAPHICS, pa.pipeline.handle);

  vkCmdSetViewport  (comb, 0, 1, &pa.pipeline.state.viewport.vps[0].viewport);

  vkCmdSetScissor   (comb, 0, 1, &pa.pipeline.state.viewport.vps[0].scissor);

  const uint32_t descr_set_count = 1; // b/c grid only needs Globals
  vkCmdBindDescriptorSets (comb, VK_PIPELINE_BIND_POINT_GRAPHICS, pa.plo, 0, descr_set_count, &descrmap.at("Global"), 0, nullptr);

  VkBuffer     vertex_buffers[] = {data.vb_device.handle};
  VkDeviceSize offsets[]        = {0};

  vkCmdBindVertexBuffers(comb, 0, 1, vertex_buffers, offsets);

  vkCmdBindIndexBuffer(comb, data.ib_device.handle, 0, VK_INDEX_TYPE_UINT16);

  const VkShaderStageFlags shader_stages =
    VK_SHADER_STAGE_VERTEX_BIT ; //| VK_SHADER_STAGE_FRAGMENT_BIT;

  vkCmdPushConstants (comb, pa.plo, shader_stages, 0, sizeof(rekz::GridPushConstant), &push_consts);

  const uint32_t vdim   = 11; // matches SetupGrid
  const uint32_t totalv = vdim * vdim;
  // 2 sets of lines criss cross
  for  (uint32_t iz = 0; iz < vdim; ++iz) {
    uint32_t first_index = vdim * iz ;
    vkCmdDrawIndexed (comb, vdim, 1, first_index, 0, 0);
  }    

  for (uint32_t ix = 0; ix < vdim; ++ix) {
    uint32_t first_index = totalv + vdim * ix;
    vkCmdDrawIndexed (comb, vdim, 1, first_index, 0, 0);
  }
    
  return 0; 
}


// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
rokz::DrawSequence::Ref rekz::CreateDrawGrid (const GridData& dat) {

  return std::make_shared<DrawGrid> (dat); 
}


