
#ifndef REKZ_FONT_TOOL_INCLUDE
#define REKZ_FONT_TOOL_INCLUDE


#include "rekz.h"

namespace rekz { namespace fonttool { 


    struct glyphattributes {
      glm::ivec2 advance;
      glm::ivec2 dim; 
      glm::ivec2 hbearing;
      glm::ivec2 vbearing;
    }; 

    const size_t kGlyphAttributeArrayCount = 128;
    extern const char kGlyphAttributeFilename[];
    
    // 
    std::string font_glyph_filename (uint32 asciicode); 
    void BuildFont (const char* fontFile, rokz::systempath outputpath);
    
  
  }}

#endif
