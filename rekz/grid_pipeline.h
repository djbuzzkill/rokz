//
#ifndef REKZ_GRID_PIPELINE
#define REKZ_GRID_PIPELINE

#include "rekz.h"

namespace rekz { 


  //
  //
  typedef rekz::Vertex_pos_col    GridVert; 
  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  extern const VkVertexInputBindingDescription        kGridVertexBindingDesc;
  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  extern const Vec<VkVertexInputAttributeDescription> kGridVertInputAttributeDesc;

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  struct GridPushConstant {

    glm::vec4 x_color;  // 10 is reasonable
    glm::vec4 z_color;  // 10 is reasonable
    glm::vec4 origin_color; 

    float     xstep;    // size every 'column'
    float     zstep;    // size every 'row' 
    float     xoffset;
    float     zoffset;
  };


  //

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  bool InitGridPipeline  (Pipeline&              pipeline,
                          PipelineLayout&        plo,
                          //0
                          const Vec<VkDescriptorSetLayout>&   dslos,
                          const std::filesystem::path& fspath,
                          const VkExtent2D&            viewport_extent, //const rokz::Swapchain& swapchain,
                          VkSampleCountFlagBits        msaa_samples,
                          VkFormat                     color_format,
                          VkFormat                     depth_format,
                          const Device&          device) ; 

  // ----------------------------------------------------------------------------------------------
  // connect descriptors to buffers images samplers etc
  // ----------------------------------------------------------------------------------------------
  bool BindGridDescriptorResources (std::vector<VkDescriptorSet>& dss, const std::vector<rokz::Buffer>& global_uniforms, const rokz::Device& device);
  



  
}


#endif
