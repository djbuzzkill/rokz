
#include "marzdata.h"
#include "rekz/image_loader.h"
#include "landscape_pipeline.h"
#include "rokz/image.h"
#include "rokz/texture.h"
#include <vulkan/vulkan_core.h>




auto NO_PROB = 0;


using namespace rokz;

namespace tileregion { 

  const uint32 X_BEG  = 4;
  const uint32 X_LAST = 4;

  const uint32 Z_BEG  = 9;
  const uint32 Z_LAST = 9;

  const uint32 XDim = X_LAST - X_BEG + 1; 
  const uint32 ZDim = Z_LAST - Z_BEG + 1;

  uint32 lindx (uint32 x, uint32 z) {
    return z * XDim + x; 
  } 
}


std::string marz::name_format (const char* name_format, uint32 x, uint32 z) {
  std::array<char, marz::kMaxNameLen> buf;
  sprintf (&buf[0], name_format, x, z); 
  return &buf[0];
}

// ------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------
struct height_handlr : public cx::mappedimage_cb  {

  const std::string&  fqname; 

  const VkFormat  imageformat = VK_FORMAT_R32_SFLOAT; 
  //
  height_handlr (const std::string& filen): fqname (filen)  {
  }
  //
  virtual int on_mapped (void* mappedp, size_t maxsize, const VkExtent2D& ext2d) {
    
    rokz::bytearray fheights; 
    rokz::From_file (fheights, fqname, true); 

    if (fheights.size() <= maxsize) {
      memcpy (mappedp, &fheights[0], fheights.size());
      return 0;
    }


      
    HERE ("FAILED height handler");
    return __LINE__;
  }

};


// ------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------
struct color_handler_f32 : public rokz::cx::mappedimage_cb { 

  rc::Image::Ref&     image;
  const std::string&  fqname; 

  const VkFormat      colorformat = VK_FORMAT_R32_SFLOAT; 
  //
    
  color_handler_f32 (rc::Image::Ref& im, const std::string& srcname) : image (im), fqname (srcname) {
  }
  
  virtual int on_mapped  (void* mappedp, size_t maxsize, const VkExtent2D& ext2d) {

    bytearray filebytes; 
    rokz::From_file (filebytes, fqname, true );

    if (filebytes.size() <= maxsize) {
      memcpy (mappedp, &filebytes[0], filebytes.size());
      return 0;
    }
    
    HERE ("FAILED");
    return __LINE__;
  }};

// ------------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------------
bool marz::SetupData (marz::Data& dat, const rokz::Device& device) {

  using namespace tileregion; 
  
  std::filesystem::path basepath = "/home/djbuzzkill/owenslake/somepath"; 

  VkFormat image_format;

  uint32 total_tiles = XDim * ZDim; 
    
  dat.colormaps.resize (total_tiles);
  dat.colorviews.resize (total_tiles);

  dat.colorsampler = rc::CreateSampler_default (device); 
    
  for (uint32 iz = tileregion::Z_BEG; iz <= tileregion::Z_LAST; ++iz) {
    for (uint32 ix = tileregion::X_BEG; ix <= tileregion::X_LAST; ++ix) {

      int res = __LINE__;
      
      { // COLOR

        const size_t     sizeofcolordata = sizeof(float) * marz::tile::x_dim *  marz::tile::z_dim; 
        const VkFormat   colorformat   = VK_FORMAT_R32_SFLOAT;
        const VkExtent2D imgext         = { marz::tile::x_dim, marz::tile::z_dim } ;

        dat.colormaps[lindx(ix, iz)] =
          rc::CreateImage_2D_color_sampling (marz::tile::x_dim, marz::tile::z_dim,
                                             colorformat, VK_SAMPLE_COUNT_1_BIT, device);
        
        const std::string fqname = basepath/color_name(ix, iz);

        if (NO_PROB != cx::TransferToDeviceImage (dat.heightmaps[lindx(ix, iz)]->handle,
                                                  sizeofcolordata,
                                                  colorformat, 
                                                  imgext,
                                                  std::make_shared<color_handler_f32>(dat.colormaps[lindx(ix, iz)], fqname),
                                                  device)) {
          return false;
        }

        dat.colorviews[lindx(ix, iz)] = rc::CreateImageView (dat.colormaps[lindx(ix, iz)]->handle,
                                                             colorformat,
                                                             VK_IMAGE_ASPECT_COLOR_BIT, device);
          
        
      }

      
      { // HEIGHT 
        const size_t     sizeofheightdata = sizeof(float) * marz::tile::x_dim *  marz::tile::z_dim; 
        const VkFormat   heightformat   = VK_FORMAT_R32_SFLOAT;
        const VkExtent2D imgext         = { marz::tile::x_dim, marz::tile::z_dim } ;

        dat.heightmaps[lindx(ix, iz)] =
          rc::CreateImage_2D_color_sampling (marz::tile::x_dim, marz::tile::z_dim,
                                             heightformat, VK_SAMPLE_COUNT_1_BIT, device);
        
        const std::string fqname = basepath/height_name(ix, iz);

        res = __LINE__;
        if (NO_PROB != cx::TransferToDeviceImage (dat.heightmaps[lindx(ix, iz)]->handle,
                                                  sizeofheightdata,
                                                  heightformat, 
                                                  imgext, std::make_shared<height_handlr>(fqname), device)) {
          // transfer failed?
          return false;
        }

        dat.heightviews[lindx(ix, iz)] = rc::CreateImageView (dat.heightmaps[lindx(ix, iz)]->handle,
                                                              heightformat,
                                                              VK_IMAGE_ASPECT_COLOR_BIT, device);

        
          
      }
      
    }} // END TILE LOOP 

  
  
  struct handlegeom : public cx::mappedbuffer_cb {
    handlegeom () {}
    virtual int on_mapped  (void* mappedp , size_t maxsize) {
      // ??? is this it
      rekz::landscape::PatchVert verts[4] = {
        { glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec2(0,0) }, 
        { glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec2(1, 0) }, 
        { glm::vec3(1, 0, 1), glm::vec3(0, 1, 0), glm::vec2(1, 1) }, 
        { glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec2(0, 1) }, 
      }; 
      memcpy (mappedp, verts, maxsize); 
      return 0; 
    }} handleg;

  const size_t reqsize = 4 * sizeof (rekz::landscape::PatchVert);
  dat.devicebuffer = rc::CreateDeviceBuffer (reqsize, cx::kDeviceGeometryUsage, device); 
  cx::TransferToDeviceBuffer (dat.devicebuffer->handle, reqsize, &handleg, device); 
    
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
