
#include "dust_pipe.h"


using namespace rokz;



bool dust::InitPipeline (Pipeline&                    pipeline,
                         PipelineLayout&              plo,
                         Vec<VkDescriptorSetLayout>&  dslos,
                         VkSampleCountFlagBits        msaa_samples,
                         VkFormat                     color_format,
                         VkFormat                     depth_format,
                         const std::filesystem::path& pipe_path,
                         const VkExtent2D&            displayextent,
                         const Device&                device)
{
  assert (false); 






  return true;
}
  
    // ----------------------------------------------------------------------------------------
    // set 0= Global  descriptors ,  set 1= landscape descriptors
    // ----------------------------------------------------------------------------------------
bool dust::BindDescriptorResources (Vec<VkDescriptorSet>&      dss,
                                    const DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                    const Device&              device) {

  assert (false); 
  
  return true; 
}
