
#include "darkrootgarden.h"



using namespace darkroot;



// -------------------------------------------------------------------------
// used in PolygonDraw
// -------------------------------------------------------------------------
int obj_image_handler (const unsigned char* dat, const rekz::DevILImageProps& props, void* up);



// -------------------------------------------------------------------------
// UP for  obj_image_handler 
// -------------------------------------------------------------------------
template<typename Ty> 
struct ObjParams {

  ObjParams (const rokz::Device& dev, Ty* m) : device (dev), more (m) {
  }
  
  const rokz::Device& device;
  Ty*                 more;
};

// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
struct PolygonDraw : public DrawSequence {


  PolygonDraw (const rokz::Device& device) {

    setup_obj_resources;
    setup_object_texture_and_sampler;
    setup_object_uniforms ;

  }

  
  rokz::Buffer        vb_device;
  rokz::Buffer        ib_device;

  
  virtual int Exec (VkCommandBuffer command_buffer, const rokz::Pipeline& pl, const VkDescriptorSet* ds) {

    const DarkMesh& darkmesh = DarkOctohedron ();
    // ext2D  used to come from -> swapchain.ci.imageExtent
    //    VkViewport viewport{};
    //    viewport.x = 0.0f;
    //    viewport.y = 0.0f;
    //    viewport.width = static_cast<float>(ext2d.width);
    //    viewport.height = static_cast<float>(ext2d.height);
    //    viewport.minDepth = 0.0f;
    //    viewport.maxDepth = 1.0f;
        
    //    VkRect2D scissor{};
    //    scissor.offset = {0, 0};
    //    scissor.extent = ext2d;
    vkCmdBindPipeline (command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pl.handle);

    vkCmdSetViewport(command_buffer, 0, 1, &pl.state.viewport.viewports[0]);

    vkCmdSetScissor(command_buffer, 0, 1, &pl.state.viewport.scissors[0]);

    vkCmdBindDescriptorSets (command_buffer,
                             VK_PIPELINE_BIND_POINT_GRAPHICS,
                             pl.layout.handle,
                             0,
                             1, ds, //&descriptorSets[currentFrame],
                             0, nullptr);
  
    VkBuffer vertex_buffers[] = {vb_device.handle};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);
    vkCmdBindIndexBuffer(command_buffer, ib_device.handle, 0, VK_INDEX_TYPE_UINT16);

    for (uint32_t i = 0; i < 2; ++i) {

      darkroot::PushConstants pcs {};
      pcs.drawIDs.x = i; 
      pcs.drawIDs.y = i; 
      pcs.drawIDs.z = i; 
      pcs.drawIDs.w = i; 

      const VkShaderStageFlags shader_stages =
        VK_SHADER_STAGE_VERTEX_BIT ; //| VK_SHADER_STAGE_FRAGMENT_BIT;

      vkCmdPushConstants (command_buffer,
                          pl.layout.handle,
                          shader_stages,
                          0,
                          sizeof(darkroot::PushConstants),
                          &pcs);

      vkCmdDrawIndexed (command_buffer, darkmesh.indices.size(), 1, 0, 0, 0);
    }

    return false;
  }


