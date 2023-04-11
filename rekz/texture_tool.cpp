
//#include "rockz.h"             
#include "rokz/rokz.hpp"
// #include <GLFW/glfw3.h>
// #include <vulkan/vulkan_core.h>
#include "rokz/defaults.hpp"
//#include "rokz/rokz_funcs.h"
#include <IL/il.h>
#include <IL/ilu.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

typedef std::array<unsigned char, 4> rgba;

// ------------------------------------------------------------------------------------------------
//                                   
// ------------------------------------------------------------------------------------------------
int texture_tool (const std::vector<std::string>& args) {

  ilInit ();
  iluInit (); 

  printf  ("%s\n", __FUNCTION__); 

  ILuint image_name = 0; 

  const std::string tex_path = "/home/djbuzzkill/owenslake/data/textures"; 

  const char* file_names[] = {

    "blue-texture-image-hd.jpg",
    "abstract-texture-3.jpg",
    "mb3d_exp.png",
    "fuerte-stone-1.jpg", 
    "fuerte-stone-2.jpg", 
    "solid_waves.png",
  };

  size_t num_files = sizeof (file_names) / sizeof (const char*); 

  for (size_t  i = 0; i < num_files; ++i) {
    int ext_pos = strlen (file_names[i]) - 3;
    std::string ext = file_names[i] + ext_pos;
    printf ( "[%s] filename: %s/%s \n", ext.c_str(), tex_path.c_str(), file_names[i] ); 
  }

  std::vector<ILuint> image_names (num_files, 0); 
  std::vector<ILuint> work_names (num_files, 0); 

  ilGenImages (num_files, &image_names[0]);
  ilGenImages (num_files, &work_names[0]);

  for (size_t i = 0; i < num_files; ++i) { 
    // construct name
    int ext_pos = strlen (file_names[i]) - 3;

    std::string file_part (file_names[i], file_names[i] + ext_pos - 1);
    printf ( "file_part [%s]\n", file_part.c_str());
    std::string ext = file_names[i] + ext_pos;
    std::string fq_name = tex_path + "/" + ext + "/" + file_names[i]; 
    printf ("loading.. %s\n", fq_name.c_str());
    
    // source
    ilBindImage(image_names[i]);

    if (ilLoadImage(fq_name.c_str())) {

      int image_width    = ilGetInteger (IL_IMAGE_WIDTH); 
      int image_height   = ilGetInteger (IL_IMAGE_HEIGHT);
      int image_depth    = ilGetInteger (IL_IMAGE_DEPTH);
      int bytes_per_pixel= ilGetInteger (IL_IMAGE_BYTES_PER_PIXEL); 
      int image_bpp      = ilGetInteger (IL_IMAGE_BPP);
      int image_type     = ilGetInteger (IL_IMAGE_TYPE);
      int image_format   = ilGetInteger (IL_IMAGE_FORMAT); 

      int min_dim = std::min (image_width, image_height); 
      printf ("min_dim %i\n", min_dim); 

      printf ( "image dim [w:%i, h:%i, d:%i | bpp:%i, bytes:%i, type:%x, format:%x]\n",
               image_width, image_height, image_depth,
               image_bpp, bytes_per_pixel, image_type, image_format); 

      if (min_dim < 1024)
        continue; 

      
      ilBindImage (work_names[i]); 
      const int out_dim = 1024; 
      bool img_res = ilTexImage (out_dim, out_dim, image_depth,
                                 bytes_per_pixel, image_format, image_type,
                                 nullptr); 
      if (!img_res) { 
        printf ("img error : %s\n", iluErrorString (ilGetError()));
      }

      bool blit_res = ilBlit (image_names[i],
                              0, 0, 0,
                              0, 0, 0,
                              out_dim, out_dim, image_depth);                   
      
      if (!blit_res) { 
        printf ("blit error : %s\n", iluErrorString (ilGetError()));
      }
      
      //ILboolean iluScale(ILuint Width, ILuint Height, ILuint Depth)

      int work_width  = ilGetInteger (IL_IMAGE_WIDTH); 
      int work_height = ilGetInteger (IL_IMAGE_HEIGHT);
      int work_depth  = ilGetInteger (IL_IMAGE_DEPTH);
      int work_bpp    = ilGetInteger (IL_IMAGE_BPP);
      int work_bytes_per_pix  = ilGetInteger (IL_IMAGE_BYTES_PER_PIXEL);
      int work_type     = ilGetInteger (IL_IMAGE_TYPE);
      int work_format   = ilGetInteger (IL_IMAGE_FORMAT); 

      printf ("work_image[%i, %i, %i | bpp:%i, bytes:%i, t:%x, f:%x] \n",
              work_width, work_height, work_depth,
              work_bpp, work_bytes_per_pix,
              work_type, work_format); 

      char save_filename[256]; 
      sprintf (save_filename, "%s/out/out_%zu_%s.png", tex_path.c_str(), i, file_part.c_str());
      printf ("   save_filename %s\n\n\n", save_filename); 
      ilSave (IL_PNG, save_filename);  
      //      iluImageParameter(ILU_FILTER, ILU_SCALE_BELL);
      
      //      iluScale(ILuint Width, ILuint Height, ILuint Depth)
      //#endif
      

    }
    else {
      printf ("failed: [%zu] %s\n", i, fq_name.c_str()); 
    }    

    
  }

  ilDeleteImages (num_files, &image_names[0]);
  printf   ("%s BAI\n", __FUNCTION__); 
  return 0; 
}



