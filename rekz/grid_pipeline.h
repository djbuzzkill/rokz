#ifndef REKZ_GRID_PIPELINE

#include "rokz/rokz.h"
#include "rekz.h"

namespace rekz { 

  typedef rekz::Vertex_pos_col    GridVert; 
  typedef rekz::TriMesh<GridVert> GridMesh;

  //bool SetupGridShaderModules (rokz::Pipeline& pipeline, const std::filesystem::path& fspath, const rokz::Device& device); 

  bool SetupGridPipeline (rokz::Pipeline& pipeline,
                          const std::filesystem::path& fspath,
                          const VkExtent2D& viewport_extent, //const rokz::Swapchain& swapchain,
                          VkSampleCountFlagBits msaa_samples,
                          VkFormat color_format,
                          VkFormat depth_format,
                          uint32_t sizeof_push_constants, 
                          const rokz::Device& device);



  
  // bool SetupGridDescriptorSets (rokz::Pipeline&              pipeline,
  //                               const std::vector<rokz::Buffer>&  vma_terr_uniform_buffs,
  //                               const rokz::DescriptorPool&       descpool,
  //                               const rokz::Device&               device); 

  bool SetupGridDescriptorSets (rokz::DescriptorGroup&            dg,
                                const std::vector<rokz::Buffer>&  vma_uniform_buffs, // MVPTransform
                                const rokz::DescriptorSetLayout&  dslo, //const rokz::DescriptorPool& descpool,
                                const rokz::Device&               device);

  
  // 
  bool  SetupGridDescriptorPool (rokz::DescriptorPool& dp, uint32_t num_pools, const rokz::Device& device); 

}


#endif
