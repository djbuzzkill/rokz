
#include "darkrootgarden.h"
#include "rekz/dark_types.h"

#include "image_loader.h"
#include "rekz/meshery.h"
#include "rekz/rekz.h"
#include "rokz/buffer.h"
#include "rokz/rc_types.h"
#include "rokz/ut_offset.h"
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

    polyd.imageview = rc::CreateImageView (polyd.texture, VK_FORMAT_B8G8R8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, device);

    if (!polyd.imageview) {
      printf ("[FAILED] %s create texture image view\n", __FUNCTION__);
      res = __LINE__;
    }

    polyd.sampler = rc::CreateSampler_default (device); 
    printf ("[SUCCESS] %s all things created\n", __FUNCTION__);
  }

  return (res == 0); 
}


// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
struct preparebuff : public rokz::cx::mappedbuffer_cb {

  preparebuff (rekz::PolygonData& pd) :polyd (pd) { }
  // 
  virtual int on_mapped  (void* dstp, size_t maxsize) {

    const rekz::platonic::Mesh& darkmesh = rekz::platonic::Octohedron (); 

    assert (maxsize == geom::ComputeTotalSize (darkmesh));

    std::array<size_t, 4> asize = {
      geom::ComputeVertexSize (darkmesh), 
      geom::ComputeIndexSize  (darkmesh), 
    };

    polyd.vertexoffs= ut::offset_at (asize, 0);    
    polyd.indexoffs = ut::offset_at (asize, 1);

    unsigned char* dstuc = (unsigned char*) dstp;

    memcpy (dstuc + polyd.vertexoffs, &darkmesh.verts[0],   asize[0]);
    memcpy (dstuc + polyd.indexoffs , &darkmesh.indices[0], asize[1]); 

    return 0; 
  }

  rekz::PolygonData& polyd;
};


// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
bool setup_obj_resources (rekz::PolygonData& polyd, const std::string& data_root, const rokz::Device& device) { 

  const rekz::platonic::Mesh& darkmesh = rekz::platonic::Octohedron (); 

  const VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT
                                 | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
                                 | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  polyd.devicebuffer =
      rc::CreateDeviceBuffer (rekz::geom::ComputeTotalSize (darkmesh), usage, device); 

  preparebuff prepb (polyd); 
  rokz::cx::TransferToDeviceBuffer (polyd.devicebuffer->handle,
                                    rekz::geom::ComputeTotalSize (darkmesh),
                                    &prepb, device);

  if (!setup_object_texture_and_sampler(polyd, data_root, device)) {
    printf ("[FAILED] --> SetupObjectTexture \n"); 
    return false;
  }

  return true;
}

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

  pd.sampler.reset (); 
  pd.texture.reset (); // rokz::cx::Destroy (pd.texture, device.allocator.handle); 
  pd.imageview.reset(); //rokz::cx::Destroy (pd.imageview, device.handle); 
  pd.devicebuffer.reset ();
}

