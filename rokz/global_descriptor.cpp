
#include "global_descriptor.h"
#include "utility.h"

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
   
  {  0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // <- MVPTransform
  { 10, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // <- GridState

};

// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
bool rokz::SetupGlobalUniforms (Vec<Buffer>& uniform_buffs, uint32_t num_sets, const Device& device) {
 printf ("%s", __FUNCTION__);

 const size_t sizeOf_GlobalState = sizeof(rokz::MVPTransform) + sizeof (rokz::GridState);
   
 uniform_buffs.resize (num_sets);
 for (size_t i = 0; i < num_sets; i++) {
  if (!CreateUniformBuffer (uniform_buffs[i], sizeOf_GlobalState, 1, device)) {
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
    rokz::Destroy (ub, device.allocator); 
  }

}
// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
bool rokz::BindGlobalDescriptorResources (Vec<VkDescriptorSet>& descs, const Vec<rokz::Buffer>& buffs, const rokz::Device& device) {

   printf ("[%i]  %s\n", __LINE__, __FUNCTION__);

   assert (descs.size () == buffs.size ());

  for (uint32_t i = 0; i < descs.size (); i++) {
    // wtf does this do
    VkDescriptorBufferInfo binfo_mvp {};
    binfo_mvp.buffer     = buffs[i].handle;
    binfo_mvp.offset     = 0;
    binfo_mvp.range      = sizeof(rokz::MVPTransform);

    VkDescriptorBufferInfo binfo_grid {};
    binfo_grid.buffer     = buffs[i].handle;
    binfo_grid.offset     = sizeof(rokz::MVPTransform);
    binfo_grid.range      = sizeof(rokz::GridState);

    const uint32_t binding_ind_mvp = 0;
    const uint32_t binding_ind_grid = 1;
    //
    std::array<VkWriteDescriptorSet, 2> descriptor_writes {};
    descriptor_writes[0].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[0].pNext            = nullptr;    
    descriptor_writes[0].dstSet           = descs[i];
    descriptor_writes[0].dstBinding       = kGlobalDescriptorBindings[binding_ind_mvp].binding;
    descriptor_writes[0].dstArrayElement  = 0;
    descriptor_writes[0].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[0].descriptorCount  = 1;
    descriptor_writes[0].pBufferInfo      = &binfo_mvp;
    descriptor_writes[0].pImageInfo       = nullptr;
    descriptor_writes[0].pTexelBufferView = nullptr;

    descriptor_writes[1].sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_writes[1].pNext            = nullptr;    
    descriptor_writes[1].dstSet           = descs[i];
    descriptor_writes[1].dstBinding       = kGlobalDescriptorBindings[binding_ind_grid].binding; 
    descriptor_writes[1].dstArrayElement  = 0;
    descriptor_writes[1].descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_writes[1].descriptorCount  = 1;
    descriptor_writes[1].pBufferInfo      = &binfo_grid;
    descriptor_writes[1].pImageInfo       = nullptr;
    descriptor_writes[1].pTexelBufferView = nullptr;

    vkUpdateDescriptorSets (device.handle, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);
    

  }
  
   return true;
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
//void UpdateGlobals (Glob& glob, uint32_t current_frame, double dt) {
void rokz::UpdateGlobals (rokz::DrawSequence::Globals& shared, const rokz::Buffer& buf, const VkExtent2D& viewext, double dt) {

  //
  //  SharedGlobals
  {
    shared.dt             = dt;
    shared.sim_time      += dt;
    shared.viewport_ext   = viewext;
  }    
  
  // 
  { // MVPTransform buffer
    rokz::MVPTransform* mvp = reinterpret_cast<rokz::MVPTransform*>(rokz::cx::MappedPointer (buf));
    if (mvp) {
    
      mvp->model = glm::mat4(1.0); // model is elsewhere 
      const float aspf = ut::ViewAspectRatio (viewext.width, viewext.height);

      glm::mat4 xrot = glm::rotate (glm::mat4(1), shared.view_rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
      glm::mat4 yrot = glm::rotate (glm::mat4(1), shared.view_rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
      glm::mat4 zrot = glm::rotate (glm::mat4(1), shared.view_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));

      glm::mat4 rotation =  zrot  * yrot  * xrot;
      glm::mat4 viewmatrix = glm::translate (glm::mat4(1.0f), shared.view_pos) * rotation;
      mvp->view = glm::inverse (viewmatrix); 
      //glm::vec3 (0.0, .5, -5.0));
      // mats.view  = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      
      mvp->proj = glm::perspective(glm::radians(60.0f), aspf , 1.0f, 800.0f);
      // !! GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is
      // inverted. The easiest way to compensate for that is to flip the sign on the scaling factor of the Y
      // axis in the projection matrix. If you don't do this, then the image will be rendered upside down.
      mvp->proj[1][1] *= -1;
      
    }
  }
  
}
