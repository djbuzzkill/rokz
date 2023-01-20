
#include "rekz.h"              // 


//#include "rokz/rokz.h"
//
//#include "rokz_test.cpp"
//#include "texture_tool.cpp"
#include <IL/il.h>
#include <IL/ilu.h>


// ---------------------------------------------------------------------------
// handle most of the common ones
// ---------------------------------------------------------------------------
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

// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
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

  rokz::cx::Destroy (stage_buff, allocator); 
  return true; 
}



// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
void rekz::SetupViewportState (rokz::ViewportState & vps, const VkExtent2D& swapchain_extent) {

  const VkOffset2D offs0 {0, 0};

  vps.viewports.resize (1);
  vps.scissors.resize (1);
  
  vps.scissors[0] = { offs0, swapchain_extent };
  rokz::ViewportState_default (vps, vps.scissors[0], 1.0f); 

}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------

int test_rokz (const std::vector<std::string>& args); 
int test_rokz_hpp (const std::vector<std::string>& args); 
int texture_tool (const std::vector<std::string>& args); 
int test_ouput (const std::vector<std::string>& args); 
int test_time (); 
int darkroot_basin (const std::vector<std::string>& args);

int mars_run (const std::vector<std::string>& args);
int mars_prelim (const std::vector<std::string>& args);

// --------------------------------------------------------------------
int main (int argv, char** argc) {

  const std::vector<std::string> args (argc, argc + argv);

  //   test_time (); 
  //test_rokz (args); 
  darkroot_basin  (args);
  //mars_run  (args);
  // test_rokz_hpp (args); 
  // texture_tool (args); 
  //test_ouput (args);
  
  printf ("lolz bai %s\n", __FUNCTION__); 
  return 0; 
}
