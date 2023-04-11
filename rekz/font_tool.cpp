
//#define FT_GLYPH_H
//#include  <freetype2/freetype/
#include "font_tool.h"

#include "IL/il.h"


#include "ft2build.h"
#include "rokz/binary_IO.h"
#include <numeric>
#include FT_FREETYPE_H
#include  FT_GLYPH_H

using namespace rokz;
// ---------------------------------------------------------------------------------------------
// these macros r old
// ---------------------------------------------------------------------------------------------
#define CHAR_PIXEL_SIZE 32
#define NUM_X_CELLS 16
#define NUM_Y_CELLS 16


const char rekz::fonttool::kGlyphAttributeFilename[] = "glyph_attributes.bin";
// ---------------------------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------------------------
const char k_font_glyph_file_format[64] =  "font_glyph_%u.bin";

const rokz::systempath k_base_bin_path =  "/home/djbuzzkill/owenslake/tmp/textbin/";

std::string rekz::fonttool::font_glyph_filename (uint32 asciicode ) {
  char font_glyph_file_name [64]; 
  sprintf (font_glyph_file_name , k_font_glyph_file_format, asciicode);
  return font_glyph_file_name;
}

// ---------------------------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------------------------
void save_to_bin (const rokz::imagebuff<uint8>& image, uint32 asciicode, const rokz::systempath binpath) {

  uint8 maxv = *std::max_element (image.dat.begin () , image.dat.end () );
  uint8 minv = *std::min_element (image.dat.begin () , image.dat.end () );
  printf (" %s --> <minv:%u, maxv:%u>\n", __FUNCTION__, minv, maxv); 

  rokz::imagebuff<float> fimg (image.width, image.height); 

  for (uint32 ipix = 0; ipix < image.numpix () ; ++ipix) {
    fimg.dat[ipix] = float (image.dat[ipix]) / float(255);
  }

  std::string fqname = binpath/rekz::fonttool::font_glyph_filename (asciicode) ;
  rokz::WriteStream::Ref ws = CreateWriteFileStream (fqname.c_str ());
  ws->Write (fimg.p (), fimg.numbytes ()); 
}

// ---------------------------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------------------------
void save_to_png (const rokz::imagebuff<uint8>& image, uint32 asciicode ) {
  // void sample_output (void* data, char* outname) {
  ilInit ();

  const rokz::systempath basepath =  "/home/djbuzzkill/owenslake/tmp/testtext/";


 int imgID = ilGenImage ();
  ilBindImage (imgID);
  ilEnable (IL_ORIGIN_SET);
  ilSetInteger (IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT);

  ILenum  ret_val = ilGetError ();
  if (IL_NO_ERROR != ret_val) {
    printf ( "il Error : %i\n", ret_val); }
  ret_val = ilGetError ();
  if (IL_NO_ERROR != ret_val) {
    printf ( "il Error : %i\n", ret_val); 
  }

  char msg[256];
  sprintf (msg, "test_char_%u.png", asciicode);


  std::string fqname = basepath/msg;
  Vec<uint8> savebuf = image.dat;
  // sample output 
  ilTexImage ( image.width, image.height, 1, 1, IL_LUMINANCE, IL_UNSIGNED_BYTE, &savebuf[0] ); // 

  ret_val = ilGetError ();
  if (IL_NO_ERROR != ret_val) {
    printf ( "il Error : %i\n", ret_val); 
  }

  ilSave (IL_PNG, fqname.c_str ());

  ret_val = ilGetError ();
  if (IL_NO_ERROR != ret_val) {
    printf ( "il Error : %i\n", ret_val); 
  } // end sample out put 

  ilShutDown ();
}

