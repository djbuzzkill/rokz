
#include "drawmarz.h"
#include "marzdata.h"
#include "landscape_pipeline.h"
#include "rokz/rokz_types.h"
#include <vulkan/vulkan_core.h>


using namespace rokz;
// ------------------------------------------------------------------------------------------
//                        
// ------------------------------------------------------------------------------------------
DrawSequence::Ref marz::CreateDrawMarsLandscape (marz::Data& dat, const Vec<VkDescriptorSet>& dsets)  {

  struct drawmarz : public DrawSequence {

    marz::Data& marzd;
    const Vec<VkDescriptorSet>& dss;
    //
    drawmarz (marz::Data& dat, const Vec<VkDescriptorSet>& descrsets) : marzd (dat), dss (descrsets) {
    }
    //
    virtual ~drawmarz () {
    }

    //
    virtual int Prep (uint32_t current_frame, const RenderEnv& env, const rokz::Device& device) {
      // nada
      return 0;
    }

    //
    virtual int Exec (VkCommandBuffer commb, uint32_t currentframe, const RenderEnv& env) {

      //HERE ("WATTTTAAAAAAAAAAAAAAAAAAAAAAA");

      //virtual int Exec (VkCommandBuffer command_buffer, const shared_globals& globals, const pipeline_assembly& pa, const DescriptorMap& descrmap) {
      //const DescriptorMap& descrmap = env.descriptormap;

      vkCmdBindPipeline (commb, VK_PIPELINE_BIND_POINT_GRAPHICS, env.pipeline.handle);

      // b/c these r dynamic state
      vkCmdSetViewport(commb, 0, 1, &env.pipeline.state.viewport.vps[0].viewport);

      vkCmdSetScissor (commb, 0, 1, &env.pipeline.state.viewport.vps[0].scissor);
      vkCmdSetDepthTestEnable (commb, VK_TRUE); 

      Vec<VkDescriptorSet> descrsets = {
        dss[currentframe], 
        // descrmap.at ("Global"),
        // descrmap.at ("lscape"),
      };

      vkCmdBindDescriptorSets (commb, VK_PIPELINE_BIND_POINT_GRAPHICS, env.layout,
                               0, descrsets.size(), &descrsets[0], 0, nullptr);
          
      VkBuffer     vertex_buffers[] = { marzd.devicebuffer->handle };
      VkDeviceSize voffsets[] = { 0 };

      vkCmdBindVertexBuffers(commb, 0, 1, vertex_buffers, voffsets);

      //const float DEM_scale_mul = 1242.0f;
      const float DEM_scale_mul  = 20.0f; // this just looks right
      const float x_tile_size    = 10.0f;
      const float z_tile_size    = 10.0f;

      glm::vec3 groffs (0.5f * x_tile_size * roi::XDim,
                            0.0f,
                            0.5f * z_tile_size * roi::ZDim);
      
      for (uint32 iz = 0; iz < roi::ZDim ; ++iz) {
        for (uint32 ix = 0; ix < roi::XDim; ++ix) {

          lscape::tile::PushConstant pc {};
          pc.position  = glm::vec4 (ix * x_tile_size - groffs.x , 0.0          , iz * -z_tile_size + groffs.z , 1.0f); 
          pc.scale     = glm::vec4 (x_tile_size     , DEM_scale_mul, z_tile_size, 1.0f); 
          pc.resource_id = iz * roi::XDim + ix;
          
          vkCmdPushConstants (commb, env.layout,
                              lscape::kPCStages, 0, sizeof (lscape::tile::PushConstant), &pc); 
          vkCmdDraw (commb, 4, 1, 0, 0);  
        }} // XZ loop

      return __LINE__;
    }
  }; 


  return std::make_shared<drawmarz> (dat, dsets);
 }

