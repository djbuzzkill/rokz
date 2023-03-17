

// #ifndef REKZ_EXAMINE_TIFFS_INCLUDE
// #define REKZ_EXAMINE_TIFFS_INCLUDE
// //
#include "rekz.h"

#include "mars_files.h"
#include "image_loader.h"
#include "rokz/binary_IO.h"
#include "rokz/file.h"

#include <IL/il.h>
#include <IL/ilu.h>

#include <glm/fwd.hpp>
#include <limits>
//#include "rokz/shared_types.h"

using namespace rokz;


// //  using DevILOpenFileCB =  int(*)(const unsigned char* dat, const DevILImageProps& props, void* up); 
// int big_tif_handler (const unsigned char* dat, const rekz::DevILImageProps& props, void*) {
//   printf ("%s\n ", __FUNCTION__); 
//   printf ("TIF<w:%i, h:%i>\n", props.width ,props.height);

//   return 0; 
// }

const uint32 k_tile_dim = 1024;

// -------------------------------------------------------------------------------------------
//
// -------------------------------------------------------------------------------------------
int make_sample_tile (const Vec<std::string>& args) {

  printf ("%s\n ", __FUNCTION__); 

  using namespace ESP_018065_1975_RED_ESP_019133_1975_RED; 
  
  const uint32 total_image_pixels = kWIDTH * kHEIGHT;
  const uint32 total_tile_pixels = k_tile_dim * k_tile_dim;
  
  std::string      base_path = "/home/djbuzzkill/owenslake/data/Mars/"; 
  std::string      colorname = "ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.6900x17177.dat";
  std::string      fqcolorfile = base_path + colorname; 
  imagebuff<float> colorimage (kWIDTH, kHEIGHT);
  load_from_file (colorimage, fqcolorfile); 
  // From_file (colorimage.dat, fqcolorfile); 

  imagebuff<float>       testtilef   (k_tile_dim, k_tile_dim);
  imagebuff<glm::u8vec3> testgr(k_tile_dim, k_tile_dim); 

  glm::uvec2 begp = {k_tile_dim, k_tile_dim};
  glm::uvec2 endp = {2*k_tile_dim, 2*k_tile_dim };

  copy_sub_image (testtilef, colorimage, begp, endp); 

  const float kDRG_MIN = -0.016220f;

  for (uint32 i = 0; i < total_tile_pixels; ++i) {
    if (testtilef.dat[i] > -2550.0f) {
      uint8 grad = uint8 ((testtilef.dat[i] - kDRG_MIN) * 128.0f);
      testgr.dat[i] = glm::u8vec3 (grad, grad, grad);
    }
    else {
      testgr.dat[i] = glm::u8vec3 (0); 
      }
  }

  ilInit ();
  iluInit (); 
  ILuint testim = ilGenImage ();

  ilBindImage (testim);

  bool img_res = ilTexImage (k_tile_dim, k_tile_dim, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, &testgr.dat[0]); 
  if (!img_res) { 
    printf ("img error : %s\n", iluErrorString (ilGetError()));
  }

  std::string savename = "/home/djbuzzkill/owenslake/tmp/test-DRG-tile.png"; 
  
  ilSave (IL_PNG, savename.c_str());  
  //IL_PNG, 
  ilDeleteImage (testim);
  ilShutDown ();
  
  printf ("%s lolz bai \n", __FUNCTION__);
  return 0;
}


