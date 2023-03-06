
#include "darkrootgarden.h"
#include "rekz/dark_types.h"



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
int obj_image_handler (const unsigned char* dat, const rekz::DevILImageProps& props, void* up) {

  obj_params*   params = reinterpret_cast<obj_params*> (up); 
  rekz::PolygonData& polyd  = params->poly;

  
  if (rekz::LoadTexture_color_sampling (polyd.texture, VK_FORMAT_R8G8B8A8_SRGB ,
                                        VkExtent2D { (uint32_t) props.width, (uint32_t) props.height },
                                        dat, params->device.allocator.handle, params->device.queues.graphics, 
                                        params->device.command_pool, params->device)) {
    return 0;  
  }
  
  
  return __LINE__;
} 


// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
bool setup_object_texture_and_sampler (rekz::PolygonData& polyd, const std::string& data_root, const rokz::Device& device) {

  obj_params params (device, polyd); 

  printf ("%s \n", __FUNCTION__); 
  //rokz::ReadStreamRef rs = rokz::CreateReadFileStream (data_root + "/texture/blue_0_texture.png"); 
  const char*  test_image_files[] = { 
    "out_0_blue-texture-image-hd_rgba.png",
    "out_1_abstract-texture-3_rgba.png",
  };

 
  rokz::Buffer stage_image; 
  //   const std::string fq_test_file = data_root + "/texture/out_1_abstract-texture-3_rgba.png";  
  const std::string fq_test_file = data_root + "/texture/out_0_blue-texture-image-hd_rgba.png";  

  int res =  rekz::OpenImageFile (fq_test_file, obj_image_handler, &params); 
  
  if (res == 0) {
    rokz::cx::CreateInfo (polyd.imageview.ci, VK_IMAGE_ASPECT_COLOR_BIT, polyd.texture);  
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
  // used in here:
  // SetupObjectUniforms;
  // SetupObjectTextureAndSampler;
  const rekz::PolyMesh& darkmesh = rekz::platonic::Octohedron (); 

  // vertex buffer
  rokz::Create_VB_device (polyd.vb_device,  &darkmesh.verts[0], DarkMesh::VertexSize * darkmesh.verts.size(), device) ;
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
rekz::PolygonData& rekz::CleanupPolygonData (PolygonData& pd, const VmaAllocator& allocator) {

    // SetupObjectUniforms ; 
    // SetupObjectTextureAndSampler;
    // SetupObjResources;
    
    return  pd;
}