int test_ouput (const std::vector<std::string>& args) {


  const std::string root_path = "/home/djbuzzkill/owenslake/data/textures/out"; 


  const char* output_files[] = { 
    "out_0_blue-texture-image-hd.png",
    "out_1_abstract-texture-3.png", 
    "out_2_mb3d_exp.png", 
    "out_3_fuerte-stone-1.png", 
    "out_4_fuerte-stone-2.png", 
    "out_5_solid_waves.png", 
  };


  const int num_files = 6; 
  
  ilInit ();
  iluInit (); 

  printf   ("%s\n", __FUNCTION__); 

  std::vector<ILuint> source_image (num_files, 0); 
  std::vector<ILuint> work_image (num_files, 0); 

  ilGenImages (num_files, &source_image[0]);
  ilGenImages (num_files, &work_image[0]);

  for (size_t i = 0; i < num_files; ++i) { 
    // construct name
    int ext_pos = strlen (output_files[i]) - 3;
    std::string file_part (output_files[i], output_files[i] + ext_pos - 1);
    printf ( "file_part [%s]\n", file_part.c_str());
    //std::string ext = output_files[i] + ext_pos;

    const std::string fq_name = root_path + "/" + std::string (output_files[i]);
    //std::string fq_name = tex_path + "/" + ext + "/" + file_names[i]; 

    // source
    printf ("loading.. %s\n", fq_name.c_str());
    ilBindImage(source_image[i]);
    if (ilLoadImage(fq_name.c_str())) {

      int image_width    = ilGetInteger (IL_IMAGE_WIDTH); 
      int image_height   = ilGetInteger (IL_IMAGE_HEIGHT);
      int image_depth    = ilGetInteger (IL_IMAGE_DEPTH);
      int bytes_per_pixel= ilGetInteger (IL_IMAGE_BYTES_PER_PIXEL); 
      int image_bpp      = ilGetInteger (IL_IMAGE_BPP);
      int image_type     = ilGetInteger (IL_IMAGE_TYPE);
      int image_format   = ilGetInteger (IL_IMAGE_FORMAT); 

      printf ( "image dim [w:%i, h:%i, d:%i | bpp:%i, bytes:%i, type:%x, format:%x]\n",
               image_width, image_height, image_depth,
               image_bpp, bytes_per_pixel, image_type, image_format); 

      std::vector<rgba> image4 (image_width * image_height, {255, 255, 255, 255}); 

      ILubyte* image_mem = ilGetData (); 
      for (int iy = 0; iy < image_height; ++iy) {
        for (int ix = 0; ix < image_width; ++ix) {
          const int rgb_width = image_width * 3;
          const int pix_ind   = iy * rgb_width + (3 * ix); 
          std::copy (image_mem+pix_ind, image_mem+pix_ind + 3, &image4[iy * image_width + ix][0]); 
        }
      }

      //printf ("R TYPES thE sAME[%i, %i]\n", image_type , IL_UNSIGNED_BYTE);
      ilDeleteImage (source_image[i]); 

      ilBindImage (work_image[i]);

      printf ( "file_part [%s]\n", file_part.c_str());
      const std::string fq_out = root_path + "/" +  file_part + "_rgba.png"; 
      printf ( "fq_out [%s]\n", fq_out.c_str());
    
      bool img_res = ilTexImage (image_width, image_height, image_depth, 4, IL_RGBA, image_type, &image4[0]); 
      if (!img_res) { 
        printf ("img error : %s\n", iluErrorString (ilGetError()));
      }
      
      if (!ilSave (IL_PNG, fq_out.c_str())) { 
        printf ("img error : %s\n", iluErrorString (ilGetError()));
      }

      ilDeleteImage (work_image[i]); 

    }

  }

  

  
  return 0; 
}