// ------------------------------------------------------------------------------------------
void new_gen_tiles  (const Vec<std::string>& args) {

  using namespace ESP_018065_1975_RED_ESP_019133_1975_RED; 
  
  rekz::imagebuff<float> DRG (kWIDTH, kHEIGHT);

  const uint32 total_num_pixels = kWIDTH * kHEIGHT;

  std::string base_path = "/home/djbuzzkill/owenslake/data/Mars/"; 

  // "ESP_018065_1975_RED_ESP_019133_1975_RED-DEM_f32x6900x17177.bin";
  std::string heightname = "ESP_018065_1975_RED_ESP_019133_1975_RED-DEM.6900x17177.hgt"; 

  // "ESP_018065_1975_RED_ESP_019133_1975_RED-DRG_f32x6900x17177.bin";
  std::string colorname  = "ESP_018065_1975_RED_ESP_019133_1975_RED-DRG.6900x17177.dat";

  // "ESP_018065_1975_RED_ESP_019133_1975_RED-DEM_dvec2x6900x17177.bin";
  std::string coordname  = "ESP_018065_1975_RED_ESP_019133_1975_RED-IGM.dat";

  
  { // color
    std::string colorfile = base_path + colorname; 
    rekz::imagebuff<float> colorimage (kWIDTH , kHEIGHT);
    rokz::From_file (colorimage.dat, colorfile); 

    rekz::imagebuff<float> subim  (k_tile_dim, k_tile_dim);
    rekz::copy_sub_image (subim, colorimage, {0, 0}, {k_tile_dim, k_tile_dim});
  }
  
  { // height
    const uint32 num_x_tiles = kWIDTH  / k_tile_dim;
    const uint32 num_y_tiles = kHEIGHT / k_tile_dim;
    
    std::string heightfile = base_path + heightname; 
    rekz::imagebuff<float> heightimage (kWIDTH , kHEIGHT);
    rokz::From_file (heightimage.dat, heightfile); 

    rekz::imagebuff<float> htile (1024, 1024);

    for (uint32 iy = 0; iy < num_y_tiles; ++iy) {
      for (uint32 ix = 0; ix < num_x_tiles; ++ix) {
        //beg      step    overlap prev
        // |        |        |
        // v        v        v  
        // ix * k_tile_dim - ix
        glm::uvec2 beg = { ix * k_tile_dim - ix, iy * k_tile_dim - iy};
        glm::uvec2 end = { beg.x + k_tile_dim, beg.y + k_tile_dim };
        //
        rekz::copy_sub_image (htile, heightimage, beg, end );

        rokz::WriteStream::Ref ws = nullptr; //rokz::CreateWriteFileStream ( "");
        ws->Write (&htile.el(0, 0), htile.sizebytes()); 
      }}
  }
  
}

// ------------------------------------------------------------------------------------------
// main()
// ------------------------------------------------------------------------------------------
int examine_TIFs (const Vec<std::string>& args) {

  printf ("%s\n ", __FUNCTION__); 

  //ESP_018065_1975_RED_ESP_019133_1975_RED::print_attributes ();
  make_sample_tile  (args);
    //new_gen_tiles  (args);


  
  return 0; 
  
}

