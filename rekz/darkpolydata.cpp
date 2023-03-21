
#include "darkrootgarden.h"
#include "rekz/dark_types.h"

#include "image_loader.h"
#include <vulkan/vulkan_core.h>


using namespace darkroot;


// -------------------------------------------------------------------------
// UP for  obj_image_handler 
// -------------------------------------------------------------------------
struct obj_params {
  
  obj_params (const rokz::Device& dev, struct rekz::PolygonData& d) : device (dev), poly (d) {
  }
  
  const rokz::Device& device;
  rekz::PolygonData& poly;
};
// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------

struct obj_image_handler : public rekz::DevILOpenFileCB {

  obj_image_handler (obj_params& p): params (p) {
  }

  obj_params& params;
  
  virtual int Exec (const unsigned char* dat, const rekz::DevILImageProps& props) {

  rekz::PolygonData& polyd  = params.poly;

  VkImageCreateInfo     ci {};
  


  polyd.texture = rekz::LoadTexture_color_sampling (VK_FORMAT_R8G8B8A8_SRGB, VkExtent2D { (uint32_t) props.width, (uint32_t) props.height },
                                                    dat, params.device.allocator.handle, params.device.queues.graphics, 
                                                    params.device.command_pool, params.device);
  
  if (polyd.texture) {
    return 0;
  }
  
  
  return __LINE__;
  }};

// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
bool setup_object_texture_and_sampler (rekz::PolygonData& polyd, const std::string& data_root, const rokz::Device& device) {

  obj_params params (device, polyd); 

  printf ("%s \n", __FUNCTION__); 
  //rokz::ReadStreamRef rs = rokz::CreateReadFileStream (data_root + "/texture/blue_0_texture.png"); 
  const char* test_image_files[] = { 
    "out_0_blue-texture-image-hd_rgba.png",
    "out_1_abstract-texture-3_rgba.png",
  };

  rokz::Buffer stage_image; 
  //   const std::string fq_test_file = data_root + "/texture/out_1_abstract-texture-3_rgba.png";  
  const std::string fq_test_file = data_root + "/texture/out_0_blue-texture-image-hd_rgba.png";  

  int res =  rekz::OpenImageFile (fq_test_file, std::make_shared<obj_image_handler> (params)); 
  
  if (res == 0) {

    // VkImageViewCreateInfo& CreateInfo (
    //      VkImageViewCreateInfo& ci, VkFormat format, VkImageAspectFlags aspect_flags, const rc::Image::Ref image);

    rokz::cx::CreateInfo (polyd.imageview.ci, VK_FORMAT_B8G8R8A8_SRGB, 
                          VK_IMAGE_ASPECT_COLOR_BIT, polyd.texture);  

    if (VK_SUCCESS == vkCreateImageView(device.handle, &polyd.imageview.ci, nullptr, &polyd.imageview.handle)) {
      // make the sampler
      rokz::cx::CreateInfo (polyd.sampler.ci, device.physical.properties);
      rokz::cx::CreateSampler (polyd.sampler, device.handle);
      printf ("[SUCCESS] %s all things created\n", __FUNCTION__);

    }
    else {
      printf ("[FAILED] %s create texture image view\n", __FUNCTION__);
      res = __LINE__;
    }
  }

  return (res == 0); 
}

// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
bool setup_obj_resources (rekz::PolygonData& polyd, const std::string& data_root, const rokz::Device& device) { 


  const rekz::platonic::Mesh& darkmesh = rekz::platonic::Octohedron (); 

  // vertex buffer
  rokz::Create_VB_device (polyd.vb_device,  &darkmesh.verts[0], rekz::platonic::Mesh::VertexSize * darkmesh.verts.size(), device) ;
  // index buffer 
  rokz::Create_IB_16_device (polyd.ib_device, &darkmesh.indices[0], darkmesh.indices.size (), device) ;

  if (!setup_object_texture_and_sampler(polyd, data_root, device)) {
    printf ("[FAILED] --> SetupObjectTexture \n"); 
    return false;
  }

  return true;
}
//#endif

// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
rekz::PolygonData& rekz::SetupPolygonData (rekz::PolygonData& pd, uint32_t num_frames, const std::string& data_root, const rokz::Device& device) {

  setup_obj_resources (pd, data_root, device) ; 
  return  pd;
}

// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
void rekz::CleanupPolygonData (PolygonData& pd, const rokz::Device& device) {
    // SetupObjectUniforms ; 
    // SetupObjectTextureAndSampler;
    // SetupObjResources;
  rokz::cx::Destroy (pd.ib_device, device.allocator); 
  rokz::cx::Destroy (pd.vb_device, device.allocator); 

  rokz::cx::Destroy (pd.sampler, device.handle); 

  pd.texture.reset (); // rokz::cx::Destroy (pd.texture, device.allocator.handle); 

  rokz::cx::Destroy (pd.imageview, device.handle); 

}

