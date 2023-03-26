
//
#ifndef REKZ_LANSCAPE_PIPELINE_INCLUDE
#define REKZ_LANSCAPE_PIPELINE_INCLUDE

#include "rekz.h"
#include "rokz/vert_defs.h"
#include <vulkan/vulkan_core.h>

namespace rekz {

  // ----------------------------------------------------------------------------------------------
  // 
  // ----------------------------------------------------------------------------------------------
  namespace landscape { 

    typedef rokz::PNTx_Vert    PatchVert;
    const size_t               kMaxPatchCount = 128; 
    const size_t               kControlPoints = 4; 

    extern const Vec<VkDescriptorSetLayoutBinding>       kDescriptorBindings;
    extern const VkVertexInputBindingDescription&        kVertexInputBindingDesc;
    extern const Vec<VkVertexInputAttributeDescription>& kVertexInputAttributeDesc;


    const VkShaderStageFlags kPCStages = VK_SHADER_STAGE_VERTEX_BIT
                                       | VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT
                                       | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT
                                       | VK_SHADER_STAGE_FRAGMENT_BIT;

    struct PatchPushConstant {
      glm::vec4 position;
      glm::vec4 scale;
      uint32 resource_id;
      uint32 _unused3_id;
      uint32 _unused4_id;
      uint32 _unused5_id;
    };
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
  // this serves no purpose
  // ----------------------------------------------------------------------------------------
  bool SetupLandscapeResources (Buffer& patches_vb, Buffer& patches_ib,
                                uint32_t num_sets, const Device& device);

  // ----------------------------------------------------------------------------------------
  // set 0= Global  descriptors ,  set 1= landscape descriptors
  // ----------------------------------------------------------------------------------------
  bool BindLandscapeResources (Vec<VkDescriptorSet>&           dss,
                               const rc::Sampler::Ref&         colorsamp,
                               const Vec<rc::ImageView::Ref>&  colorviews,

                               const rc::Sampler::Ref&         heightsamp, 
                               const Vec<rc::ImageView::Ref>&  heightviews,

                               const rc::Sampler::Ref&         normalsamp, 
                               const Vec<rc::ImageView::Ref>&  normalviews,

                               const DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                               const Device&              device);
  
}

#endif
