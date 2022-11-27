
#include "rockz.h"              // 
#include "rokz/rokz.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

#include "rokz/defaults.h"
//#include "rokz/rokz_funcs.h"

#include <IL/il.h>
#include <IL/ilu.h>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
int texture_tool (const std::vector<std::string>& args) {

  ilInit ();
  iluInit (); 

  printf   ("%s\n", __FUNCTION__); 

  ILuint image_name = 0; 

  const std::string tex_path = "/home/djbuzzkill/owenslake/data/textures"; 

  const char* file_names[] = {
  //   "abstract-texture-3.jpg",
  //   "blue-texture-image-hd.jpg",
  //   "bokeh_texture.jpg",
  //   "burlgrunge.jpg",
  //   "silkpurple.jpg",
  //   "sky_blue.jpg",
  //   "texture_123.jpg",
    //    "phoenix_feathers.png", 
    //    "mb3d_spaceship.png",
    //"redstar.png",
    "blue-texture-image-hd.jpg",
    "abstract-texture-3.jpg",
    "mb3d_exp.png",
    "fuerte-stone-1.jpg", 
    "fuerte-stone-2.jpg", 
    "solid_waves.png",
  };

  
  size_t num_files = sizeof (file_names) / sizeof (const char*); 
  

  for (size_t  i = 0; i < num_files; ++i) {
    printf ( "filename: %s/%s \n", tex_path.c_str(), file_names[i] ); 
  }

  
  std::vector<ILuint> image_names (num_files, 0); 
  std::vector<ILuint> work_names (num_files, 0); 

  ilGenImages (num_files, &image_names[0]);
  ilGenImages (num_files, &work_names[0]);

  for (size_t i = 0; i < num_files; ++i) { 

    // construct 
    std::string fq_name = tex_path+ "/" + file_names[i]; 

    ilBindImage(image_names[i]);

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
      // find smaller of dims
      int min_dim = std::min (image_width, image_height); 
      printf ("min_dim %i\n", min_dim); 

      ilBindImage (work_names[i]); 

      bool img_res = ilTexImage (min_dim, min_dim, image_depth,
                                 bytes_per_pixel, image_format, image_type,
                                 nullptr); 
      if (!img_res) { 
        printf ("img error : %s\n", iluErrorString (ilGetError()));
      }

      bool blit_res = ilBlit (image_names[i],
                              0, 0, 0,
                              0, 0, 0,
                              min_dim, min_dim, image_depth);                   
      
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

      printf ("work_image[%i, %i, %i | bpp:%i, byte:%i, t:%x, f:%x] \n",
              work_width, work_height, work_depth,
              work_bpp, work_bytes_per_pix,
              work_type, work_format); 

      char save_filename[256]; 
      sprintf (save_filename, "%s/out_%i_%s", tex_path.c_str(), i, file_names[i]);
      printf ("   save_filename %s\n\n\n", save_filename); 
      ilSave (IL_PNG, save_filename);  
      //      iluImageParameter(ILU_FILTER, ILU_SCALE_BELL);
      
      //      iluScale(ILuint Width, ILuint Height, ILuint Depth)


   // do shit
    }
    else {
      printf ("failed: [%zu] %s\n", i, fq_name.c_str()); 
    }    

    
  }

  ilDeleteImages (num_files, &image_names[0]);
  printf   ("%s BAI\n", __FUNCTION__); 
  return 0; 
}
