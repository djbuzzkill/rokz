
#ifndef REKZ_FONT_TOOL_INCLUDE
#define REKZ_FONT_TOOL_INCLUDE


#include "rekz.h"

namespace rekz { namespace fonttool { 

    // --
    extern const char kGlyphAttributeFilename[];

    // --
    const size_t kGlyphAttributeArrayCount = 128;

    // --
    struct glyphattributes {
      glm::ivec2 advance;
      glm::ivec2 dim; 
      glm::ivec2 hbearing;
      glm::ivec2 vbearing;
    }; 

    // -- only between (33, '!')  and (126, '~') ---
    std::string font_glyph_filename (uint32 asciicode); 

    // -- 
    void BuildFont (const char* fontFile, const rokz::systempath outputpath);
    
  }} // namespace 

#endif
