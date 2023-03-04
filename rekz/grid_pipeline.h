#ifndef REKZ_GRID_PIPELINE

#include "rokz/rokz.h"
#include "rekz.h"

namespace rekz { 


  //
  //
  typedef rekz::Vertex_pos_col    GridVert; 
  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  extern const std::vector<VkDescriptorSetLayoutBinding>      kGridDescriptorBindings;
  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  extern const VkVertexInputBindingDescription                kGridVertexBindingDesc;

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  extern const std::vector<VkVertexInputAttributeDescription> kGridVertInputAttributeDesc;

  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  struct GridPushConstant {

    glm::vec4 x_color;  // 10 is reasonable
    glm::vec4 z_color;  // 10 is reasonable
    glm::vec4 origin_color; 

    float     xstep;    // size every 'column'
    float     zstep;    // size every 'row' 
  };



  //
  // descriptor bindings 
  

  // bind descriptor 
  
  //bool SetupGridShaderModules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device); 

  // bool SetupGridPipeline (rokz::Pipeline& pipeline,
  //                         const std::filesystem::path& fspath,
  //                         const VkExtent2D& viewport_extent, //const rokz::Swapchain& swapchain,
  //                         VkSampleCountFlagBits msaa_samples,
  //                         VkFormat color_format,
  //                         VkFormat depth_format,
  //                         uint32_t sizeof_push_constants, 
  //                         const rokz::Device& device);

  // bool SetupGridDescriptorSets (rokz::Pipeline&              pipeline,
  //                               const std::vector<rokz::Buffer>&  vma_terr_uniform_buffs,
  //                               const rokz::DescriptorPool&       descpool,
  //                               const rokz::Device&               device); 

  // bool SetupGridDescriptorSets (rokz::DescriptorGroup&            dg,
  //                               const std::vector<rokz::Buffer>&  vma_uniform_buffs, // MVPTransform
  //                               const rokz::DescriptorSetLayout&  dslo, //const rokz::DescriptorPool& descpool,
  //                               const rokz::Device&               device);
  // // 
  //bool  SetupGridDescriptorPool (rokz::DescriptorPool& dp, uint32_t num_pools, const rokz::Device& device); 

  // ----------------------------------------------------------------------------------------------
  // setting  up grid uniforms is a very app specific.  (?? wat do u rly want to do
  // ----------------------------------------------------------------------------------------------
  //bool SetupGridUniforms      ;// (std::vector<rokz::Buffer>& uniform_buffs, std::vector<rokz::Buffer>& objparams,




  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  bool InitGridPipeline  (rokz::Pipeline&              pipeline,
                          rokz::PipelineLayout&        plo,
                          rokz::DescriptorSetLayout&   dslo,
                          //0
                          const std::filesystem::path& fspath,
                          const VkExtent2D&            viewport_extent, //const rokz::Swapchain& swapchain,
                          VkSampleCountFlagBits        msaa_samples,
                          VkFormat                     color_format,
                          VkFormat                     depth_format,
                          const rokz::Device&          device) ; 

  // ----------------------------------------------------------------------------------------------
  // connect descriptors to buffers images samplers etc
  // ----------------------------------------------------------------------------------------------
  bool BindGridDescriptorResources (std::vector<VkDescriptorSet>& dss, const std::vector<rokz::Buffer>& global_uniforms, const rokz::Device& device);
  



  
}


#endif
