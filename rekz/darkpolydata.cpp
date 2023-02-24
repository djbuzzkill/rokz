
#include "darkrootgarden.h"
#include "rekz/dark_types.h"



using namespace darkroot;


// -------------------------------------------------------------------------
// UP for  obj_image_handler 
// -------------------------------------------------------------------------
struct obj_params {
  
  obj_params (const rokz::Device& dev, struct PolygonData& d) : device (dev), poly (d) {
  }
  
  const rokz::Device& device;
  PolygonData& poly;
};
// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
int obj_image_handler (const unsigned char* dat, const rekz::DevILImageProps& props, void* up) {

  obj_params*   params = reinterpret_cast<obj_params*> (up); 
  PolygonData& polyd  = params->poly;

  
  if (rekz::LoadTexture_color_sampling (polyd.texture, VK_FORMAT_R8G8B8A8_SRGB ,
                                        VkExtent2D { (uint32_t) props.width, (uint32_t) props.height },
                                        dat, params->device.allocator.handle, params->device.queues.graphics, 
                                        params->device.command_pool, params->device)) {
    return 0;  
  }
  
  
  return __LINE__;
} 


//#ifdef DARKROOT_HIDE_SETUP_OBJECT_TEXTURE_AND_SAMPLER
// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
bool setup_object_texture_and_sampler (PolygonData& polyd, const std::string& data_root, const rokz::Device& device) {

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

//#endif



//#ifdef DARKROOT_HIDE_SETUP_OBJECT_UNIFORMS
#ifdef DARKROOT_MOVE_SETUP_OBJECT_UNIFORMS_TO_PIPELINE
// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
bool setup_object_uniforms (std::vector<rokz::Buffer>& uniform_buffs, std::vector<rokz::Buffer>& objparams,
                            uint32_t num_sets, const rokz::Device& device) {
  printf ("%s", __FUNCTION__);

  //  VkPhysicalDevice const&  physdev = glob.physical_device.handle;

  // does this belong in pipeline?  
  // 
  
  
  uniform_buffs.resize (num_sets);
  //mapped_ptrs.resize (kMaxFramesInFlight); 
  objparams.resize (num_sets);
  //objparam_ptrs.resize (kMaxFramesInFlight);
  
  for (size_t i = 0; i < num_sets; i++) {
    rokz::cx::CreateInfo_uniform (uniform_buffs[i].ci, rokz::kSizeOf_MVPTransform, 1); 
    rokz::cx::AllocCreateInfo_mapped (uniform_buffs[i].alloc_ci); 
    if (!rokz::cx::CreateBuffer (uniform_buffs[i], device.allocator.handle)) {
      printf (" --> [FAIL]  create MVPTransform buffer \n"); 
      return false; 
    }

    rokz::cx::CreateInfo_uniform (objparams[i].ci, SizeOf_SceneObjParam, 128);
    rokz::cx::AllocCreateInfo_mapped (objparams[i].alloc_ci);
    if (!rokz::cx::CreateBuffer (objparams[i], device.allocator.handle)) {
      printf (" --> [FAIL]  create Polygon uniform buffer \n"); 
      return false; 
    }
  }

  printf (" --> [true] \n"); 
  return true; 
}

#endif



//#ifdef DARKROOT_HIDE_SETUP_OBJ_RESOURCES
// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
bool setup_obj_resources (PolygonData& polyd, uint32_t max_frames_in_flight,
                          const std::string& data_root, const rokz::Device& device) { 


  // used in here:
  // SetupObjectUniforms;
  // SetupObjectTextureAndSampler;
  const DarkMesh& darkmesh = darkroot::DarkOctohedron (); 

  void* pmapped  = nullptr;
  rokz::Buffer vb_x; 
  rokz::cx::CreateInfo_VB_stage (vb_x.ci, DarkMesh::VertexSize, darkmesh.verts.size());
  rokz::cx::AllocCreateInfo_stage (vb_x.alloc_ci);
  rokz::cx::CreateBuffer (vb_x, device.allocator.handle);
  if (rokz::cx::MapMemory (&pmapped, vb_x.allocation, device.allocator.handle)) {
    memcpy (pmapped, &darkmesh.verts[0], DarkMesh::VertexSize * darkmesh.verts.size()); 
    rokz::cx::UnmapMemory (vb_x.allocation, device.allocator.handle); 
  }
  rokz::cx::CreateInfo_VB_device (polyd.vb_device.ci, DarkMesh::VertexSize, darkmesh.verts.size());
  rokz::cx::AllocCreateInfo_device (polyd.vb_device.alloc_ci); 
  rokz::cx::CreateBuffer (polyd.vb_device, device.allocator.handle); 

  //rokz::Transfer_2_Device;
  rokz::cx::MoveToBuffer_XB2DB (polyd.vb_device, vb_x, DarkMesh::VertexSize * darkmesh.verts.size(), 
                                device.command_pool.handle, device.queues.graphics, device.handle); 

  rokz::cx::Destroy (vb_x, device.allocator.handle); 
  
  // INDEX BUFF
  rokz::Buffer ib_x;
  rokz::cx::CreateInfo_IB_16_stage (ib_x.ci, darkmesh.indices.size()); 
  rokz::cx::AllocCreateInfo_stage (ib_x.alloc_ci);
  rokz::cx::CreateBuffer (ib_x, device.allocator.handle);

  if (rokz::cx::MapMemory (&pmapped, ib_x.allocation, device.allocator.handle)) {
    memcpy (pmapped, &darkmesh.indices[0], DarkMesh::IndexSize * darkmesh.indices.size()  ); 
    rokz::cx::UnmapMemory (ib_x.allocation, device.allocator.handle); 
  }
  
  rokz::cx::CreateInfo_IB_16_device (polyd.ib_device.ci, darkmesh.indices.size()); 
  rokz::cx::AllocCreateInfo_device  (polyd.ib_device.alloc_ci);
  rokz::cx::CreateBuffer            (polyd.ib_device, device.allocator.handle);

  rokz::cx::MoveToBuffer_XB2DB  (polyd.ib_device, ib_x, DarkMesh::IndexSize * darkmesh.indices.size (), 
                                 device.command_pool.handle, device.queues.graphics, device.handle); 
  rokz::cx::Destroy (ib_x, device.allocator.handle); 
  //DarkMesh& dark_mesh = glob.dark_mesh;

#ifdef DARKROOT_MOVE_SETUP_OBJECT_UNIFORMS_TO_PIPELINE
  // this should never have been in here 
  if (!setup_object_uniforms (polyd.vma_uniform_buffs, polyd.vma_objparam_buffs,
                              max_frames_in_flight, device)) {
    printf ("[FAILED] --> setup_object_uniforms \n"); 
    return false;
  }
#endif
  
  // bool setup_object_uniforms (std::vector<rokz::Buffer>& uniform_buffs, std::vector<rokz::Buffer>& objparams, 
//                             const rokz::Device& device) {
  
  // // rokz::DescriptorPool           uniform_descriptor_pool;
  // // rokz::DescriptorGroup          uniform_group; 
  if (!setup_object_texture_and_sampler(polyd, data_root, device)) {
    printf ("[FAILED] --> SetupObjectTexture \n"); 
    return false;
  }

  return true;
}
//#endif



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
PolygonData& darkroot::SetupPolygonData (PolygonData& pd, uint32_t num_frames, const std::string& data_root, const rokz::Device& device) {
  pd.view_orie.theta = 0.0f;
  pd.view_orie.phi   = kPi;
  setup_obj_resources (pd, num_frames, data_root, device) ; 
  return  pd;
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
PolygonData& darkroot::CleanupPolygonData (PolygonData& pd, const VmaAllocator& allocator) {

    // SetupObjectUniforms ; 
    // SetupObjectTextureAndSampler;
    // SetupObjResources;

    for (auto buf : pd.vma_poly_uniforms) {  
      rokz::cx::Destroy (buf, allocator);
    }
    
    return  pd;
}

