
#ifndef REKZ_LANSCAPE_PIPELINE_INCLUDE
#define REKZ_LANSCAPE_PIPELINE_INCLUDE

#include "rekz.h"


namespace rekz {

  // ----------------------------------------------------------------------------------------
  //                     
  // ----------------------------------------------------------------------------------------
  bool InitLandscapePipeline (Pipeline&                    pipeline,
                              PipelineLayout&              plo,
                              Vec<VkDescriptorSetLayout>&  dslos,
                              VkSampleCountFlagBits        msaa_samples,
                              VkFormat                     color_format,
                              VkFormat                     depth_format,
                              const std::string&           pipe_path,
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