protected:

  // -----------------------------------------------------------------------------
  bool setup_obj_resources (const rokz::Device& device) { 

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
  
    rokz::cx::CreateInfo_VB_device (vb_device.ci, DarkMesh::VertexSize, darkmesh.verts.size());
    rokz::cx::AllocCreateInfo_device (vb_device.alloc_ci); 
    rokz::cx::CreateBuffer (vb_device, device.allocator.handle); 

    //rokz::Transfer_2_Device;
    rokz::cx::MoveToBuffer_XB2DB (vb_device, vb_x, DarkMesh::VertexSize * darkmesh.verts.size(), 
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
  
    rokz::cx::CreateInfo_IB_16_device (ib_device.ci, darkmesh.indices.size()); 
    rokz::cx::AllocCreateInfo_device  (ib_device.alloc_ci);
    rokz::cx::CreateBuffer            (ib_device, device.allocator.handle);

    rokz::cx::MoveToBuffer_XB2DB  (ib_device, ib_x, DarkMesh::IndexSize * darkmesh.indices.size (), 
                                   device.command_pool.handle, device.queues.graphics, device.handle); 
    rokz::cx::Destroy (ib_x, device.allocator.handle); 
    //DarkMesh& dark_mesh = glob.dark_mesh;

    // if (!SetupObjectUniforms (glob)) {
    //   printf ("[FAILED] --> SetupObjectUniforms \n"); 
    //   return false;
    // }
  
    // // rokz::DescriptorPool           uniform_descriptor_pool;
    // // rokz::DescriptorGroup          uniform_group; 
    // if (!SetupObjectTextureAndSampler (glob)) {
    //   printf ("[FAILED] --> SetupObjectTexture \n"); 
    //   return false;
    // }

    return true;
  }

  // ------------------------------------------------------------------------------------------------

  bool setup_object_uniforms (std::vector<rokz::Buffer>& uniform_buffs, std::vector<rokz::Buffer>& objparams, 
                            const rokz::Device& device) {
    printf ("%s", __FUNCTION__);

    //  VkPhysicalDevice const&  physdev = glob.physical_device.handle;

  
    uniform_buffs.resize (kMaxFramesInFlight);
    //mapped_ptrs.resize (kMaxFramesInFlight); 
    objparams.resize (kMaxFramesInFlight);
    //objparam_ptrs.resize (kMaxFramesInFlight);
  
    for (size_t i = 0; i <  kMaxFramesInFlight; i++) {

      rokz::cx::CreateInfo_uniform (uniform_buffs[i].ci, rokz::kSizeOf_MVPTransform, 1); 
      rokz::cx::AllocCreateInfo_mapped (uniform_buffs[i].alloc_ci); 
      if (!rokz::cx::CreateBuffer (uniform_buffs[i], device.allocator.handle)) {
        printf (" --> [FAIL]  create MVPTransform \n"); 
        return false; 
      }

      rokz::cx::CreateInfo_uniform (objparams[i].ci, SizeOf_SceneObjParam, 128);
      rokz::cx::AllocCreateInfo_mapped (objparams[i].alloc_ci);
      if (!rokz::cx::CreateBuffer (objparams[i], device.allocator.handle)) {
        printf (" --> [FAIL]  create SceneObjParam \n"); 
        return false; 
      }

    }

    printf (" --> [true] \n"); 
    return true; 
  }

  // ------------------------------------------------------------------------------------------------

  bool setup_object_texture_and_sampler (const rokz::Device& device) {

    ObjParams<PolygonDraw> params (device, this); 

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
      rokz::cx::CreateInfo (texture_imageview.ci, VK_IMAGE_ASPECT_COLOR_BIT, texture_image);  
      if (VK_SUCCESS == vkCreateImageView(device.handle, &texture_imageview.ci, nullptr, &texture_imageview.handle)) {
        // make the sampler
        rokz::cx::CreateInfo (sampler.ci, physical_device.properties);
        rokz::cx::CreateSampler (sampler, device.handle);
        printf ("[SUCCESS] %s all things created\n", __FUNCTION__);

      }
      else {
        printf ("[FAILED] %s create texture image view\n", __FUNCTION__);
        res = __LINE__;
      }
    }

    return (res == 0); 
  }


}; // PolygonDraw


// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
int obj_image_handler (const unsigned char* dat, const rekz::DevILImageProps& props, void* up) {

    ObjParams<PolygonDraw>* params = reinterpret_cast<ObjParams<PolygonDraw>*> (up); 
    PolygonDraw*            poly   = params->more;
    
  if (rekz::LoadTexture_color_sampling (poly->texture_image, VK_FORMAT_R8G8B8A8_SRGB ,
                                  VkExtent2D { (uint32_t) props.width, (uint32_t) props.height },
                                  dat, params->device.allocator.handle, params->device.queues.graphics, 
                                  params->device.command_pool, params->device)) {
    return 0;  
  }

  return __LINE__;
} 


