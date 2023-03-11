
#include "rekz.h"              // 
#include "rekz/grid_pipeline.h"
#include "rokz/buffer.h"
#include "rokz/rokz_types.h"


//#include "rokz/rokz.h"
//
//#include "rokz_test.cpp"
//#include "texture_tool.cpp"
#include <IL/il.h>
#include <IL/ilu.h>
#include <glm/ext/quaternion_common.hpp>



//   typedef struct VkDescriptorSetLayoutBinding {
//     uint32_t              binding;
//     VkDescriptorType      descriptorType;
//     uint32_t              descriptorCount;
//     VkShaderStageFlags    stageFlags;
//     const VkSampler*      pImmutableSamplers;
// } VkDescriptorSetLayoutBinding;

const std::vector<VkDescriptorSetLayoutBinding>  rekz::kGlobalDescriptorBindings = {
   
  {  0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // <- MVPTransform
  { 10, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT  , nullptr }, // <- GridState

};

// ----------------------------------------------------------------------------------------------
//                                    
// ----------------------------------------------------------------------------------------------
bool rekz::SetupGlobalUniforms (std::vector<rokz::Buffer>& uniform_buffs, uint32_t num_sets, const rokz::Device& device) {
 printf ("%s", __FUNCTION__);


 const size_t sizeOf_GlobalState = sizeof(rokz::MVPTransform) + sizeof (rekz::GridState);
   
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
bool rekz::BindGlobalDescriptorResources (std::vector<VkDescriptorSet>& descs, const std::vector<rokz::Buffer>& buffs, const rokz::Device& device) {

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
    binfo_grid.range      = sizeof(rekz::GridState);

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
void rekz::UpdateGlobals (rokz::DrawSequence::Globals& shared, const rokz::Buffer& buf, const VkExtent2D& viewext, double dt) {

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
      const float aspf = rekz::ViewAspectRatio (viewext.width, viewext.height);

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
// ----------------------------------------------------------------------------------------------
// handle most of the common ones
// ----------------------------------------------------------------------------------------------
uint32_t rekz::NumberOfComponents (VkFormat format) {

  switch (format) { 
    // 1 comp
  case VK_FORMAT_R8_UNORM:
  case VK_FORMAT_R8_SNORM :
  case VK_FORMAT_R8_USCALED:
  case VK_FORMAT_R8_SSCALED:
  case VK_FORMAT_R8_UINT:    
  case VK_FORMAT_R8_SINT:    
  case VK_FORMAT_R8_SRGB:    
  case VK_FORMAT_R16_UNORM:
  case VK_FORMAT_R16_SNORM:
  case VK_FORMAT_R16_USCALED:
  case VK_FORMAT_R16_SSCALED:
  case VK_FORMAT_R16_UINT:
  case VK_FORMAT_R16_SINT: 
  case VK_FORMAT_R16_SFLOAT:
  case VK_FORMAT_R32_UINT  :
  case VK_FORMAT_R32_SINT  :
  case VK_FORMAT_R32_SFLOAT:
  case VK_FORMAT_R64_UINT  :    
  case VK_FORMAT_R64_SINT  :    
  case VK_FORMAT_R64_SFLOAT:    
    // sorta
  case VK_FORMAT_D16_UNORM: 
  case VK_FORMAT_D32_SFLOAT: 
  case VK_FORMAT_S8_UINT: 

    return 1;
    break;

    // + 2 components +
  case VK_FORMAT_R4G4_UNORM_PACK8 : // <-- hmmm
  case VK_FORMAT_R8G8_UNORM:
  case VK_FORMAT_R8G8_SNORM:     
  case VK_FORMAT_R8G8_USCALED:
  case VK_FORMAT_R8G8_SSCALED: 
  case VK_FORMAT_R8G8_UINT:      
  case VK_FORMAT_R8G8_SINT:      
  case VK_FORMAT_R8G8_SRGB:      
  case VK_FORMAT_R16G16_UNORM  : 
  case VK_FORMAT_R16G16_SNORM  : 
  case VK_FORMAT_R16G16_USCALED: 
  case VK_FORMAT_R16G16_SSCALED: 
  case VK_FORMAT_R16G16_UINT   :    
  case VK_FORMAT_R16G16_SINT   :    
  case VK_FORMAT_R16G16_SFLOAT :    
  case VK_FORMAT_R32G32_UINT  :
  case VK_FORMAT_R32G32_SINT  :
  case VK_FORMAT_R32G32_SFLOAT:
  case VK_FORMAT_R64G64_UINT  :    
  case VK_FORMAT_R64G64_SINT  :    
  case VK_FORMAT_R64G64_SFLOAT:    
    // sorta
  case VK_FORMAT_D16_UNORM_S8_UINT: 
  case VK_FORMAT_D24_UNORM_S8_UINT:
  case VK_FORMAT_D32_SFLOAT_S8_UINT:  
    // ??? VK_FORMAT_X8_D24_UNORM_PACK32: ???
    return 2;
    break;

   // + 3 components +
  case VK_FORMAT_R5G6B5_UNORM_PACK16:
  case VK_FORMAT_B5G6R5_UNORM_PACK16:   
  case VK_FORMAT_R8G8B8_UNORM  : 
  case VK_FORMAT_R8G8B8_SNORM  : 
  case VK_FORMAT_R8G8B8_USCALED: 
  case VK_FORMAT_R8G8B8_SSCALED:
  case VK_FORMAT_R8G8B8_UINT   :
  case VK_FORMAT_R8G8B8_SINT   :
  case VK_FORMAT_R8G8B8_SRGB   :
  case VK_FORMAT_B8G8R8_UNORM  :
  case VK_FORMAT_B8G8R8_SNORM  :
  case VK_FORMAT_B8G8R8_USCALED: 
  case VK_FORMAT_B8G8R8_SSCALED: 
  case VK_FORMAT_B8G8R8_UINT   :   
  case VK_FORMAT_B8G8R8_SINT   :   
  case VK_FORMAT_B8G8R8_SRGB   :
  case VK_FORMAT_R16G16B16_UNORM  : 
  case VK_FORMAT_R16G16B16_SNORM  :
  case VK_FORMAT_R16G16B16_USCALED:
  case VK_FORMAT_R16G16B16_SSCALED:
  case VK_FORMAT_R16G16B16_UINT   :
  case VK_FORMAT_R16G16B16_SINT   :
  case VK_FORMAT_R16G16B16_SFLOAT :
  case VK_FORMAT_R32G32B32_UINT  :
  case VK_FORMAT_R32G32B32_SINT  :
  case VK_FORMAT_R32G32B32_SFLOAT:
  case VK_FORMAT_R64G64B64_UINT  :    
  case VK_FORMAT_R64G64B64_SINT  :    
  case VK_FORMAT_R64G64B64_SFLOAT:    
  case VK_FORMAT_B10G11R11_UFLOAT_PACK32: 
    return 3;
    break;

   // + 4 components +
  case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
  case VK_FORMAT_B4G4R4A4_UNORM_PACK16: 
  case VK_FORMAT_R5G5B5A1_UNORM_PACK16: 
  case VK_FORMAT_B5G5R5A1_UNORM_PACK16: 
  case VK_FORMAT_A1R5G5B5_UNORM_PACK16: 
  case VK_FORMAT_R8G8B8A8_UNORM:   
  case VK_FORMAT_R8G8B8A8_SNORM:   
  case VK_FORMAT_R8G8B8A8_USCALED: 
  case VK_FORMAT_R8G8B8A8_SSCALED: 
  case VK_FORMAT_R8G8B8A8_UINT   : 
  case VK_FORMAT_R8G8B8A8_SINT   : 
  case VK_FORMAT_R8G8B8A8_SRGB   : 
  case VK_FORMAT_B8G8R8A8_UNORM  : 
  case VK_FORMAT_B8G8R8A8_SNORM  : 
  case VK_FORMAT_B8G8R8A8_USCALED: 
  case VK_FORMAT_B8G8R8A8_SSCALED: 
  case VK_FORMAT_B8G8R8A8_UINT:
  case VK_FORMAT_B8G8R8A8_SINT:
  case VK_FORMAT_B8G8R8A8_SRGB:
  case VK_FORMAT_A8B8G8R8_UNORM_PACK32:  
  case VK_FORMAT_A8B8G8R8_SNORM_PACK32:  
  case VK_FORMAT_A8B8G8R8_USCALED_PACK32:  
  case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:  
  case VK_FORMAT_A8B8G8R8_UINT_PACK32   :  
  case VK_FORMAT_A8B8G8R8_SINT_PACK32   :  
  case VK_FORMAT_A8B8G8R8_SRGB_PACK32: 
  case VK_FORMAT_A2R10G10B10_UNORM_PACK32  :
  case VK_FORMAT_A2R10G10B10_SNORM_PACK32  :
  case VK_FORMAT_A2R10G10B10_USCALED_PACK32: 
  case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: 
  case VK_FORMAT_A2R10G10B10_UINT_PACK32   : 
  case VK_FORMAT_A2R10G10B10_SINT_PACK32   : 
  case VK_FORMAT_A2B10G10R10_UNORM_PACK32  : 
  case VK_FORMAT_A2B10G10R10_SNORM_PACK32  : 
  case VK_FORMAT_A2B10G10R10_USCALED_PACK32: 
  case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: 
  case VK_FORMAT_A2B10G10R10_UINT_PACK32   : 
  case VK_FORMAT_A2B10G10R10_SINT_PACK32   : 
  case VK_FORMAT_R16G16B16A16_UNORM :  
  case VK_FORMAT_R16G16B16A16_SNORM :  
  case VK_FORMAT_R16G16B16A16_USCALED: 
  case VK_FORMAT_R16G16B16A16_SSCALED: 
  case VK_FORMAT_R16G16B16A16_UINT   : 
  case VK_FORMAT_R16G16B16A16_SINT   : 
  case VK_FORMAT_R16G16B16A16_SFLOAT : 
  case VK_FORMAT_R32G32B32A32_UINT   :
  case VK_FORMAT_R32G32B32A32_SINT   :
  case VK_FORMAT_R32G32B32A32_SFLOAT :
  case VK_FORMAT_R64G64B64A64_UINT   :    
  case VK_FORMAT_R64G64B64A64_SINT  :  
  case VK_FORMAT_R64G64B64A64_SFLOAT: 
    return 4;
    break;

  default:
    // ?? case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32 : 
    printf ( "%s [WARNING] --> unhandled format %u\n", __FUNCTION__, format);
    return 0;
    break;
  }

  return 0;
}


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
uint32_t rekz::SizeOfComponents (VkFormat format) {
  
  switch (format) { 
    // 1 byte component
  case VK_FORMAT_R8_UNORM:
  case VK_FORMAT_R8_SNORM :
  case VK_FORMAT_R8_USCALED:
  case VK_FORMAT_R8_SSCALED:
  case VK_FORMAT_R8_UINT:    
  case VK_FORMAT_R8_SINT:    
  case VK_FORMAT_R8_SRGB:    
  case VK_FORMAT_S8_UINT: 

  case VK_FORMAT_R8G8_UNORM:
  case VK_FORMAT_R8G8_SNORM:     
  case VK_FORMAT_R8G8_USCALED:
  case VK_FORMAT_R8G8_SSCALED: 
  case VK_FORMAT_R8G8_UINT:      
  case VK_FORMAT_R8G8_SINT:      
  case VK_FORMAT_R8G8_SRGB:      
    
  case VK_FORMAT_R8G8B8_UNORM  : 
  case VK_FORMAT_R8G8B8_SNORM  : 
  case VK_FORMAT_R8G8B8_USCALED: 
  case VK_FORMAT_R8G8B8_SSCALED:
  case VK_FORMAT_R8G8B8_UINT   :
  case VK_FORMAT_R8G8B8_SINT   :
  case VK_FORMAT_R8G8B8_SRGB   :
  case VK_FORMAT_B8G8R8_UNORM  :
  case VK_FORMAT_B8G8R8_SNORM  :
  case VK_FORMAT_B8G8R8_USCALED: 
  case VK_FORMAT_B8G8R8_SSCALED: 
  case VK_FORMAT_B8G8R8_UINT   :   
  case VK_FORMAT_B8G8R8_SINT   :   
  case VK_FORMAT_B8G8R8_SRGB   :

  case VK_FORMAT_R8G8B8A8_UNORM:   
  case VK_FORMAT_R8G8B8A8_SNORM:   
  case VK_FORMAT_R8G8B8A8_USCALED: 
  case VK_FORMAT_R8G8B8A8_SSCALED: 
  case VK_FORMAT_R8G8B8A8_UINT   : 
  case VK_FORMAT_R8G8B8A8_SINT   : 
  case VK_FORMAT_R8G8B8A8_SRGB   : 
  case VK_FORMAT_B8G8R8A8_UNORM  : 
  case VK_FORMAT_B8G8R8A8_SNORM  : 
  case VK_FORMAT_B8G8R8A8_USCALED: 
  case VK_FORMAT_B8G8R8A8_SSCALED: 
  case VK_FORMAT_B8G8R8A8_UINT:
  case VK_FORMAT_B8G8R8A8_SINT:
  case VK_FORMAT_B8G8R8A8_SRGB:
  case VK_FORMAT_A8B8G8R8_UNORM_PACK32:  
  case VK_FORMAT_A8B8G8R8_SNORM_PACK32:  
  case VK_FORMAT_A8B8G8R8_USCALED_PACK32:  
  case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:  
  case VK_FORMAT_A8B8G8R8_UINT_PACK32   :  
  case VK_FORMAT_A8B8G8R8_SINT_PACK32   :  
  case VK_FORMAT_A8B8G8R8_SRGB_PACK32: 

    return 1;
    break;

    // + 2 bytes +
  case VK_FORMAT_R16_UNORM:
  case VK_FORMAT_R16_SNORM:
  case VK_FORMAT_R16_USCALED:
  case VK_FORMAT_R16_SSCALED:
  case VK_FORMAT_R16_UINT:
  case VK_FORMAT_R16_SINT: 
  case VK_FORMAT_R16_SFLOAT:
  case VK_FORMAT_D16_UNORM: 
  case VK_FORMAT_R16G16_UNORM  : 
  case VK_FORMAT_R16G16_SNORM  : 
  case VK_FORMAT_R16G16_USCALED: 
  case VK_FORMAT_R16G16_SSCALED: 
  case VK_FORMAT_R16G16_UINT   :    
  case VK_FORMAT_R16G16_SINT   :    
  case VK_FORMAT_R16G16_SFLOAT :    

  case VK_FORMAT_R16G16B16_UNORM  : 
  case VK_FORMAT_R16G16B16_SNORM  :
  case VK_FORMAT_R16G16B16_USCALED:
  case VK_FORMAT_R16G16B16_SSCALED:
  case VK_FORMAT_R16G16B16_UINT   :
  case VK_FORMAT_R16G16B16_SINT   :
  case VK_FORMAT_R16G16B16_SFLOAT :

  case VK_FORMAT_R16G16B16A16_UNORM :  
  case VK_FORMAT_R16G16B16A16_SNORM :  
  case VK_FORMAT_R16G16B16A16_USCALED: 
  case VK_FORMAT_R16G16B16A16_SSCALED: 
  case VK_FORMAT_R16G16B16A16_UINT   : 
  case VK_FORMAT_R16G16B16A16_SINT   : 
  case VK_FORMAT_R16G16B16A16_SFLOAT : 
      
    return 2;
    break;


    //  4 bytes
  case VK_FORMAT_R32_UINT  :
  case VK_FORMAT_R32_SINT  :
  case VK_FORMAT_R32_SFLOAT:
  case VK_FORMAT_D32_SFLOAT: 

  case VK_FORMAT_R32G32_UINT  :
  case VK_FORMAT_R32G32_SINT  :
  case VK_FORMAT_R32G32_SFLOAT:

  case VK_FORMAT_R32G32B32_UINT  :
  case VK_FORMAT_R32G32B32_SINT  :
  case VK_FORMAT_R32G32B32_SFLOAT:

  case VK_FORMAT_R32G32B32A32_UINT   :
  case VK_FORMAT_R32G32B32A32_SINT   :
  case VK_FORMAT_R32G32B32A32_SFLOAT :

    return 4;
    break;


    //case VK_FORMAT_R4G4_UNORM_PACK8 : // <-- hmmm
  case VK_FORMAT_R64_UINT  :    
  case VK_FORMAT_R64_SINT  :    
  case VK_FORMAT_R64_SFLOAT:    
  case VK_FORMAT_R64G64_UINT  :    
  case VK_FORMAT_R64G64_SINT  :    
  case VK_FORMAT_R64G64_SFLOAT:
  case VK_FORMAT_R64G64B64_UINT  :    
  case VK_FORMAT_R64G64B64_SINT  :    
  case VK_FORMAT_R64G64B64_SFLOAT:    
  case VK_FORMAT_R64G64B64A64_UINT:    
  case VK_FORMAT_R64G64B64A64_SINT:  
  case VK_FORMAT_R64G64B64A64_SFLOAT: 

    return 8;
    break;

    // + WAT TODO WITH THESE +
    
  // case VK_FORMAT_D16_UNORM_S8_UINT: 
  // case VK_FORMAT_D24_UNORM_S8_UINT:
  // case VK_FORMAT_D32_SFLOAT_S8_UINT:  
  // ??? VK_FORMAT_X8_D24_UNORM_PACK32: ???
  // case VK_FORMAT_R5G6B5_UNORM_PACK16:
  // case VK_FORMAT_B5G6R5_UNORM_PACK16:   
  // case VK_FORMAT_B10G11R11_UFLOAT_PACK32: 
  // case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
  // case VK_FORMAT_B4G4R4A4_UNORM_PACK16: 
  // case VK_FORMAT_R5G5B5A1_UNORM_PACK16: 
  // case VK_FORMAT_B5G5R5A1_UNORM_PACK16: 
  // case VK_FORMAT_A1R5G5B5_UNORM_PACK16: 
  // case VK_FORMAT_A2R10G10B10_UNORM_PACK32  :
  // case VK_FORMAT_A2R10G10B10_SNORM_PACK32  :
  // case VK_FORMAT_A2R10G10B10_USCALED_PACK32: 
  // case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: 
  // case VK_FORMAT_A2R10G10B10_UINT_PACK32   : 
  // case VK_FORMAT_A2R10G10B10_SINT_PACK32   : 
  // case VK_FORMAT_A2B10G10R10_UNORM_PACK32  : 
  // case VK_FORMAT_A2B10G10R10_SNORM_PACK32  : 
  // case VK_FORMAT_A2B10G10R10_USCALED_PACK32: 
  // case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: 
  // case VK_FORMAT_A2B10G10R10_UINT_PACK32   : 
  // case VK_FORMAT_A2B10G10R10_SINT_PACK32   : 
  // case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32 : 


  default:
    printf ( "%s [WARNING] --> unhandled format %u\n", __FUNCTION__, format);
    return 0;
    break;
  }

  return 0;

}


// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
int rekz::OpenImageFile (const std::string& fqname, rekz::DevILOpenFileCB cb, void* up) {

  DevILImageProps props; 

  ilInit ();
  ilBindImage (ilGenImage ());

  int res = 0;
  if (ilLoadImage(fqname.c_str())) {
    
    printf ("Opened [%s]\n", fqname.c_str() ); 
    props.width    = ilGetInteger (IL_IMAGE_WIDTH); 
    props.height   = ilGetInteger (IL_IMAGE_HEIGHT);
    props.depth    = ilGetInteger (IL_IMAGE_DEPTH);
    props.bytes_per_pixel= ilGetInteger (IL_IMAGE_BYTES_PER_PIXEL); 
    props.bpp      = ilGetInteger (IL_IMAGE_BPP);
    props.type     = ilGetInteger (IL_IMAGE_TYPE);
    props.format   = ilGetInteger (IL_IMAGE_FORMAT);


    res = cb (ilGetData (), props, up); 

    ilDeleteImage (ilGetInteger (IL_ACTIVE_IMAGE)); 

  }

  ilShutDown ();
  return res; 
}

#ifdef REKZ_HIDE_CPP_IMAGE_HANDLER
// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
int cpp_image_handler (const unsigned char* dat, const rekz::DevILImageProps& props, void* up) {

  if (up) {
    rekz::ImageCB* cb =  static_cast <rekz::ImageCB*> (up); 
    return cb->do_shit (dat, props);
  }

  printf ("[ERROR] %s..bad user pointer\n", __FUNCTION__); 
  return __LINE__; 
}

// ---------------------------------------------------------------------
// 
// ---------------------------------------------------------------------
int rekz::OpenImageFile (const std::string& fqname, ImageCB* cb) {

  return OpenImageFile (fqname, cpp_image_handler, (void*) cb); 
  
}
#endif


// ---------------------------------------------------------------------
// load texture to device memory
// ---------------------------------------------------------------------
bool rekz::LoadTexture_color_sampling (rokz::Image&             image,
                                           VkFormat                 format,
                                           const VkExtent2D&        ext2d,
                                           const void*              srcimage,
                                           const VmaAllocator&      allocator, 
                                           const VkQueue&           queue, 
                                           const rokz::CommandPool& commandpool, 
                                           const rokz::Device&      device) {

  //size_t image_size = image_width * image_height *  bytes_per_pixel; 
  auto image_size = SizeOfComponents (format)
                  * NumberOfComponents (format)
                  * ext2d.width * ext2d.height;
  assert (image_size); 

  rokz::Buffer stage_buff; 
  
  rokz::cx::CreateInfo_buffer_stage (stage_buff.ci, image_size);
  rokz::cx::AllocCreateInfo_stage (stage_buff.alloc_ci);
  rokz::cx::CreateBuffer (stage_buff, allocator); 

  void* mapped = nullptr; 
  if (rokz::cx::MapMemory (&mapped, stage_buff.allocation, allocator)) { 
  
    const uint8_t* image_data = reinterpret_cast<const unsigned char*> (srcimage); 
    std::copy (image_data, image_data + image_size, reinterpret_cast<uint8_t*> (mapped));
  }
  rokz::cx::UnmapMemory (stage_buff.allocation, allocator);

  rokz::cx::CreateInfo_2D_color_sampling  (image.ci, VK_SAMPLE_COUNT_1_BIT, ext2d.width, ext2d.height);
  rokz::cx::AllocCreateInfo_device (image.alloc_ci);
  if (!rokz::cx::CreateImage (image, allocator)) {
    printf ("[FAILED] %s setup test texture", __FUNCTION__);
    return false;
  }

  //VK_FORMAT_R8G8B8A8_SRGB
  rokz::cx::TransitionImageLayout (image.handle, format, VK_IMAGE_LAYOUT_UNDEFINED,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               queue, commandpool.handle, device.handle);

  rokz::cx::CopyBufferToImage (image.handle, stage_buff.handle, ext2d.width, ext2d.height,
                           queue, commandpool.handle, device.handle);

  rokz::cx::TransitionImageLayout (image.handle, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                               queue, commandpool.handle, device.handle);

  rokz::Destroy (stage_buff, allocator); 
  return true; 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool rekz::CreateDepthBufferTarget (rokz::Image&          depth_image,
                                    rokz::ImageView&      depth_imageview,
                                    //rokz::SwapchainGroup& scg,
                                    VkSampleCountFlagBits msaa_samples, 
                                    VkFormat              depth_format,
                                    const VkExtent2D&     ext,
                                    const rokz::Device&   device)
{
  printf ("%s\n", __FUNCTION__); 

  //rokz::SwapchainGroup& scg = glob.swapchain_group;
  
  //
  // uint32_t wd = scg.swapchain.ci.imageExtent.width; 
  // uint32_t ht = scg.swapchain.ci.imageExtent.height;   

  rokz::cx::CreateInfo_2D_depthstencil (depth_image.ci,
                                        depth_format, 
                                        msaa_samples,
                                        ext.width, ext.height);

  rokz::cx::AllocCreateInfo_device (depth_image.alloc_ci); 
  rokz::cx::CreateImage (depth_image, device.allocator.handle);

  rokz::cx::CreateInfo (depth_imageview.ci, VK_IMAGE_ASPECT_DEPTH_BIT, depth_image); 
  rokz::cx::CreateImageView (depth_imageview, depth_imageview.ci, device.handle);

  // manual transition b/c dynamic_rendering
  rokz::cx::TransitionImageLayout (depth_image.handle, depth_format,
                                   VK_IMAGE_LAYOUT_UNDEFINED,
                                   VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                   device.queues.graphics, device.command_pool.handle, device.handle);

  return true;
}


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
bool rekz::CreateMSAAColorTarget  (rokz::Image&          color_image, 
                             rokz::ImageView&      color_imageview, 
                             VkSampleCountFlagBits msaa_samples,
                             VkFormat              image_format,
                             const VkExtent2D&     ext,
                             const rokz::Device&   device) {
  printf ("%s\n", __FUNCTION__); 
  rokz::cx::CreateInfo_2D_color_target (color_image.ci, image_format, msaa_samples, ext.width, ext.height);

  rokz::cx::AllocCreateInfo_device (color_image.alloc_ci);
  rokz::cx::CreateImage (color_image, device.allocator.handle);

  // imageview 
  rokz::cx::CreateInfo (color_imageview.ci, VK_IMAGE_ASPECT_COLOR_BIT, color_image);
  rokz::cx::CreateImageView (color_imageview, color_imageview.ci, device.handle);
  // dynamic_rendering
  rokz::cx::TransitionImageLayout (color_image.handle, image_format, VK_IMAGE_LAYOUT_UNDEFINED,
                                   VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, device.queues.graphics,
                                   device.command_pool.handle, device.handle);
  return true;
}

// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
bool rekz::SetupGridData (GridData& gd, const rokz::Device& device) {

  void*  mem = nullptr;
  size_t szmem = 0;

  const uint16_t vertdim    = 11;
  const uint16_t totalverts = vertdim * vertdim;
  const float    dimsize    = 20.0f;
  const float    dimstep    = float (dimsize) / float (vertdim - 1);

  glm::vec3 zaxis_co  (1.0, 0.4, 0.8);
  glm::vec3 xaxis_co  (0.3, 0.5, 1.0);
  glm::vec3 origin_co (0.0, 0.4, 0.6);

  float dx = 1.0f / float (vertdim);
  
  const glm::vec3 voffs (-dimsize * 0.5f, 0.0f, -dimsize * 0.5f);

  std::vector<rekz::GridVert> verts (vertdim * vertdim);
  std::vector<uint16_t> inds (2 * totalverts);

  // -- vertices --
  for (uint16_t iz = 0; iz < vertdim; ++iz) {
    for (uint16_t ix = 0; ix < vertdim; ++ix) {
      verts[iz * vertdim + ix].pos = glm::vec3 (ix * dimstep, 0.0f, iz * dimstep) + voffs;
      verts[iz * vertdim + ix].col = glm::mix (origin_co, zaxis_co, ix * dx); 

    }
  } // move 2 vb
  rokz::Create_VB_device ( gd.vb_device, &verts[0], verts.size () * sizeof(rekz::GridVert), device); 

  // -- indices --
  for (uint16_t iz = 0; iz < vertdim; ++iz) { // draw x lines
    for (uint16_t ix = 0; ix < vertdim; ++ix) { 
      inds[iz * vertdim + ix] = iz * vertdim + ix;   
    }}

  for (uint32_t ix = 0; ix < vertdim; ++ix) {  // draw z lines
    for (uint32_t iz = 0; iz < vertdim; ++iz) { 
      inds[totalverts + ix * vertdim + iz] = iz * vertdim + ix; 
    }
  } // move
  rokz::Create_IB_16_device (gd.ib_device, &inds[0], inds.size (), device); 
  
  return true; 
}


// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
void rekz::CleanupGridData (GridData& gd, const rokz::Device& device) {

  rokz::Destroy (gd.vb_device, device.allocator.handle); 
  
}


// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
glm::vec3& rekz::unit_angle_xz (glm::vec3& v, float theta) {
  v.x = cos (theta) ;
  v.y = 0.0f;
  v.z = -sinf (theta) ;
  return v; 
}

glm::vec3& rekz::unit_angle_yz (glm::vec3& v, float theta) {
  v.x = 0.0;
  v.y = cos (theta) ;
  v.z = -sinf (theta) ;
  return v; 
}


// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
float rekz::ViewAspectRatio (uint32_t w, uint32_t h) {
  return   float (w) / float (h) ;
}  

//
float rekz::AspectRatio (const VkExtent2D& ext) {

  return ViewAspectRatio (ext.width, ext.height); 

}

bool rekz::SetupDisplay (rokz::Display& display, rekz::InputState& input_state, const VkExtent2D& dim, const rokz::Instance& instance) { 
  
  // create GLFW window
  rokz::CreateWindow (display.window, dim.width, dim.height, "wut"); 
  glfwSetFramebufferSizeCallback (display.window.glfw_window, rekz::win_event::on_resize ); 
  glfwSetKeyCallback (display.window.glfw_window, rekz::win_event::on_keypress);
  glfwSetCursorPosCallback(display.window.glfw_window, rekz::win_event::on_mouse_move);
  glfwSetMouseButtonCallback(display.window.glfw_window, rekz::win_event::on_mouse_button);
  glfwSetCursorEnterCallback (display.window.glfw_window, rekz::win_event::on_mouse_enter); 
                              
  glfwSetWindowUserPointer (display.window.glfw_window, &input_state);

  // create surface
  return  rokz::cx::CreateSurface  (&display.surface, display.window.glfw_window, instance.handle);

}



// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//bool SetupRenderingAttachments (Glob& glob, rokz::SwapchainGroup& scg, const rokz::Device& device) { 
bool rekz::SetupRenderingAttachments (rokz::Image&          msaa_color_image       ,
                                      rokz::ImageView&      msaa_color_imageview   ,

                                      rokz::Image&          msaa_depth_image       ,
                                      rokz::ImageView&      msaa_depth_imageview   ,

                                      VkSampleCountFlagBits msaa_samples           ,
                                      VkFormat              swapchain_image_format ,
                                      VkFormat              msaa_depth_format      ,
                                      const VkExtent2D&     image_ext, 
                                      const rokz::Device& device) { 

  //CreateMSAAColorImage -> (image, imageview)
  CreateMSAAColorTarget (msaa_color_image, msaa_color_imageview, msaa_samples,
                               swapchain_image_format, image_ext, device);

  // CreateDepthBufferImage -> (image, imageview)
  CreateDepthBufferTarget (msaa_depth_image, msaa_depth_imageview, msaa_samples,
                                 msaa_depth_format, image_ext, device);

  return true;

}



// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
void rekz::CleanupSwapchain (std::vector<rokz::ImageView>& sc_image_views,
                                 rokz::Image&                  msaa_color_image,
                                 rokz::ImageView&              msaa_color_imageview,

                                 rokz::Image&                  depth_image,
                                 rokz::ImageView&              depth_imageview,

                                 rokz::Swapchain&              swapchain,
                                 const rokz::Device&           device,
                                 const VmaAllocator&           allocator) {

  // for (auto fb : framebuffers) {
  //   vkDestroyFramebuffer (device.handle, fb.handle, nullptr); 
  // }

  for (auto sc_imageview : sc_image_views) {
    vkDestroyImageView(device.handle, sc_imageview.handle, nullptr);
  }

  rokz::cx::Destroy (msaa_color_image, allocator);
  rokz::cx::Destroy (msaa_color_imageview, device.handle);

  rokz::cx::Destroy (depth_image, allocator);
  rokz::cx::Destroy (depth_imageview, device.handle);

  vkDestroySwapchainKHR(device.handle, swapchain.handle, nullptr);
}


// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
bool rekz::RecreateSwapchain(rokz::Swapchain&  swapchain, const rokz::Window& win, 
                             std::vector<rokz::Image>& swapchain_images, std::vector<rokz::ImageView>& imageviews,
                             rokz::Image& depth_image, rokz::ImageView& depth_imageview,
                             rokz::Image& multisamp_color_image, rokz::ImageView& multisamp_color_imageview,
                             const VmaAllocator& allocator, const rokz::Device& device) {

  printf ("%s\n", __FUNCTION__);

  int width = 0, height = 0;
  glfwGetFramebufferSize(win.glfw_window, &width, &height);

  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(win.glfw_window, &width, &height);
    glfwWaitEvents();
  }
  
  vkDeviceWaitIdle (device.handle);

  CleanupSwapchain (imageviews,
                    depth_image, depth_imageview,
                    multisamp_color_image,
                    multisamp_color_imageview,
                    swapchain, device, allocator);

  //CreateInfo_default (swapchain.ci, surf, extent, swapchain_support_info, 
  bool swapchain_res    = rokz::cx::CreateSwapchain (swapchain, device);
  bool imageviews_res   = rokz::cx::CreateImageViews (imageviews, swapchain_images, device);

  assert (false);
  // rokz::Image& multisamp_color_image, rokz::ImageView& multisamp_color_imageview,

  assert (false);
  //rokz::Image& depth_image, rokz::ImageView& depth_imageview,

  return (swapchain_res && imageviews_res); 
}

// -------------------------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------------------------
rokz::ResetSwapchainCB::Ref rekz::CreateSwapchainResetter (rokz::Swapchain& sc, 
                                                           std::vector<rokz::Image>& scis, std::vector<rokz::ImageView>& scivs,
                                                           rokz::Image& dp, rokz::ImageView& div,
                                                           rokz::Image& mscim, rokz::ImageView& mscimv) {

  struct DefaultResetSwapchain : public rokz::ResetSwapchainCB {
  public:
  
    DefaultResetSwapchain (rokz::Swapchain& sc, 
                           std::vector<rokz::Image>& scis, std::vector<rokz::ImageView>& scivs,
                           rokz::Image& dp, rokz::ImageView& dpiv,  
                           rokz::Image& mscim, rokz::ImageView&  mscimv)
      : ResetSwapchainCB ()
      , swapchain (sc)
      , swapchain_images (scis)
      , swapchain_imageviews (scivs)
      , depth_image (dp)
      , depth_imageview(dpiv)
      , msaa_color_image(mscim)
      , msaa_color_imageview(mscimv) { 
    }
    
    //
    virtual bool ResetSwapchain (const rokz::Window& win, const rokz::Allocator& allocator,  const rokz::Device& device) {
      return rekz::RecreateSwapchain (swapchain, win, 
                                      swapchain_images, swapchain_imageviews,
                                      depth_image,      depth_imageview,  //glob.depth_image, glob.depth_imageview,
                                      msaa_color_image, msaa_color_imageview,
                                      allocator.handle, device);
    }
    
  protected:
    
    rokz::Swapchain&              swapchain;
    std::vector<rokz::Image>&     swapchain_images;
    std::vector<rokz::ImageView>& swapchain_imageviews;
    
    rokz::Image&                  depth_image;
    rokz::ImageView&              depth_imageview;  //
    rokz::Image&                  msaa_color_image;
    rokz::ImageView&              msaa_color_imageview; 
  };


  return std::make_shared<DefaultResetSwapchain> (sc, scis, scivs, dp, div, mscim, mscimv); 
}

// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool rekz::SetupDynamicRenderingInfo (rokz::RenderingInfoGroup& ri,
                                const rokz::ImageView&    msaa_color_imageview ,
                                const rokz::ImageView&    msaa_depth_imageview ,
                                const VkExtent2D&         image_extent) {

  // const rokz::ImageView& msaa_color_imageview = glob.msaa_color_imageview;
  // const rokz::ImageView& msaa_depth_imageview = glob.depth_imageview;
  // const VkExtent2D& image_extent              = glob.swapchain_group.swapchain.ci.imageExtent;
  ri.clear_colors.resize (1);
  ri.color_attachment_infos.resize (1);

  ri.clear_colors[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  //rig.clear_colors[1].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  ri.clear_depth.depthStencil = {1.0f, 0};

  rokz::cx::AttachmentInfo (ri.color_attachment_infos[0],
                            msaa_color_imageview.handle,
                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                            VK_RESOLVE_MODE_AVERAGE_BIT,
                            VK_NULL_HANDLE, // swapchain_group.imageviews[i].handle <-- this will change
                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                            VK_ATTACHMENT_STORE_OP_STORE,
                            ri.clear_colors[0]);

  rokz::cx::AttachmentInfo (ri.depth_attachment_info,
                            msaa_depth_imageview.handle,
                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                            VK_RESOLVE_MODE_NONE,
                            nullptr,
                            VK_IMAGE_LAYOUT_UNDEFINED,
                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                            ri.clear_depth);

  
  ri.render_area = { VkOffset2D {0, 0}, image_extent };

  rokz::cx::RenderingInfo (ri.ri, ri.render_area, 1, 0, ri.color_attachment_infos, &ri.depth_attachment_info, nullptr);
  return true;
}

// -----------------------------------------------------------------------------
// basically populates the AttachmentInfo
// -----------------------------------------------------------------------------
void rekz::UpdateDynamicRenderingInfo (rokz::RenderingInfoGroup& ri,
                                       const rokz::ImageView&    msaa_color_imageview ,
                                       const rokz::ImageView&    target_imageview) {
  //printf ("%s\n", __FUNCTION__); 
  rokz::cx::AttachmentInfo (ri.color_attachment_infos[0],
                        msaa_color_imageview.handle, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                        VK_RESOLVE_MODE_AVERAGE_BIT, target_imageview.handle,
                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR,
                        VK_ATTACHMENT_STORE_OP_STORE, ri.clear_colors[0]);

}



// rokz::cx::QuerySwapchainSupport (glob.swapchain_support_info,
  //                              glob.surface,
  //                              glob.physical_device.handle);

  //rokz::SwapchainGroup& scg = glob.swapchain_group;
// -------------------------------------------------------------------------------------------
//                                             
// -------------------------------------------------------------------------------------------
int test_rokz (const std::vector<std::string>& args); 
int test_rokz_hpp (const std::vector<std::string>& args); 
int texture_tool (const std::vector<std::string>& args); 
int test_ouput (const std::vector<std::string>& args); 
int test_time (); 
int darkrootbasin (const std::vector<std::string>& args);
int run_marz (const std::vector<std::string>& args);
int mars_prelim (const std::vector<std::string>& args);
bool test_grid_geom_gen ();


// -------------------------------------------------------------------------------------------
int main (int argv, char** argc) {

  const std::vector<std::string> args (argc, argc + argv);

  darkrootbasin  (args);
  //marz_run  (args);

  // test_rokz_hpp (args); 
  // texture_tool (args); 
  //test_ouput (args);
  //test_grid_geom_gen ();
  //test_time (); 
  //test_rokz (args); 
  
  printf ("lolz bai %s\n", __FUNCTION__); 
  return 0; 
}