// -------------------------------------------------------------------------------------- 
//
// -------------------------------------------------------------------------------------- 
void rekz::fonttool::BuildFont (const char* fontFile, rokz::systempath outputpath) {

  
  unsigned int res = 0;

  FT_Library ftLib;
  FT_Face ftFace;

  res = FT_Init_FreeType (&ftLib);
  res = FT_New_Face (ftLib, fontFile, 0, &ftFace); 

  res = FT_Set_Pixel_Sizes ( ftFace, 0, 64); 
  //res = FT_Set_Char_Size (ftFace, 0, 0); 

  int pxlsPerChar = CHAR_PIXEL_SIZE * CHAR_PIXEL_SIZE;
  //unsigned char* pData = new unsigned char[pxlsPerChar * 256];
  //memset (pData, 0, pxlsPerChar * 256);

  std::array<glyphattributes, kGlyphAttributeArrayCount> glyphattrs = {}; 

  //
  // ------ Collect glyphs and render onto output memory.  conventions 
  //
  //FT_UInt glyph_index;
  // ! and ~, start and end of ascii printable range
  for (char ch = '!' ; ch <=  '~'; ch++) {

    FT_UInt glyph_index = FT_Get_Char_Index (ftFace, ch);
    res = FT_Load_Glyph (ftFace, glyph_index, FT_LOAD_DEFAULT);
    res = FT_Render_Glyph (ftFace->glyph, FT_RENDER_MODE_NORMAL);

    rokz::imagebuff<uint8> image  (64, 64);

    uint32 glyph_advance_x = (ftFace->glyph->advance.x >> 6);
    uint32 glyph_advance_y = (ftFace->glyph->advance.y >> 6);

    int32 metric_width =  ftFace->glyph->metrics.width >> 6;
    int32 metric_height = ftFace->glyph->metrics.height >> 6;

    int32 hbear_x = ftFace->glyph->metrics.horiBearingX >> 6;
    int32 hbear_y = ftFace->glyph->metrics.horiBearingY >> 6;

    int32 vbear_x = ftFace->glyph->metrics.vertBearingX >> 6;
    int32 vbear_y = ftFace->glyph->metrics.vertBearingY >> 6; 
        
    printf ( "[char:%c | ascii:%i | glyph_index:%u] \n"
             "  horizo_bearing [x: %i, y: %i, advance: %i]\n"
             "  vertica_bearing [x: %i, y: %i, advance: %i]\n"
             "  glyph_advance [x:%u, y:%u]\n"
             "  metric_dim [width:%u, height:%u]\n"
             "  bitmap [width:%u, rows:%u, pitch:%i ]\n\n",
             ch, ch, glyph_index, 
             hbear_x, hbear_y, ftFace->glyph->metrics.horiAdvance >> 6, 
             vbear_x, vbear_y, ftFace->glyph->metrics.vertAdvance >> 6, 
             glyph_advance_x, glyph_advance_y, 
             metric_width , metric_height, 
             ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows, ftFace->glyph->bitmap.pitch); 

        
    glyphattrs[ch] ={
      { glyph_advance_x, glyph_advance_y },
      { metric_width , metric_height }, 
      { hbear_x, hbear_y }, 
      { vbear_x , vbear_y }, 
    }; 

    
    unsigned char* pPixel = ftFace->glyph->bitmap.buffer;
    for (uint32 irow = 0; irow < ftFace->glyph->bitmap.rows; irow++) {
      for (uint32 icol = 0; icol < ftFace->glyph->bitmap.width; icol++) {

        uint32 invert_y = 63 - irow ;
        image.el(icol, invert_y) = pPixel [(irow * ftFace->glyph->bitmap.pitch) + icol];
        //data[CHAR_PIXEL_SIZE * irow + icol]  = pPixel [(irow * ftFace->glyph->bitmap.pitch) + icol];
      }
    }

    // save_to_png (image, ch); 
    // save_to_bin (image, ch, outputpath);
    
  }


  std::string fqattribefilename = k_base_bin_path/kGlyphAttributeFilename;
  WriteStream::Ref ws_attribs  = rokz::CreateWriteFileStream (fqattribefilename) ; 
  ws_attribs->Write ( &glyphattrs[0], kGlyphAttributeArrayCount *  sizeof(glyphattributes)); 
  ws_attribs.reset (); 

}



// -------------------------------------------------------------------------------------- 
//
// -------------------------------------------------------------------------------------- 

int font_tool (const Vec<std::string>& args) {

  FT_Library library;
  FT_Glyph   glyph;
  FT_Face   face;

  auto error = FT_Init_FreeType (&library);
  if (error) {
      //FT_Load_Glyph (glyph);
    return __LINE__;
  }

  HERE("FreeType Initialized");
  systempath basepath = "/usr/share/fonts/liberation/";
  systempath font_list[]  = {
    "LiberationSans-Regular.ttf"    ,
    "LiberationSerif-Regular.ttf"   ,
    "LiberationMono-BoldItalic.ttf" ,
    "LiberationMono-Bold.ttf"       ,
    "LiberationMono-Italic.ttf"     ,
    "LiberationSans-BoldItalic.ttf" ,
    "LiberationSans-Bold.ttf"       ,
    "LiberationSans-Italic.ttf"     ,
    "LiberationSans-Regular.ttf"    ,
    "LiberationSerif-BoldItalic.ttf",
    "LiberationSerif-Bold.ttf"      ,
    "LiberationSerif-Italic.ttf"    ,
  };
  

  std::filesystem::path  fqname = basepath/font_list[0];

  rekz::fonttool::BuildFont (fqname.c_str(), k_base_bin_path);
  
  error = FT_New_Face (library, fqname.c_str(), 0,
                     &face);
  if (error == FT_Err_Unknown_File_Format ) {
    HERE("FT_Err_Unknown_File_Format");
  }

  HERE("FACE CREATED");

  printf ( "num_glyphs:%i\n", face->num_glyphs);
  printf ( "num_fixed_sized:%u\n", face->num_fixed_sizes);
  printf ( "num_available:%u\n", face->available_sizes);
  printf ( "num_charmaps:%u\n", face->num_charmaps);

  if  (FT_HAS_FIXED_SIZES (face)) HERE("HAS FIXED SIZES");
  if  (FT_HAS_GLYPH_NAMES (face)) HERE("FT_HAS_GLYPH_NAMES");
  if  (FT_IS_FIXED_WIDTH (face)) HERE("FT_IS_FIXED_WIDTH");
  
  if  (FT_IS_SCALABLE (face)) HERE("FT_IS_SCALABLE");
   
  return 0;
}





void sample_output (void* data, char* outname)
{
  ilInit ();

  int imgID = ilGenImage ();
  ilBindImage (imgID);
  ilEnable (IL_ORIGIN_SET);
  ilSetInteger (IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT);

  ILenum  ret_val = ilGetError ();
  if (IL_NO_ERROR != ret_val) {
    printf ( "il Error : %i\n", ret_val); }
  ret_val = ilGetError ();
  if (IL_NO_ERROR != ret_val) {
    printf ( "il Error : %i\n", ret_val); 
  }


  // sample output 

  ilTexImage (16 * CHAR_PIXEL_SIZE, 16 * CHAR_PIXEL_SIZE, 1,
              1, IL_LUMINANCE, IL_UNSIGNED_BYTE, data);

  ret_val = ilGetError ();
  if (IL_NO_ERROR != ret_val) {
    printf ( "il Error : %i\n", ret_val); 
  }

  ilSave (IL_PNG, outname);

  ret_val = ilGetError ();
  if (IL_NO_ERROR != ret_val) {
    printf ( "il Error : %i\n", ret_val); 
  } // end sample out put 

  ilShutDown ();
}


//using google::sparse_hash_map;
