
#ifndef REKZ_LANSCAPE_PIPELINE_INCLUDE
#define REKZ_LANSCAPE_PIPELINE_INCLUDE

#include "rekz.h"


namespace rekz {


  const size_t kMaxPatchCount = 256; 
  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  namespace landscape { 
    typedef rekz::Vertex_pos_nrm_txc                     PatchVert;
    // ----------------------------------------------------------------------------------------------
    extern const Vec<VkDescriptorSetLayoutBinding>       kDescriptorBindings;
    // ----------------------------------------------------------------------------------------------
    extern const VkVertexInputBindingDescription&        kVertexInputBindingDesc;
    // ----------------------------------------------------------------------------------------------
    extern const Vec<VkVertexInputAttributeDescription>& kVertexInputAttributeDesc;
  }
  // ----------------------------------------------------------------------------------------
  //                     
  // ----------------------------------------------------------------------------------------
  bool InitLandscapePipeline (Pipeline&                    pipeline,
                              PipelineLayout&              plo,
                              Vec<VkDescriptorSetLayout>&  dslos,
                              VkSampleCountFlagBits        msaa_samples,
                              VkFormat                     color_format,
                              VkFormat                     depth_format,
                              const std::filesystem::path& pipe_path,
                              const VkExtent2D&            displayextent,
                              const Device&                device);
  
  // ----------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------
  bool SetupLandscapeResources (Buffer& patches_vb, Buffer& patches_ib,
                                uint32_t num_sets, const Device& device);

  // ----------------------------------------------------------------------------------------
  // set 0= Global  descriptors ,  set 1= landscape descriptors
  // ----------------------------------------------------------------------------------------
  bool BindLanscapeDescriptors (VkDescriptorSet            ds,
                                const Sampler&             colorsamp,
                                const Vec<VkImageView>&    colorviews,

                                const Sampler&             heightsamp, 
                                const Vec<VkImageView>&    heightviews,

                                const Sampler&             normalsamp, 
                                const Vec<VkImageView>&    normalviews,

                                const DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                const Device&              device);

  
}

#endif