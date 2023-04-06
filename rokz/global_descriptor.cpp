
#include "global_descriptor.h"
#include "shared_descriptor.h"

#include "utility.h"
#include "rc_types.h"
#include "ut_offset.h"
#include "uniform.h"


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

// const Vec<VkDescriptorSetLayoutBinding> rokz::kGlobalDescriptorBindings = {
   
//   {  0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // <- MVPTransform
//   { 10, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // <- GridState

// };

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
//                                    
// ----------------------------------------------------------------------------------------------
void rokz::CleanupGlobalUniforms (Vec<Buffer>& uniform_buffs, const Device& device) {

  for (auto& ub : uniform_buffs) {
    rokz::cx::Destroy (ub, device.allocator); 
  }

}

// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
// bool rokz::BindGlobalDescriptorResources (Vec<VkDescriptorSet>& descs, const Vec<rokz::Buffer>& buffs, const rokz::Device& device) {

//    printf ("[%i]  %s\n", __LINE__, __FUNCTION__);

//    assert (descs.size () == buffs.size ());

//   for (uint32_t i = 0; i < descs.size (); i++) {
//     // wtf does this do
//     VkDescriptorBufferInfo binfo_mvp {};
//     binfo_mvp.buffer     = buffs[i].handle;
//     binfo_mvp.offset     = 0;
//     binfo_mvp.range      = sizeof(rokz::MVPTransform);

//     VkDescriptorBufferInfo binfo_grid {};
//     binfo_grid.buffer     = buffs[i].handle;
//     binfo_grid.offset     = sizeof(rokz::MVPTransform);
//     binfo_grid.range      = sizeof(rokz::GridState);

//     const uint32_t binding_ind_mvp = 0;
//     const uint32_t binding_ind_grid = 1;
//     //
//     std::array<VkWriteDescriptorSet, 2> descriptor_writes {};
//     descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//     descriptor_writes[0].pNext            = nullptr;    
//     descriptor_writes[0].dstSet           = descs[i];
//     descriptor_writes[0].dstBinding       = kGlobalDescriptorBindings[binding_ind_mvp].binding;
//     descriptor_writes[0].dstArrayElement  = 0;
//     descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//     descriptor_writes[0].descriptorCount  = 1;
//     descriptor_writes[0].pBufferInfo      = &binfo_mvp;
//     descriptor_writes[0].pImageInfo       = nullptr;
//     descriptor_writes[0].pTexelBufferView = nullptr;

//     descriptor_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//     descriptor_writes[1].pNext            = nullptr;    
//     descriptor_writes[1].dstSet           = descs[i];
//     descriptor_writes[1].dstBinding       = kGlobalDescriptorBindings[binding_ind_grid].binding; 
//     descriptor_writes[1].dstArrayElement  = 0;
//     descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//     descriptor_writes[1].descriptorCount  = 1;
//     descriptor_writes[1].pBufferInfo      = &binfo_grid;
//     descriptor_writes[1].pImageInfo       = nullptr;
//     descriptor_writes[1].pTexelBufferView = nullptr;

//     vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);
    

//   }
  
//    return true;
// }

// ----------------------------------------------------------------------------------------------
//                                     RC Ver 
// ----------------------------------------------------------------------------------------------
bool rokz::BindGlobalDescriptorResources (Vec<VkDescriptorSet>& descs, const Vec<rc::Buffer::Ref>& buffs, const rokz::Device& device) {
  
  HERE ("no call this anymore ");
  assert (false); 

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
    binfo_grid.buffer     = buffs[i]->handle;
    binfo_grid.offset     = ut::offset_at (global_ub::UB_sizes, global_ub::TEXTITEMS_BINDINGI); 
    binfo_grid.range      = sizeof(std::array<global_ub::TextItem, global_ub::TextItem::max_length>);

    //
    std::array<VkWriteDescriptorSet, 2> descriptor_writes {};
    descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[0].pNext            = nullptr;    
    descriptor_writes[0].dstSet           = descs[i];
    descriptor_writes[0].dstBinding       = global_ub::MVP_SCENE_BINDINGI;
    descriptor_writes[0].dstArrayElement  = 0;
    descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[0].descriptorCount  = 1;
    descriptor_writes[0].pBufferInfo      = &binfo_mvps;
    descriptor_writes[0].pImageInfo       = nullptr;
    descriptor_writes[0].pTexelBufferView = nullptr;

    descriptor_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[1].pNext            = nullptr;    
    descriptor_writes[1].dstSet           = descs[i];
    descriptor_writes[1].dstBinding       = global_ub::MVP_OVERLAY_BINDINGI; 
    descriptor_writes[1].dstArrayElement  = 0;
    descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[1].descriptorCount  = 1;
    descriptor_writes[1].pBufferInfo      = &binfo_mvpo;
    descriptor_writes[1].pImageInfo       = nullptr;
    descriptor_writes[1].pTexelBufferView = nullptr;


    descriptor_writes[2].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[2].pNext            = nullptr;    
    descriptor_writes[2].dstSet           = descs[i];
    descriptor_writes[2].dstBinding       = global_ub::GRIDSTATE_BINDINGI; 
    descriptor_writes[2].dstArrayElement  = 0;
    descriptor_writes[2].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[2].descriptorCount  = 1;
    descriptor_writes[2].pBufferInfo      = &binfo_grid;
    descriptor_writes[2].pImageInfo       = nullptr;
    descriptor_writes[2].pTexelBufferView = nullptr;


    descriptor_writes[2].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[2].pNext            = nullptr;    
    descriptor_writes[2].dstSet           = descs[i];
    descriptor_writes[2].dstBinding       = global_ub::TEXTITEMS_BINDINGI; 
    descriptor_writes[2].dstArrayElement  = 0;
    descriptor_writes[2].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[2].descriptorCount  = 1;
    descriptor_writes[2].pBufferInfo      = &binfo_text;
    descriptor_writes[2].pImageInfo       = nullptr;
    descriptor_writes[2].pTexelBufferView = nullptr;

    
    vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);
    

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

    // ---------------- OVERLAY MVP ----------------------
    global_ub::MVPTransform* mvpo =
      (global_ub::MVPTransform* ) (uc + ut::offset_at (global_ub::UB_sizes, global_ub::MVP_OVERLAY_BINDINGI));

    mvpo->model = glm::mat4(1); 
    mvpo->view  = glm::mat4(1); 

    float lt = 0.0f;
    float rt = viewext.width;
    float bt = 0.0;
    float tp = viewext.height; 

    mvpo->proj = glm::ortho (lt, rt, bt, tp); 
    mvpo->proj[1][1] *= -1;
  }
}
