
#include "global_descriptor.h"
#include "shared_descriptor.h"

#include "utility.h"
#include "rc_types.h"
#include "ut_offset.h"
#include "uniform.h"


#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <numeric>
#include <vulkan/vulkan_core.h>

//#include "grid_pipeline.h"


using namespace rokz;
//   typedef struct VkDescriptorSetLayoutBinding {
//     uint32_t              binding;
//     VkDescriptorType      descriptorType;
//     uint32_t              descriptorCount;
//     VkShaderStageFlags    stageFlags;
//     const VkSampler*      pImmutableSamplers;
// } VkDescriptorSetLayoutBinding;


const Vec<VkDescriptorSetLayoutBinding> rokz::kGlobalDescriptorBindings = {
   
  { global_ub::MVP_SCENE_BINDINGI  , VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // <- MVPTransform
  { global_ub::MVP_OVERLAY_BINDINGI, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // <- MVPTransform
  { global_ub::GRIDSTATE_BINDINGI  , VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // <- GridState
  { global_ub::TEXTITEMS_BINDINGI  , VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr }, // <- GridState
};


// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
// bool rokz::SetupGlobalUniforms (Vec<Buffer>& uniform_buffs, uint32_t num_sets, const Device& device) {
//  printf ("%s", __FUNCTION__);


 
//  const size_t sizeOf_GlobalState = sizeof(global_ub::MVPTransform)
//                                  + sizeof(global_ub::MVPTransform)
//                                  + sizeof(global_ub::GridState) 
//                                  + sizeof(global_ub::TextItem);
   
//  uniform_buffs.resize (num_sets);
//  for (size_t i = 0; i < num_sets; i++) {
//   if (!CreateUniformBuffer (uniform_buffs[i], sizeOf_GlobalState, 1, device)) {
//      // pritnf (); 
//      return false; 
//    }
//  }

//  printf (" --> [true] \n"); 
//  return true; 
// }



bool rokz::SetupGlobalUniforms (Vec<rc::Buffer::Ref>& buffs, uint32_t num_sets, const Device& device) {
 printf ("%s", __FUNCTION__);

 const size_t sizeOf_GlobalState = 
   std::accumulate (global_ub::UB_sizes.begin (), global_ub::UB_sizes.end (), 0); 
 
 buffs.resize (num_sets);
 for (size_t i = 0; i < num_sets; i++) {

   buffs[i] = rc::Create_uniform_mapped (sizeOf_GlobalState, 1, device);
   if ( !buffs[i] ) {
     // pritnf (); 
     return false; 
   }
 }

 printf (" --> [true] \n"); 
 return true; 
}

// ----------------------------------------------------------------------------------------------
//                                     RC Ver 
// ----------------------------------------------------------------------------------------------
bool rokz::BindGlobalDescriptorResources (Vec<VkDescriptorSet>& descs, const Vec<rc::Buffer::Ref>& buffs, const rokz::Device& device) {
  // this cqn be used with others
  assert (descs.size () == buffs.size ());

  for (uint32_t i = 0; i < descs.size (); i++) {
    assert (buffs[i]->handle); 
    // wtf does this do
    VkDescriptorBufferInfo binfo_mvps {};
    binfo_mvps.buffer     = buffs[i]->handle;
    binfo_mvps.offset     = ut::offset_at (global_ub::UB_sizes, global_ub::MVP_SCENE_BINDINGI); 
    binfo_mvps.range      = sizeof(global_ub::MVPTransform);

    VkDescriptorBufferInfo binfo_mvpo {};
    binfo_mvpo.buffer     = buffs[i]->handle;
    binfo_mvpo.offset     = ut::offset_at (global_ub::UB_sizes, global_ub::MVP_OVERLAY_BINDINGI); 
    binfo_mvpo.range      = sizeof(global_ub::MVPTransform);
    
    VkDescriptorBufferInfo binfo_grid {};
    binfo_grid.buffer     = buffs[i]->handle;
    binfo_grid.offset     = ut::offset_at (global_ub::UB_sizes, global_ub::GRIDSTATE_BINDINGI); 
    binfo_grid.range      = sizeof(global_ub::GridState);

    VkDescriptorBufferInfo binfo_text {};
    binfo_text.buffer     = buffs[i]->handle;
    binfo_text.offset     = ut::offset_at (global_ub::UB_sizes, global_ub::TEXTITEMS_BINDINGI); 
    binfo_text.range      = sizeof(global_ub::TextItem) * global_ub::kMaxTextElements;

    //
    const size_t num_to_write = 4; 
    std::array<VkWriteDescriptorSet, num_to_write> dws {};
    dws[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    dws[0].pNext            = nullptr;    
    dws[0].dstSet           = descs[i];
    dws[0].dstBinding       = global_ub::MVP_SCENE_BINDINGI;
    dws[0].dstArrayElement  = 0;
    dws[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    dws[0].descriptorCount  = 1;
    dws[0].pBufferInfo      = &binfo_mvps;
    dws[0].pImageInfo       = nullptr;
    dws[0].pTexelBufferView = nullptr;

    dws[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    dws[1].pNext            = nullptr;    
    dws[1].dstSet           = descs[i];
    dws[1].dstBinding       = global_ub::MVP_OVERLAY_BINDINGI; 
    dws[1].dstArrayElement  = 0;
    dws[1].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    dws[1].descriptorCount  = 1;
    dws[1].pBufferInfo      = &binfo_mvpo;
    dws[1].pImageInfo       = nullptr;
    dws[1].pTexelBufferView = nullptr;

    dws[2].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    dws[2].pNext            = nullptr;    
    dws[2].dstSet           = descs[i];
    dws[2].dstBinding       = global_ub::GRIDSTATE_BINDINGI; 
    dws[2].dstArrayElement  = 0;
    dws[2].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    dws[2].descriptorCount  = 1;
    dws[2].pBufferInfo      = &binfo_grid;
    dws[2].pImageInfo       = nullptr;
    dws[2].pTexelBufferView = nullptr;

    dws[3].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    dws[3].pNext            = nullptr;    
    dws[3].dstSet           = descs[i];
    dws[3].dstBinding       = global_ub::TEXTITEMS_BINDINGI; 
    dws[3].dstArrayElement  = 0;
    dws[3].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    dws[3].descriptorCount  = 1;
    dws[3].pBufferInfo      = &binfo_text;
    dws[3].pImageInfo       = nullptr;
    dws[3].pTexelBufferView = nullptr;

    vkUpdateDescriptorSets (device.handle, num_to_write, &dws[0], 0, nullptr);

  }
  
   return true;
}

// ---------------------------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------------------------
void rokz::UpdateGlobals (rokz::DrawSequence::Globals& shared, const rokz::rc::Buffer::Ref buf, const VkExtent2D& viewext, double dt) {
  //
  //  SharedGlobals
  {
    shared.dt             = dt;
    shared.sim_time      += dt;
    shared.viewport_ext   = viewext;
  }    
  
  // MVPTransform buffer
  if (uint8* uc = reinterpret_cast<uint8*>(rokz::rc::MappedPointer (buf))) {
    // ---------------- 3D SCENE MVP ----------------------
    global_ub::MVPTransform* mvps =
      (global_ub::MVPTransform* ) (uc + ut::offset_at (global_ub::UB_sizes, global_ub::MVP_SCENE_BINDINGI));
    
    mvps->model = glm::mat4(1.0); // model is elsewhere 
    const float aspf = ut::ViewAspectRatio (viewext.width, viewext.height);

    glm::mat4 xrot = glm::rotate (glm::mat4(1), shared.view_rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 yrot = glm::rotate (glm::mat4(1), shared.view_rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 zrot = glm::rotate (glm::mat4(1), shared.view_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 rotation =  zrot  * yrot  * xrot;
    glm::mat4 viewmatrix = glm::translate (glm::mat4(1.0f), shared.view_pos) * rotation;
    mvps->view = glm::inverse (viewmatrix); 
    //glm::vec3 (0.0, .5, -5.0));
    // mats.view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    mvps->proj = glm::perspective(glm::radians(60.0f), aspf , 1.0f, 800.0f);
    // !! GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is
    // inverted. The easiest way to compensate for that is to flip the sign on the scaling factor of the Y
    // axis in the projection matrix. If you don't do this, then the image will be rendered upside down.
    mvps->proj[1][1] *= -1;
    // HERE("mvps->proj"); 
    // ut::printmat (mvps->proj);

    // ---------------- OVERLAY MVP ----------------------
    global_ub::MVPTransform* mvpo = (global_ub::MVPTransform*)
      (uc + ut::offset_at (global_ub::UB_sizes, global_ub::MVP_OVERLAY_BINDINGI));

    mvpo->model = glm::mat4(1); 
    mvpo->view  = glm::mat4(1); 

    float lt = 0.0f;
    float rt = viewext.width;

    float tp = viewext.height;
    float bt = 0.0;

    float nr = 0.0f;
    float fr = 1000.0f;

    printf ( " -> ortho [lt:%f, rt:%f, bt:%f, tp:%f, nr:%f, fr:%f]\n", lt, rt, bt, tp, nr, fr );
    //mvpo->proj = glm::orthoRH_ZO (lt, rt, bt, tp, -1.0f, 80.f);
    //mvpo->proj[1][1] *= -1;

    mvpo->proj = glm::orthoRH_ZO (lt, rt, bt, tp, nr, fr);
    mvpo->proj[1][1] *= -1;
    HERE("orthoRH_ZO"); 
    ut::printmat (mvpo->proj);

    glm::mat4 cbmat; 
    cbmat = ut::orthographic_projection (lt, rt, bt, tp , nr, fr);   //mvpo->proj = glm::ortho (lt, rt, bt, tp); 
    HERE("cookbook mat");
    ut::printmat (cbmat);


  }
}
