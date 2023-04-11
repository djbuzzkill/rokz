
#ifndef ROKZ_IMAGEBUFF_INCLUDE 
#define ROKZ_IMAGEBUFF_INCLUDE

#include "common.hpp"
#include "shared_types.hpp"


namespace rokz {

  // ------------------------------------------------------------------------------------------
  // 
  // ------------------------------------------------------------------------------------------
  template<typename Ty> struct imagebuff {

    imagebuff () : imagebuff (256, 256) {
    }

    imagebuff (uint32 xdim, uint32 ydim)
      : width (xdim), height (ydim), dat (xdim * ydim) {
    }

    typedef Ty  pixel_type;

    uint32      width;
    uint32      height;
    Vec<Ty>     dat;

    size_t numpix () const {
      return dat.size (); }

    size_t numbytes () const {
      return sizeof(pixel_type) * numpix (); }
    
    void resize (uint32 w, uint32 h) {
      width = w;
      height = h;
      dat.resize (w * h);
    }

    
    size_t index2 (size_t x, size_t y) const {
      return width * y + x; 
    }

    Ty& el (size_t x, size_t y) {
      return dat[index2 (x, y)]; 
    }

    const Ty& el (size_t x, size_t y) const {
      return dat[index2 (x, y)]; 
    }

    const Ty* p (size_t x, size_t y) const {
      return &dat[index2 (x, y)]; 
    }
    
    Ty* p (size_t x, size_t y) {
      return &dat[index2 (x, y)]; 
    }

    const Ty* p () const {
      return &dat[0]; 
    }
    
    Ty* p () {
      return &dat[0]; 
    }
  };


  // ----------------------------------------------------------
  //
  // ----------------------------------------------------------
  template<template<typename> class ImTy, typename Ty> inline imagebuff<Ty>&
  load_from_file (ImTy<Ty>& outi, const std::string& fqname) {
    From_file (outi.dat, fqname, false); 
    return outi;
  }

  // ------------------------------------------------------------------------------------------
  // !! doesnt necessarily depend on *imagebuff* 
  // ------------------------------------------------------------------------------------------
  template<template<typename> class ImTy, typename Ty> inline imagebuff<Ty>&
  copy_sub_image (ImTy<Ty>& dsti, const ImTy<Ty>& srci, const glm::uvec2& begp, const glm::uvec2& endp) {
    return copy_sub_image (dsti, srci, begp, endp, true);
  }

  // ------------------------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------------------------
  template<template<typename> class ImTy, typename Ty> inline imagebuff<Ty>&
  copy_sub_image (ImTy<Ty>& dsti, const ImTy<Ty>& srci, 
                  const glm::uvec2& begp, const glm::uvec2& endp, bool resize) {

    assert (begp.x < endp.x);
    assert (begp.y < endp.y);

    glm::uvec2 dim = endp - begp;
    //printf ("--> subimage dim <%u, %u>\n", dim.x, dim.y);

    if (resize)  
      dsti.resize (dim.x, dim.y);

    uint32 total_pixs = dim.x * dim.y;
    if (dsti.numpix () < total_pixs) {
      printf (" --> failed size check  dim <%u, %u>, resize(%s)\n",
              dim.x, dim.y,
              (resize ? "true" : "false"));
      return dsti; 
    }

    for (uint32 iy = begp.y; iy < endp.y; ++iy) {
      uint32 ydist = iy - begp.y;
      std::copy (srci.p(begp.x, iy), srci.p(begp.x+dim.x, iy), dsti.p(0, ydist));   
    }

    return dsti;
  }
  
  // ------------------------------------------------------------------------------------------
}

#endif

