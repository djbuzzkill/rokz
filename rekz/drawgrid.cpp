
#include "rekz.h"
#include "rekz/dark_types.h"
#include "rekz/grid_pipeline.h"
#include "rokz/draw_sequence.h"
#include "rokz/rokz_types.h"

// ---------------------------------------------------------------------------------------
//                   
// ---------------------------------------------------------------------------------------

struct drawgrid_buff : public rokz::DrawSequence {

  rokz::rc::Buffer::Ref&  gd; 
  rokz::DescriptorGroup& dg; 
  size_t vertexoffset;
  size_t indexoffset;
  
  drawgrid_buff (rokz::rc::Buffer::Ref& griddata, rokz::DescriptorGroup& descrg, size_t voffs, size_t ioffs)
    : gd (griddata), dg (descrg), vertexoffset (voffs) , indexoffset (ioffs) {
  }

  virtual ~drawgrid_buff () {
  }
  
  // do crap before recording ("UpdateDescriptors()", etc)
  virtual int Prep (uint32_t current_frame, const RenderEnv& env, const rokz::Device& device) { 
    // nada 
    return 0; 
  }
  
  // the draw sequence recording, mebe rename to DrawSeq::Rec() 
  virtual int Exec (VkCommandBuffer comb, uint32_t current_frame, const RenderEnv& env) { 

    using namespace rekz;

    //const DescriptorMap& descrmap = env.descriptormap; 

    grid::PushConstant push_consts = {};

    push_consts.z_color      = glm::vec4 (0.4, 0.7, 0.2, 1.0); 
    push_consts.x_color      = glm::vec4 (0.2, 0.3, 0.9, 1.0);
    push_consts.origin_color = glm::vec4 (0.2, 0.2, 0.2, 1.0);
    push_consts.xstep        = 1.0f;
    push_consts.zstep        = 1.0f;
    push_consts.xoffset      = 0.0f;  
    push_consts.zoffset      = 0.0f;  

    vkCmdBindPipeline (comb, VK_PIPELINE_BIND_POINT_GRAPHICS, env.pipeline.handle);

    vkCmdSetViewport  (comb, 0, 1, &env.pipeline.state.viewport.vps[0].viewport);

    vkCmdSetScissor   (comb, 0, 1, &env.pipeline.state.viewport.vps[0].scissor);

    const uint32_t descr_set_count = 1; //

    VkDescriptorSet descrsets[] = {
      dg.descrsets[current_frame]

    };
    
    vkCmdBindDescriptorSets (comb, VK_PIPELINE_BIND_POINT_GRAPHICS, env.layout, 0,
                             descr_set_count, descrsets, 0, nullptr);

    VkBuffer     vertex_buffers[] = {gd->handle};
    VkDeviceSize offsets[]        = {vertexoffset};

    vkCmdBindVertexBuffers(comb, 0, 1, vertex_buffers, offsets);

    vkCmdBindIndexBuffer(comb, gd->handle,  indexoffset, VK_INDEX_TYPE_UINT16);

    const VkShaderStageFlags shader_stages =
      VK_SHADER_STAGE_VERTEX_BIT ; //| VK_SHADER_STAGE_FRAGMENT_BIT;

    vkCmdPushConstants (comb, env.layout, shader_stages, 0, sizeof(grid::PushConstant), &push_consts);

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


};

  
// ---------------------------------------------------------------------------------------
//                   
// ---------------------------------------------------------------------------------------
rokz::DrawSequence::Ref rekz::CreateDrawGrid (rokz::rc::Buffer::Ref& griddata,
                                              rokz::DescriptorGroup& descrg, size_t voffs, size_t ioffs) {

  return std::make_shared<drawgrid_buff> (griddata, descrg, voffs, ioffs); 
}


