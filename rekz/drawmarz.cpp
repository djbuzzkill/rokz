
#include "drawmarz.h"
#include "marzdata.h"
#include "landscape_pipeline.h"


using namespace rokz;
// ------------------------------------------------------------------------------------------
//                        
// ------------------------------------------------------------------------------------------
DrawSequence::Ref marz::CreateDrawMarsLandscape (marz::Data& dat)  {

  struct drawmarz : public DrawSequence {

    marz::Data& marzd;
    //
    drawmarz (marz::Data& dat) : marzd (dat) {}
    //
    virtual ~drawmarz () { }

    //
    virtual int Prep (uint32_t current_frame, const RenderEnv& env, const rokz::Device& device) {



      
      return __LINE__;
    }

    //
    virtual int Exec (VkCommandBuffer commb, uint32_t currentframe, const RenderEnv& env) {

      //virtual int Exec (VkCommandBuffer command_buffer, const shared_globals& globals, const pipeline_assembly& pa, const DescriptorMap& descrmap) {
      const DescriptorMap& descrmap = env.descriptormap;
    
      const rekz::platonic::Mesh& darkmesh = rekz::platonic::Octohedron ();
      vkCmdBindPipeline (commb, VK_PIPELINE_BIND_POINT_GRAPHICS, env.pa.pipeline.handle);

      // b/c these r dynamic state
      vkCmdSetViewport(commb, 0, 1, &env.pa.pipeline.state.viewport.vps[0].viewport);

      vkCmdSetScissor (commb, 0, 1, &env.pa.pipeline.state.viewport.vps[0].scissor);

      //VK_POLYGON_MODE_FILL = 0,
      //vkCmdSetPolygonModeEXT (command_buffer, VK_POLYGON_MODE_LINE); 
      for (uint32 iz = 0; iz < roi::ZDim ; ++iz) {
        for (uint32 ix = 0; ix < roi::XDim; ++ix) {

          lscape::tile::PushConstant pc;
          pc.position;
          pc.scale;
          pc.resource_id;
          
          sizeof (lscape::tile::PushConstant); 


          // std::vector<VkDescriptorSet> descrsets;
          // descrsets.push_back (descrmap.at ("Global"));
          // descrsets.push_back (marzd.descrsets[currentframe]); 
  
          // vkCmdBindDescriptorSets (commb, VK_PIPELINE_BIND_POINT_GRAPHICS, env.pa.plo,
          //                          0, descrsets.size(), &descrsets[0], 0, nullptr);

          // //VkBuffer vertex_buffers[] = {polyd.vb_device.handle};
          // VkBuffer vertex_buffers[] = {marzd.devicebuffer->handle};
          // VkDeviceSize offsets[] = { marzd.vertexoffs };

          // vkCmdBindVertexBuffers(commb, 0, 1, vertex_buffers, offsets);
          // vkCmdBindIndexBuffer  (commb, marzd.devicebuffer->handle, marzd.indexoffs, VK_INDEX_TYPE_UINT16);


        }} // XZ loop

      return __LINE__;
    }

  }; 


  return std::make_shared<drawmarz> (dat);
}



