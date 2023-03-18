
#ifndef REKZ_IMAGE_TOOL_INCLUDE
#define REKZ_IMAGE_TOOL_INCLUDE

#include "rekz.h"


namespace rekz {

  // -------------------------------------------------------------------------------------------
  //
  // -------------------------------------------------------------------------------------------
  struct iteratetileparams {
    glm::uvec2 tiledim;
    glm::uvec2 numtiles;
    bool       overlap; 
  }; 

  template<typename Ty> struct TileCB {

    typedef std::shared_ptr<TileCB> Ref;
    //virtual imagebuff<Ty>& Buffer () = 0; 
    virtual int Exec (const imagebuff<Ty>& tilei, uint32 xtile, uint32 ytile) = 0; 

  protected:
    TileCB () {}
  };

  // -------------------------------------------------------------------------------------------
  template<typename Ty> inline int
  iterate_over_tiles (const imagebuff<Ty>& srci, const iteratetileparams& params, typename TileCB<Ty>::Ref cb) {

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

        cb->Exec (dsti, xtile, ytile);
      }}
    
    return 0; 
  }

 
}


#endif