// ------------------------------------------------------------------------------------------
// FUNC Make_Mars_tiles 
// ------------------------------------------------------------------------------------------
void Make_Mars_tiles ()
{
#ifdef USE_ORIGINAL_MARS_DATA
  const int         kTile_txr_dim     = 1024;  
   const int         kMars_img_X_dim   = 6900;  //  6900 % 1024 -> 6
   const int         kMars_img_Y_dim   = 17177; //  17177 1024 -> 16          

   const std::string kOut_path         = Mars_test::kOut_path  ; "J:/Quarantine/Mars"; 
   const std::string kBase_fname       = Mars_test::kBase_fname; "/ESP_018065_1975_RED_ESP_019133_1975_RED"; 

   int         num_X_tiles             = kMars_img_X_dim / kTile_txr_dim;
   int         num_Y_tiles             = kMars_img_Y_dim / kTile_txr_dim;

   num_X_tiles += (num_X_tiles * kTile_txr_dim < kMars_img_X_dim) ? 1 : 0; 
   num_Y_tiles += (num_Y_tiles * kTile_txr_dim < kMars_img_Y_dim) ? 1 : 0; 

   int         last_tile_X_partial_pxls   = kMars_img_X_dim % kTile_txr_dim; 
   int         last_tile_Y_partial_pxls   = kMars_img_Y_dim % kTile_txr_dim; 
   const int   num_pxls                   = kMars_img_X_dim * kMars_img_Y_dim; 
   const int   num_txr_pxls               = kTile_txr_dim * kTile_txr_dim; 

   static bool gen_color_flag             = false; 
   static bool gen_height_flag            = false; 
   static bool gen_igm_flag               = false; 
   static bool gen_normal_tiles           = true;

   static bool inspect_tiles_flag         = false; 
   //
   // color
   if (gen_color_flag) 
   {  std::string             kFname_color   = kOut_path + kBase_fname + "-DRG.6900x17177.dat"; 
      std::shared_ptr<FILE>   f              (fopen (kFname_color.c_str(), "rb"), fclose); 
      sh_arr<float>           fdat_          (new float[num_pxls]); 

      ptru srcp =  {fdat_ .get()}; 
      fread (srcp.f, sizeof(float), num_pxls, f.get()); 
   
      for (int iY = 0; iY < num_Y_tiles ; iY++) {

        int tile_start_Y = iY * kTile_txr_dim * kMars_img_X_dim; 
         
        for (int iX = 0; iX < num_X_tiles; iX ++) {
          int tile_start_X     = iX * kTile_txr_dim;

          // is it full row of pixels or partial?
          int num_tile_pixels = (iX == kMars_img_X_dim / kTile_txr_dim ? last_tile_X_partial_pxls : kTile_txr_dim); 
          // .. same for line
          int num_tile_lines   = (iY == kMars_img_Y_dim / kTile_txr_dim ? last_tile_Y_partial_pxls : kTile_txr_dim); 

          // 
          std::string col_fn = color_tile_name (iY, iX); 
          std::shared_ptr<FILE> mars_colorf (fopen(col_fn.c_str (), "wb"), fclose); 
          // default fill with mask val
          for (int i = 0; i < num_txr_pxls; i++) 
            fwrite (&kMaskVal, sizeof(float), 1, mars_colorf.get()); 
          // reset pos            
          fseek (mars_colorf.get(), SEEK_SET, 0); 

          for (int iln = 0; iln < num_tile_lines; iln++) 
            {
              fwrite (
                      srcp.f + tile_start_X + tile_start_Y + iln * kMars_img_X_dim, 
                      sizeof(float) , 
                      num_tile_pixels, 
                      mars_colorf.get()
                      ); 
            }

          mars_colorf.reset(); 
        }
      }
   }

   //
   // height
   if (gen_height_flag) 
   {  
      std::string             kFname_heigh   = kOut_path + kBase_fname + "-DEM.6900x17177.hgt";  
      std::shared_ptr<FILE>   f              (fopen (kFname_heigh.c_str(), "rb"), fclose); 
      sh_arr<float>           fdat_          (new float[num_pxls]); 

      ptru srcp =  {fdat_ .get()}; 
      fread (srcp.f, sizeof(float), num_pxls, f.get()); 

      for (int iY = 0; iY < num_Y_tiles ; iY++)
      {
         int tile_start_Y = iY * kTile_txr_dim * kMars_img_X_dim; 
         
         for (int iX = 0; iX < num_X_tiles; iX ++) 
         {
            int tile_start_X     = iX * kTile_txr_dim;
            int num_tile_pixels  = (iX == kMars_img_X_dim / kTile_txr_dim ? last_tile_X_partial_pxls : kTile_txr_dim); 
            int num_tile_lines   = (iY == kMars_img_Y_dim / kTile_txr_dim ? last_tile_Y_partial_pxls : kTile_txr_dim); 

            std::string hgt_fn = height_tile_name (iY, iX);
            std::shared_ptr<FILE> mars_heightf (fopen(hgt_fn.c_str (), "wb"), fclose); 
            
            for (int i = 0; i < num_txr_pxls; i++)
               fwrite(&kMaskVal, sizeof(float), 1, mars_heightf.get()); 

            fseek (mars_heightf.get(), SEEK_SET, 0); 


            for (int iln = 0; iln < num_tile_lines; iln++) 
            {
               fwrite (
                  srcp.f + tile_start_X + tile_start_Y + iln * kMars_img_X_dim, 
                  sizeof(float) , 
                  num_tile_pixels, 
                  mars_heightf.get()
                  ); 
            }

            mars_heightf.reset(); 

         }
      }
   }



   //
   // gen IGM
   if (gen_igm_flag) 
   {  
      std::string                   kFname_coord   = kOut_path + kBase_fname + "-IGM.dat"; 
      std::shared_ptr<FILE>         f              (fopen (kFname_coord.c_str(), "rb"), fclose); 
      sh_arr<EN_coord>   fdat_          (new EN_coord[num_pxls]); 
      
      ptru coords =  { fdat_ .get() }; 
      fread (coords.v, sizeof(EN_coord), num_pxls, f.get()); 

      for (int iY = 0; iY < num_Y_tiles ; iY++)
      {
         int tile_start_Y = iY * kTile_txr_dim * kMars_img_X_dim; 
         
         for (int iX = 0; iX < num_X_tiles; iX ++) 
         {
            int tile_start_X     = iX * kTile_txr_dim;
            int num_tile_pixels  = (iX == kMars_img_X_dim / kTile_txr_dim ? last_tile_X_partial_pxls : kTile_txr_dim); 
            int num_tile_lines   = (iY == kMars_img_Y_dim / kTile_txr_dim ? last_tile_Y_partial_pxls : kTile_txr_dim); 


            std::string igm_fn = igm_tile_name (iY, iX); 
            std::shared_ptr<FILE> mars_igmf (fopen (igm_fn.c_str (), "wb"), fclose); 
            
            for (int i = 0; i < num_txr_pxls; i++)
            {
               EN_coord _ = { -2553.0, -2553.0 }; 
               fwrite (&_, sizeof EN_coord, 1, mars_igmf.get()); 
            }
            fseek (mars_igmf.get(), SEEK_SET, 0); 

            for (int iln = 0; iln < num_tile_lines; iln++) 
            {
               
               fwrite (
                  coords.en + tile_start_X + tile_start_Y + iln * kMars_img_X_dim, 
                  sizeof(EN_coord), 
                  num_tile_pixels, 
                  mars_igmf.get()
                  ); 
            }

            mars_igmf.reset (); 
         }
      }
   }

   //
   // generate_normal_maps; 
   if (gen_normal_tiles)
   {


   }


   //
   //
   //
   if (inspect_tiles_flag) 
   {

      if (gen_height_flag)
      {
      std::string             hgt_fn = height_tile_name (8, 3);
      sh_arr<float>           hgtp (new float[num_txr_pxls] ); 
      std::shared_ptr<FILE>   hgtf (fopen(hgt_fn.c_str (), "rb"), fclose); 
      fread (hgtp.get(), sizeof(float), num_txr_pxls, hgtf.get()); 
      hgtp.get();
      }

      if (gen_color_flag)
      {
      std::string             col_fn = color_tile_name (8, 3);
      sh_arr<float>           colp (new float[num_txr_pxls] ); 
      std::shared_ptr<FILE>   colf (fopen(col_fn.c_str (), "rb"), fclose); 
      fread (colp.get(), sizeof(float), num_txr_pxls, colf.get()); 
      colp.get(); 
      }

      
      if (gen_igm_flag)
      {
      std::string                   igm_fn = igm_tile_name (10, 4);
      sh_arr<EN_coord>   igmp (new EN_coord[num_txr_pxls]); 
      std::shared_ptr<FILE>         igmf (fopen(igm_fn.c_str (), "rb"), fclose); 
      fread (igmp.get(), sizeof(EN_coord), num_txr_pxls, igmf.get()); 
      igmp.get();

      igm_fn = igm_tile_name (10, 3);
      igmf.reset (fopen(igm_fn.c_str (), "rb"), fclose); 
      fread (igmp.get(), sizeof(EN_coord), num_txr_pxls, igmf.get()); 
      igmp.get();

      igm_fn = igm_tile_name (9, 4);
      igmf.reset (fopen(igm_fn.c_str (), "rb"), fclose); 
      fread (igmp.get(), sizeof(EN_coord), num_txr_pxls, igmf.get()); 
      igmp.get();

      igm_fn = igm_tile_name (9, 3);
      igmf.reset (fopen(igm_fn.c_str (), "rb"), fclose); 
      fread (igmp.get(), sizeof(EN_coord), num_txr_pxls, igmf.get()); 
      igmp.get();

      EN_coord& c0 = igmp [0 * kTile_txr_dim];
      EN_coord& c1 = igmp [0 * kTile_txr_dim + 1023];
      EN_coord& c2 = igmp [1023 * kTile_txr_dim + 1023];
      EN_coord& c3 = igmp [1023 * kTile_txr_dim];
      igmp.get();

      }
   }

#endif

}



