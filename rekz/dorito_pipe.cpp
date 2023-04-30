
#include "dorito.h"


using namespace rokz;



bool dorito::InitPipeline (Pipeline&                  pipeline,
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


  pipe_path/"dorito/dorito_comp_vert.vert";
  pipe_path/"dorito/dorito_comp_frag.frag";


  pipe_path/"dorito/dorito_gbuf_frag.frag";
  pipe_path/"dorito/dorito_gbuf_vert.vert";

  return true;
}
  
    // ----------------------------------------------------------------------------------------
    // set 0= Global  descriptors ,  set 1= landscape descriptors
    // ----------------------------------------------------------------------------------------
bool dorito::BindDescriptorResources (Vec<VkDescriptorSet>&      dss,
                                    const DescriptorSetLayout& dslayout, //const rokz::DescriptorPool& descpool,
                                    const Device&              device) {

  assert (false); 
  
  return true; 
}





