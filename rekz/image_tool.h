
#ifndef REKZ_IMAGE_TOOL_INCLUDE
#define REKZ_IMAGE_TOOL_INCLUDE

#include "rekz.h"


namespace rekz {

  // -------------------------------------------------------------------------------------------
  //
  // -------------------------------------------------------------------------------------------
  struct tileparams {
    glm::uvec2 tiledim;
    glm::uvec2 numtiles;
    bool       overlap; 
  }; 

  // -------------------------------------------------------------------------------------------
  template<typename Ty> 
    using tilecb = int(*)(const imagebuff<Ty>& tilei, uint32 xtile, uint32 ytile, void* up);

  // -------------------------------------------------------------------------------------------
  template<typename Ty> inline int
  iterate_over_tiles (const imagebuff<Ty>& srci, const tileparams& params, tilecb<Ty> cb, void* up) {

    imagebuff<Ty> dsti (params.tiledim.x, params.tiledim.y);
    
    for (uint32 ytile = 0; ytile < params.numtiles.y; ++ytile) {
      for (uint32 xtile = 0; xtile < params.numtiles.x; ++xtile) {
        
        glm::uvec2 begp = { xtile * params.tiledim.x, ytile * params.tiledim.y };
        if (params.overlap) { 
          begp.x -= xtile; // outer rows match
          begp.y -= ytile;
        }
        glm::uvec2 endp = { begp.x + params.tiledim.x, begp.y + params.tiledim.y };

        copy_sub_image (dsti, srci, begp, endp, false); 

        cb (dsti, xtile, ytile, up);
      }}
    
    return 0; 
  }


 
}


#endif
