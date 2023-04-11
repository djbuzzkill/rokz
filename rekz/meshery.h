//
#ifndef REKZ_MESHERY_INCLUDE
#define REKZ_MESHERY_INCLUDE

#include "rokz/rokz.hpp"

namespace rekz {

  using namespace rokz;

  namespace geom {
    // ----------------------------------------------------------------------------------------------
    //                           
    // ----------------------------------------------------------------------------------------------
    template<typename VTy, typename IndTy>
    struct GeometryData {

      typedef  VTy         VertexType; 
      typedef  IndTy       IndexType; 

      enum { VertexSize = sizeof(VTy) }; 
      enum { IndexSize  = sizeof(IndTy) }; 
  
      Vec<VTy>   verts;
      Vec<IndTy> indices;
    }; 



    // ----------------------------------------------------------------------------------------------
    //                           
    // ----------------------------------------------------------------------------------------------
    template<typename VTy, typename IndTy> inline size_t
    ComputeVertexSize (const rekz::geom::GeometryData<VTy, IndTy>& geom) {

      return geom.verts.size () * rekz::geom::GeometryData<VTy, IndTy>::VertexSize ; 
    }
    // ----------------------------------------------------------------------------------------------
    //                           
    // ----------------------------------------------------------------------------------------------
    template<typename VTy, typename IndTy> inline size_t
    ComputeIndexSize (const rekz::geom::GeometryData<VTy, IndTy>& geom) {

      return geom.indices.size () * rekz::geom::GeometryData<VTy, IndTy>::IndexSize; 
    }

    // ----------------------------------------------------------------------------------------------
    //                           
    // ----------------------------------------------------------------------------------------------
    template<typename VTy, typename IndTy> inline size_t
    ComputeTotalSize (const rekz::geom::GeometryData<VTy, IndTy>& geom) {
      return ComputeIndexSize (geom) + ComputeVertexSize (geom); 
    }
    
    // ----------------------------------------------------------------------------------------------
    //                           
    // ----------------------------------------------------------------------------------------------
    template<typename VTy> 
    using TriMesh = GeometryData<VTy, uint16_t>; 
  

  }}


#endif
