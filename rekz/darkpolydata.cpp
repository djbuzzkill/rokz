
#include "rekz/dark_types.h"

#include "image_loader.h"
#include "rekz/meshery.h"
#include "rekz/rekz.h"
#include "rokz/buffer.hpp"
#include "rokz/rc_types.hpp"
#include "rokz/ut_offset.hpp"
#include <vulkan/vulkan_core.h>


using namespace darkroot;
// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
struct obj_image_handler : public rekz::DevILOpenFileCB {

  uint32              index ; 
  const rokz::Device& device;
  rekz::PolygonData&  polyd  ;

  obj_image_handler (uint32              ind,
                     rekz::PolygonData&  poly,
                     const rokz::Device& dev)
    : index (ind), polyd (poly), device (dev) {
  }

  const VkFormat imageformat = VK_FORMAT_B8G8R8A8_SRGB;
  virtual int Exec (const unsigned char* dat, const rekz::DevILImageProps& props) {

    polyd.textures[index] = rokz::LoadTexture_color_sampling (imageformat,
                                                              VkExtent2D { (uint32_t) props.width, (uint32_t) props.height },
                                                              dat, device.allocator.handle, device.queues.graphics, 
                                                              device.command_pool, device);
    if (polyd.textures[index]) {
      polyd.imageviews[index] = rc::CreateImageView (polyd.textures[index]->handle, imageformat, VK_IMAGE_ASPECT_COLOR_BIT, device);
      return 0;
    }
  
    HERE ("FAILED LoadTexture_color_sampling");
    return __LINE__;
  }};

// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
bool setup_object_texture_and_sampler (rekz::PolygonData& polyd, const std::string& data_root, const rokz::Device& device) {

  printf ("%s \n", __FUNCTION__); 
  const char* test_image_files[] = { 
    "/texture/out_0_blue-texture-image-hd_rgba.png",
    "/texture/out_1_abstract-texture-3_rgba.png",
  };

  polyd.textures.resize (2);
  polyd.imageviews.resize (2);

  int res = __LINE__;
  for (uint32 i = 0; i < 2; ++i) {
    const std::string test_file = data_root + test_image_files[i] ;

    res =  rekz::OpenImageFile (test_file, std::make_shared<obj_image_handler> (i, polyd, device));

    if (res != 0) {
      HERE ("problem with OpenImageFile ");
      return false;
    }
    
  }

  polyd.sampler = rc::CreateSampler_default (device); 

  printf ("[SUCCESS] %s all things created\n", __FUNCTION__);
  return true; 
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

  pd.indexoffs  = 0;
  pd.vertexoffs = 0;

  for (auto& r : pd.objrot) 
    r = glm::vec3(0.0);

  for (auto& p : pd.objpos) 
    p = glm::vec3(0.0);


  setup_obj_resources (pd, data_root, device) ; 
  return  pd;
}

// ------------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------------
void rekz::CleanupPolygonData (PolygonData& pd, const rokz::Device& device) {

  pd.sampler.reset (); 
  pd.devicebuffer.reset ();
  pd.textures.clear ();
  pd.imageviews.clear ();
}

