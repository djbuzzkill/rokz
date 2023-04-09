
//#define FT_GLYPH_H
//#include  <freetype2/freetype/
#include "rekz.h"
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
void BuildFont (const char* fontFile, char* texturename, char* outName) {

  (void) texturename; 

  
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

  
  
  /** *************************************************
   * Collect glyphs and render onto output memory.  conventions 
   * assumed: 16x16 cell texture, 512x512 texture, uv from upper-right
   ***************************************************/
  //FT_UInt glyph_index;
  // ! and ~, start and end of ascii printable range

  for (char ch = '!' ; ch <=  '~'; ch++) {

    FT_UInt glyph_index = FT_Get_Char_Index (ftFace, ch);
    res = FT_Load_Glyph (ftFace, glyph_index, FT_LOAD_DEFAULT);
    res = FT_Render_Glyph (ftFace->glyph, FT_RENDER_MODE_NORMAL);

    
    rokz::imagebuff<uint8> image  (64, 64);

    uint32 advance = (ftFace->glyph->advance.x >> 6);

    int metric_width =  ftFace->glyph->metrics.width >> 6;
    int metric_height = ftFace->glyph->metrics.height >> 6;

    printf (" [char:%c | ascii:%i | glyph_index:%u \n" , ch, ch, glyph_index); 

    // printf ( " [%c] vertica<bearing x %i, bearing y %i advance %i>\n" , ch, 
    //          ftFace->glyph->metrics.vertBearingX >> 6,
    //          ftFace->glyph->metrics.vertBearingY >> 6,
    //          ftFace->glyph->metrics.vertAdvance >> 6);

    //    printf ("[%c] metric<width:%i, height:%i>\n", ch, metric_width , metric_height);

        // typedef struct  FT_Glyph_Metrics_
        // {
        //   FT_Pos  width;
        //   FT_Pos  height;
    
        //   FT_Pos  horiBearingX;
        //   FT_Pos  horiBearingY;
        //   FT_Pos  horiAdvance;
    
        //   FT_Pos  vertBearingX;
        //   FT_Pos  vertBearingY;
        //   FT_Pos  vertAdvance;

    // } FT_Glyph_Metrics;

    // printf ("[%c] bitmap.width [%u, %u]\n", ch, ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows);
    // printf ("[%c] bitmap.pitch %u | advance %u\n", ch, ftFace->glyph->bitmap.pitch, advance);
    
    
    
    unsigned char* pPixel = ftFace->glyph->bitmap.buffer;
    for (uint32 irow = 0; irow < ftFace->glyph->bitmap.rows; irow++) {
      for (uint32 icol = 0; icol < ftFace->glyph->bitmap.width; icol++)  {

        uint32 invert_y = 63 - irow ;
        
        image.el(icol, invert_y) = pPixel [(irow * ftFace->glyph->bitmap.pitch) + icol];

        //data[CHAR_PIXEL_SIZE * irow + icol]  = pPixel [(irow * ftFace->glyph->bitmap.pitch) + icol];
      }
    }

    save_to_png (image, ch); 
    save_to_bin (image, ch, k_base_bin_path); 
  }

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

  BuildFont (fqname.c_str(), nullptr, nullptr);
  
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




	
	 
	// prep for write
	// texturBlok.flags  = ImageBitUsageColor; 
	// texturBlok.format = ImageFormatUC; 
	// texturBlok.reso   = 8; 
	// texturBlok.wd = 16 * CHAR_PIXEL_SIZE;
	// texturBlok.ht = 16 * CHAR_PIXEL_SIZE;
	// texturBlok.data = pData; 


	// and material
	//mtrlBlok.diff[0] = 1.0f;
	//mtrlBlok.diff[1] = 1.0f;
	//mtrlBlok.diff[2] = 1.0f;
	//mtrlBlok.diff[3] = 1.0f;


	//mtrlBlok.spec[0]  = 1.0f;
	//mtrlBlok.intensity = 1.0f;
	//mtrlBlok.expnt = 1.0f;
	//mtrlBlok.ambimod = 1.0f;
	//mtrlBlok.emismod = 1.0f;
	//mtrlBlok.alpha = 1.0f;
	// mtrlBlok.txr_flags = 0x1;
	// mtrlBlok.tx_stage[0].resolve_ind = 0; // cus we know -> blok_arr[] = { &texturBlok, &mtrlBlok, &fontBlok };
	// mtrlBlok.tx_stage[0].u_edge = SamplerWrapRepeat;
	// mtrlBlok.tx_stage[0].v_edge = SamplerWrapRepeat;
	// mtrlBlok.tx_stage[0].blend_mode = 0;
	// mtrlBlok.tx_stage[0].map_to = 0;
	// mtrlBlok.tx_stage[0].tx_crd = 0;

	// now the texture
	// fontBlok.resolve_mtrl_ind = 1; // cus we know -> blok_arr[] = { &texturBlok, &mtrlBlok, &fontBlok };

	// sprintf (texturBlok.label, "%s%s", textureName, "Txr");
	// sprintf (fontBlok.label, "%s%s", textureName, "Font");
	// sprintf (mtrlBlok.label, "%s%s", textureName, "Mtrl");


	//sparse_hash_map<ResourceBlock*, int>::iterator blok_begin = blok_hash.begin();
	//while (blok_begin != blok_hash.end())
	//{
	//	printf ( "blok label %s\n", blok_begin->first->label);
	//	++blok_begin;
	//}

	// write to file

        // 	FILE* pFile;
// 	char buff[128];
// 	sprintf (buff, "%s.%s", outName, "e9");
	
// 	pFile = fopen (buff, "wb");

// 	if (pFile)
// 	{
//           fwrite_e9saveendian (pFile);
//           // we know its 3 entries
//           int entry_offsets[3];
//           unsigned entry_table_start; 

//           fwrite_u32 (pFile, 3);
//           entry_table_start = ftell (pFile);
//           fwrite_u32 (pFile, 0);
//           fwrite_u32 (pFile, 0);
//           fwrite_u32 (pFile, 0);

//           for (int i = 0; i < 3; i++)
//             {
//               entry_offsets[i] = ftell (pFile);
//               blok_arr[i]->Write (pFile);
//             }

//           fseek (pFile, entry_table_start, SEEK_SET);
//           for (int i = 0; i < 3; i++)
//             {
//               fwrite_u32 (pFile, entry_offsets[i]);
//             }
//           fclose (pFile);
// 	}
// 	else
// 	{
// 		Assert (0, "coudl not open %s for writing \n", buff);
// 	}

	
// 	delete[] pData;
// 	texturBlok.data = 0;
// }


// /** ************************************************************
//  * 
//  **************************************************************/
// int main (int argv, char** argc)
// {
// 	Debug ( "\nNOTE: this tool is a complete hack" );
// 	BuildFont ("C:\\Quarantine\\awsum\\fonts\\FortuneCity.ttf",  "FortuneCity", "FortuneCity");
// 	return 0;
// }
// /** ************************************************************
//  * 
//  **************************************************************/
// void RenderGlyph (unsigned char* place, int cellX, int celly, FT_Bitmap* bitmap)
// {

// }

// int NumElemsPerPixel[] = {
// 	1, 2, 3, 4,
// 	1, 2, 3 
// };
// /** ************************************************************
//  * 
//  **************************************************************/
// int WriteBlock (FILE* file, TextureResBlock* block) 
// {
// 	int sz_written  = 0;
// 	// ID_TEXTURE
// 	sz_written += fwrite_u32 (file, ID_TEXTURE);
// 	// just use filename
// 	sz_written += fwrite_label (file, block->label);
// 	// 	Flags - color bit for now
// 	sz_written += fwrite_u32 (file, block->flags);
// 	// Format - 
// 	sz_written += fwrite_u32 (file, block->format);
// 	//Depth
// 	sz_written += fwrite_u32 (file, block->reso);
// 	//Width 
// 	sz_written += fwrite_u32 (file, block->wd);
// 	//Height
// 	sz_written += fwrite_u32 (file, block->ht);
// 	//data

// 	Assert (0 == (block->reso % 8), "not a multiple of 8 (byte)");
// 	int bytesPerPixel = block->reso / 8;
// 	int numBytes  = block->wd * block->ht* NumElemsPerPixel[block->format];
																								 
// 	unsigned char* bytePtr = (unsigned char*) block->data;					
// 	for (int i = 0; i < numBytes; i++)
// 	{
// 		fwrite_u8 (file, bytePtr[i] );
// 		sz_written++;
// 	}
	
// 	return sz_written;
// }


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
