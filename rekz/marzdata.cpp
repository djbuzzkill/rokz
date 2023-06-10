
#include "marzdata.h"
#include "rekz/image_loader.h"
#include "landscape_pipeline.h"
#include "rokz/image.hpp"
#include "rokz/sampler.hpp"
#include "rokz/texture.hpp"
#include <vulkan/vulkan.h>


using namespace rokz;

  
std::filesystem::path marz::tile::basepath = "/home/djbuzzkill/owenslake/tmp/testmarz";


std::string marz::tile::name_format (const char* name_format, uint32 x, uint32 z) {
  std::array<char, marz::kMaxNameLen> buf;
  sprintf (&buf[0], name_format, x, z); 
  return &buf[0];
}

// ------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------
struct height_handlr : public cx::mappedimage_cb  {

  const std::string&  fqname; 
  //
  height_handlr (const std::string& filen): fqname (filen)  {
    printf ("%s -> %s\n", __FUNCTION__, fqname.c_str ());
  }
  //
  virtual int on_mapped (void* mappedp, size_t maxsize, const VkExtent2D& ext2d) {
    
    rokz::bytearray fheights; 

    rokz::From_file (fheights, fqname, true); 

    if (fheights.size() && fheights.size() <= maxsize) {
      memcpy (mappedp, &fheights[0], fheights.size());
      return 0;
    }
    printf ("%s FAILED | fheights.size (%zu), maxsize:%zu\n ",
            __FUNCTION__,
            fheights.size(),
            maxsize); 

    return __LINE__;
  }

};


// ------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------
struct f32color_handler : public rokz::cx::mappedimage_cb { 

  const std::string&  fqname; 
    
  f32color_handler (const std::string& srcname) : fqname (srcname) {
    printf ("%s -> %s\n", __FUNCTION__, fqname.c_str ());
  }
  
  virtual int on_mapped  (void* mappedp, size_t maxsize, const VkExtent2D& ext2d) {
    
    if (!std::filesystem::exists (fqname)) {
      printf ("NOT FOUND: %s \n", fqname.c_str()); 
      return __LINE__;
    }
    
    bytearray filebytes; 
    rokz::From_file (filebytes, fqname, true );
    if (filebytes.size()  && filebytes.size() <= maxsize) {
      memcpy (mappedp, &filebytes[0], filebytes.size());
      return 0;
    }
    
    printf ("%s FAILED  filebytes:%zu, maxsize:%zu\n",
            __FUNCTION__,
            filebytes.size (),
            maxsize);

    return __LINE__;
  }};

// ------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------
bool marz::SetupData (marz::Data& dat, const rokz::Device& device) {

  using namespace tile;

  //VkFormat image_format;
  const uint32 total_tiles = roi::XDim * roi::ZDim; 

  dat.colormaps.resize  (total_tiles);
  dat.colorviews.resize (total_tiles);

  dat.heightmaps.resize  (total_tiles);
  dat.heightviews.resize (total_tiles);

  dat.colorsampler  = rc::CreateSampler_default (device); 

  VkSamplerCreateInfo ci;
  cx::CreateInfo_height_sample (ci, device.physical.properties.limits.maxSamplerAnisotropy); 
  dat.heightsampler = rc::CreateSampler (ci, device);

  const VkExtent2D imgext {tile::x_dim, tile::z_dim};

  for (uint32 iz = roi::Z_BEG; iz <= roi::Z_LAST; ++iz) {
    for (uint32 ix = roi::X_BEG; ix <= roi::X_LAST; ++ix) {

      auto xz = roi::linear_index(ix - roi::X_BEG, iz - roi::Z_BEG);
      printf ( "x:%u, z:%u  -> xz:%u\n", ix - roi::X_BEG, iz - roi::Z_BEG, xz); 
      // --------------- COLOR ------------------
      dat.colormaps[xz] = 
        rc::CreateImage_2D_color_sampling (tile::x_dim, tile::z_dim, tile::colorformat,
                                           VK_SAMPLE_COUNT_1_BIT, device);
      const std::string fqcname = tile::basepath/color_name(ix, iz);

      if (GOT_PROB (cx::TransferToDeviceImage (dat.colormaps[xz]->handle,
                                               tile::sizeofcolor, tile::colorformat, imgext, 
                                               std::make_shared<f32color_handler>(fqcname),
                                               device))) {
        HERE("something bad");
        return false;
      }

      dat.colorviews[xz] = rc::CreateImageView (dat.colormaps[xz]->handle, tile::colorformat,
                                                VK_IMAGE_ASPECT_COLOR_BIT, device);
      if (!dat.colorviews[xz])
        printf ("BAD colorviews[%u,%u]\n", ix, iz);
      // -------------- HEIGHT --------------
      dat.heightmaps[xz] = 
        rc::CreateImage_2D_color_sampling (tile::x_dim, tile::z_dim, tile::heightformat,
                                           VK_SAMPLE_COUNT_1_BIT, device);
      const std::string fqhname = tile::basepath/height_name(ix, iz);
      if (GOT_PROB (cx::TransferToDeviceImage (dat.heightmaps[xz]->handle,
                                               tile::sizeofheight, tile::heightformat, imgext,
                                               std::make_shared<height_handlr>(fqhname),
                                               device))) {
        HERE("transfer failed");
        return false;
      }

      dat.heightviews[xz] = rc::CreateImageView (dat.heightmaps[xz]->handle, tile::heightformat,
                                                 VK_IMAGE_ASPECT_COLOR_BIT, device);
      if (!dat.heightviews[xz]) {
        // return false? 
        printf ("BAD heightviews[%u,%u]\n", ix, iz);
      }
    }} // XZLOOP 

  // 
  struct SetupGeom : public cx::mappedbuffer_cb {
    // ??? is this it
    const size_t reqsize = sizeof (verts);

    const lscape::Vert verts[4] = {
        { glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec2(0,0) }, 
        { glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec2(1, 0) }, 
        { glm::vec3(1, 0, -1), glm::vec3(0, 1, 0), glm::vec2(1, 1) }, 
        { glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), glm::vec2(0, 1) }, 
      }; 

    virtual int on_mapped  (void* mappedp , size_t maxsize) {
      memcpy (mappedp, verts, reqsize); 
      return 0; 
    }} setupg;

  dat.devicebuffer = rc::CreateDeviceBuffer (setupg.reqsize, cx::kDeviceGeometryUsage, device); 
  cx::TransferToDeviceBuffer (dat.devicebuffer->handle, setupg.reqsize, &setupg, device); 
    
  return true;
}

// --------------------------------------------------------------------------------------------
//                        
// --------------------------------------------------------------------------------------------
void marz::CleanupData (marz::Data& dat, const rokz::Device& device) {

  dat.devicebuffer.reset ();

  dat.colormaps.clear ();
  dat.colorviews.clear ();

  dat.heightmaps.clear ();
  dat.heightviews.clear ();

  dat.normalmaps.clear ();
  dat.normalviews.clear ();
    
  dat.heightsampler.reset ();
  dat.colorsampler.reset ();
  dat.normalsampler.reset ();

}
