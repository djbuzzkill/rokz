

#include "darkrootgarden.h"

#include <IL/il.h>
#include <IL/ilu.h>


namespace darkroot {
  

  const DarkrootVert OctohedronVerts[] = {
    // top 
    {{0.0, -1.0, 0.0 }, { 0.0, -1.0, 0.0 }, {0.2, 0.3, 1.0 }, {0.5, 0.5}}, 

    {{ 0.0, 0.0,  1.0 }, {-0.707106, 0.0,  0.707106}, {0.2, 0.3, 0.8}, {0.0, 1.0}}, 
    {{ 1.0, 0.0,  0.0 }, { 0.707106, 0.0,  0.707106}, {0.2, 0.3, 0.8}, {1.0, 1.0}}, 
    {{ 0.0, 0.0, -1.0 }, { 0.707106, 0.0, -0.707106}, {0.2, 0.3, 0.8}, {1.0, 0.0}}, 
    {{-1.0, 0.0,  0.0 }, {-0.707106, 0.0, -0.707106}, {0.2, 0.3, 0.8}, {0.0, 0.0}}, 

    // bottom
    {{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, {0.2, 0.3, 1.0 }, {0.5, 0.5 }}, 
    
  }; 

  const size_t kOctohedronVertexCount = sizeof(OctohedronVerts) / sizeof (DarkrootVert);
  static_assert (kOctohedronVertexCount == 6, "");

  // CW
  const uint16_t OctohedronIndices[] = {
    0, 2, 1,  0, 3, 2,  0, 4, 3,  0, 1, 4,
    5, 1, 2,  5, 2, 3,  5, 3, 4,  5, 4, 1
  }; 

  // CCW 
  // const uint16_t OctohedronIndices[] = {
  //   0, 1, 2,  0, 2, 3,  0, 3, 4,  0, 4, 1,
  //   5, 2, 1,  5, 3, 2,  5, 4, 3,  5, 1, 4
  // }; 

  const size_t kOctohedronIndicesCount = sizeof(OctohedronIndices) / sizeof (uint16_t); 

  static_assert (kOctohedronIndicesCount == 24, "");

  // ------------------------------------------------------------------------
  //
  // ------------------------------------------------------------------------
  const DarkrootMesh& DarkOctohedron () {

    static DarkrootMesh octo;
    if (octo.verts.size () == 0) { 
      octo.verts.assign  (OctohedronVerts, OctohedronVerts + kOctohedronVertexCount); 
      octo.indices.assign(OctohedronIndices, OctohedronIndices + kOctohedronIndicesCount);
    }
    return octo;
  }

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  const DarkrootVert dark_verts[] = {
    {{-0.7f, -0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.7f, -0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{ 0.7f,  0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.7f,  0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
                                      
    {{ 0.0f, -1.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{ 1.0f,  0.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{ 0.0f,  1.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{ -1.0f, 0.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
  };

  const uint16_t dark_indices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
  };

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------

  const DarkrootVert dark_cube[] = {
    {{-0.7f, -0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.7f, -0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{ 0.7f,  0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.7f,  0.7f, 0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{ 0.0f, -1.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{ 1.0f,  0.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{ 0.0f,  1.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{ -1.0f, 0.0f, -0.1f}, {0.0f,-1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
  };

  const uint16_t cube_indices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
  };


  //
  // brep is associated with a geom
  HalfEdge::BRep& BuildBoundaryRep (HalfEdge::BRep& brep, const DarkrootMesh& geom) {

    auto nverts = geom.verts.size ();
    auto ninds = geom.indices.size ();

    if (ninds % 3) {
      printf ("[%s] --> TriMesh should be:0\n", __FUNCTION__); 
      return brep;
    }

    auto nfaces = ninds / 3;
  
    brep.edgei.resize (nverts, HalfEdge::NullIndex);
    brep.edges.resize (nfaces * 3);
    brep.faces.resize (nfaces);

    auto edge_count = 0; 
    auto index_count = 0;
  
    for (size_t iface = 0; iface < nfaces; ++iface) {
      //geom.verts[
      // geom.indices[index_count+0]
      // geom.indices[index_count+1]
      // geom.indices[index_count+2]
      brep.faces[iface].edgei = edge_count+0;
      brep.edges[edge_count+0].facei = iface;
      brep.edges[edge_count+0].verti = geom.indices[index_count+0];
      brep.edges[edge_count+0].nexti = edge_count + 1;
      brep.edges[edge_count+0].pairi = HalfEdge::NullIndex; //<-- we dont know yet
        
      brep.edges[edge_count+1].facei = iface;
      brep.edges[edge_count+1].verti = geom.indices[index_count+1];
      brep.edges[edge_count+1].nexti = edge_count + 2;
      brep.edges[edge_count+1].pairi = HalfEdge::NullIndex; // <-- we dont know yet
      
      brep.edges[edge_count+2].facei = iface;
      brep.edges[edge_count+2].verti = geom.indices[index_count+2];
      brep.edges[edge_count+2].nexti = edge_count + 0;
      brep.edges[edge_count+2].pairi = HalfEdge::NullIndex; // <-- we dont know yet
      // verti : vertex at the end of the half-edge 
      // pairi : oppositely oriented adjacent half-edge 
      // facei : face the half-edge borders
      // nexti : next half-edge around the face
      edge_count  += 3;
      index_count += 3; 
    }

    // find pair 
    for (size_t iedge = 0; iedge < brep.edges.size (); ++iedge) {
      // make sure this isnt paried
      if (brep.edges[iedge].pairi != HalfEdge::NullIndex) 
        continue;
      
      for (size_t iiedge = 0; iiedge < brep.edges.size (); ++iiedge) {
        // if not itself & not paired
        if (iedge == iiedge || brep.edges[iiedge].pairi != HalfEdge::NullIndex) 
          continue;
      
        if (brep.edges[iedge].verti == brep.edges[iiedge].verti) {
          // iedge.pairi = iiedge.nexti.nexti;
          brep.edges[iedge].pairi = brep.edges[ brep.edges[iiedge].nexti ].nexti;
          
          // iiedge.nexti.nexti.pairi = iedge; 
          brep.edges[ brep.edges[ brep.edges[iiedge].nexti ].nexti ].pairi = iedge; 
        }
      }
    
    }
  
    return brep;
  }

  
}





// ---------------------------------------------------------------------------
// handle most of the common ones
// ---------------------------------------------------------------------------
uint32_t darkroot::NumberOfComponents (VkFormat format) {

  switch (format) { 
    // 1 comp
  case VK_FORMAT_R8_UNORM:
  case VK_FORMAT_R8_SNORM :
  case VK_FORMAT_R8_USCALED:
  case VK_FORMAT_R8_SSCALED:
  case VK_FORMAT_R8_UINT:    
  case VK_FORMAT_R8_SINT:    
  case VK_FORMAT_R8_SRGB:    
  case VK_FORMAT_R16_UNORM:
  case VK_FORMAT_R16_SNORM:
  case VK_FORMAT_R16_USCALED:
  case VK_FORMAT_R16_SSCALED:
  case VK_FORMAT_R16_UINT:
  case VK_FORMAT_R16_SINT: 
  case VK_FORMAT_R16_SFLOAT:
  case VK_FORMAT_R32_UINT  :
  case VK_FORMAT_R32_SINT  :
  case VK_FORMAT_R32_SFLOAT:
  case VK_FORMAT_R64_UINT  :    
  case VK_FORMAT_R64_SINT  :    
  case VK_FORMAT_R64_SFLOAT:    
    // sorta
  case VK_FORMAT_D16_UNORM: 
  case VK_FORMAT_D32_SFLOAT: 
  case VK_FORMAT_S8_UINT: 

    return 1;
    break;

    // + 2 components +
  case VK_FORMAT_R4G4_UNORM_PACK8 : // <-- hmmm
  case VK_FORMAT_R8G8_UNORM:
  case VK_FORMAT_R8G8_SNORM:     
  case VK_FORMAT_R8G8_USCALED:
  case VK_FORMAT_R8G8_SSCALED: 
  case VK_FORMAT_R8G8_UINT:      
  case VK_FORMAT_R8G8_SINT:      
  case VK_FORMAT_R8G8_SRGB:      
  case VK_FORMAT_R16G16_UNORM  : 
  case VK_FORMAT_R16G16_SNORM  : 
  case VK_FORMAT_R16G16_USCALED: 
  case VK_FORMAT_R16G16_SSCALED: 
  case VK_FORMAT_R16G16_UINT   :    
  case VK_FORMAT_R16G16_SINT   :    
  case VK_FORMAT_R16G16_SFLOAT :    
  case VK_FORMAT_R32G32_UINT  :
  case VK_FORMAT_R32G32_SINT  :
  case VK_FORMAT_R32G32_SFLOAT:
  case VK_FORMAT_R64G64_UINT  :    
  case VK_FORMAT_R64G64_SINT  :    
  case VK_FORMAT_R64G64_SFLOAT:    
    // sorta
  case VK_FORMAT_D16_UNORM_S8_UINT: 
  case VK_FORMAT_D24_UNORM_S8_UINT:
  case VK_FORMAT_D32_SFLOAT_S8_UINT:  
    // ??? VK_FORMAT_X8_D24_UNORM_PACK32: ???
    return 2;
    break;

   // + 3 components +
  case VK_FORMAT_R5G6B5_UNORM_PACK16:
  case VK_FORMAT_B5G6R5_UNORM_PACK16:   
  case VK_FORMAT_R8G8B8_UNORM  : 
  case VK_FORMAT_R8G8B8_SNORM  : 
  case VK_FORMAT_R8G8B8_USCALED: 
  case VK_FORMAT_R8G8B8_SSCALED:
  case VK_FORMAT_R8G8B8_UINT   :
  case VK_FORMAT_R8G8B8_SINT   :
  case VK_FORMAT_R8G8B8_SRGB   :
  case VK_FORMAT_B8G8R8_UNORM  :
  case VK_FORMAT_B8G8R8_SNORM  :
  case VK_FORMAT_B8G8R8_USCALED: 
  case VK_FORMAT_B8G8R8_SSCALED: 
  case VK_FORMAT_B8G8R8_UINT   :   
  case VK_FORMAT_B8G8R8_SINT   :   
  case VK_FORMAT_B8G8R8_SRGB   :
  case VK_FORMAT_R16G16B16_UNORM  : 
  case VK_FORMAT_R16G16B16_SNORM  :
  case VK_FORMAT_R16G16B16_USCALED:
  case VK_FORMAT_R16G16B16_SSCALED:
  case VK_FORMAT_R16G16B16_UINT   :
  case VK_FORMAT_R16G16B16_SINT   :
  case VK_FORMAT_R16G16B16_SFLOAT :
  case VK_FORMAT_R32G32B32_UINT  :
  case VK_FORMAT_R32G32B32_SINT  :
  case VK_FORMAT_R32G32B32_SFLOAT:
  case VK_FORMAT_R64G64B64_UINT  :    
  case VK_FORMAT_R64G64B64_SINT  :    
  case VK_FORMAT_R64G64B64_SFLOAT:    
  case VK_FORMAT_B10G11R11_UFLOAT_PACK32: 
    return 3;
    break;

   // + 4 components +
  case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
  case VK_FORMAT_B4G4R4A4_UNORM_PACK16: 
  case VK_FORMAT_R5G5B5A1_UNORM_PACK16: 
  case VK_FORMAT_B5G5R5A1_UNORM_PACK16: 
  case VK_FORMAT_A1R5G5B5_UNORM_PACK16: 
  case VK_FORMAT_R8G8B8A8_UNORM:   
  case VK_FORMAT_R8G8B8A8_SNORM:   
  case VK_FORMAT_R8G8B8A8_USCALED: 
  case VK_FORMAT_R8G8B8A8_SSCALED: 
  case VK_FORMAT_R8G8B8A8_UINT   : 
  case VK_FORMAT_R8G8B8A8_SINT   : 
  case VK_FORMAT_R8G8B8A8_SRGB   : 
  case VK_FORMAT_B8G8R8A8_UNORM  : 
  case VK_FORMAT_B8G8R8A8_SNORM  : 
  case VK_FORMAT_B8G8R8A8_USCALED: 
  case VK_FORMAT_B8G8R8A8_SSCALED: 
  case VK_FORMAT_B8G8R8A8_UINT:
  case VK_FORMAT_B8G8R8A8_SINT:
  case VK_FORMAT_B8G8R8A8_SRGB:
  case VK_FORMAT_A8B8G8R8_UNORM_PACK32:  
  case VK_FORMAT_A8B8G8R8_SNORM_PACK32:  
  case VK_FORMAT_A8B8G8R8_USCALED_PACK32:  
  case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:  
  case VK_FORMAT_A8B8G8R8_UINT_PACK32   :  
  case VK_FORMAT_A8B8G8R8_SINT_PACK32   :  
  case VK_FORMAT_A8B8G8R8_SRGB_PACK32: 
  case VK_FORMAT_A2R10G10B10_UNORM_PACK32  :
  case VK_FORMAT_A2R10G10B10_SNORM_PACK32  :
  case VK_FORMAT_A2R10G10B10_USCALED_PACK32: 
  case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: 
  case VK_FORMAT_A2R10G10B10_UINT_PACK32   : 
  case VK_FORMAT_A2R10G10B10_SINT_PACK32   : 
  case VK_FORMAT_A2B10G10R10_UNORM_PACK32  : 
  case VK_FORMAT_A2B10G10R10_SNORM_PACK32  : 
  case VK_FORMAT_A2B10G10R10_USCALED_PACK32: 
  case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: 
  case VK_FORMAT_A2B10G10R10_UINT_PACK32   : 
  case VK_FORMAT_A2B10G10R10_SINT_PACK32   : 
  case VK_FORMAT_R16G16B16A16_UNORM :  
  case VK_FORMAT_R16G16B16A16_SNORM :  
  case VK_FORMAT_R16G16B16A16_USCALED: 
  case VK_FORMAT_R16G16B16A16_SSCALED: 
  case VK_FORMAT_R16G16B16A16_UINT   : 
  case VK_FORMAT_R16G16B16A16_SINT   : 
  case VK_FORMAT_R16G16B16A16_SFLOAT : 
  case VK_FORMAT_R32G32B32A32_UINT   :
  case VK_FORMAT_R32G32B32A32_SINT   :
  case VK_FORMAT_R32G32B32A32_SFLOAT :
  case VK_FORMAT_R64G64B64A64_UINT   :    
  case VK_FORMAT_R64G64B64A64_SINT  :  
  case VK_FORMAT_R64G64B64A64_SFLOAT: 
    return 4;
    break;

  default:
    // ?? case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32 : 
    printf ( "%s [WARNING] --> unhandled format %u\n", __FUNCTION__, format);
    return 0;
    break;
  }

  return 0;
}


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
uint32_t darkroot::SizeOfComponents (VkFormat format) {
  
  switch (format) { 
    // 1 byte component
  case VK_FORMAT_R8_UNORM:
  case VK_FORMAT_R8_SNORM :
  case VK_FORMAT_R8_USCALED:
  case VK_FORMAT_R8_SSCALED:
  case VK_FORMAT_R8_UINT:    
  case VK_FORMAT_R8_SINT:    
  case VK_FORMAT_R8_SRGB:    
  case VK_FORMAT_S8_UINT: 

  case VK_FORMAT_R8G8_UNORM:
  case VK_FORMAT_R8G8_SNORM:     
  case VK_FORMAT_R8G8_USCALED:
  case VK_FORMAT_R8G8_SSCALED: 
  case VK_FORMAT_R8G8_UINT:      
  case VK_FORMAT_R8G8_SINT:      
  case VK_FORMAT_R8G8_SRGB:      
    
  case VK_FORMAT_R8G8B8_UNORM  : 
  case VK_FORMAT_R8G8B8_SNORM  : 
  case VK_FORMAT_R8G8B8_USCALED: 
  case VK_FORMAT_R8G8B8_SSCALED:
  case VK_FORMAT_R8G8B8_UINT   :
  case VK_FORMAT_R8G8B8_SINT   :
  case VK_FORMAT_R8G8B8_SRGB   :
  case VK_FORMAT_B8G8R8_UNORM  :
  case VK_FORMAT_B8G8R8_SNORM  :
  case VK_FORMAT_B8G8R8_USCALED: 
  case VK_FORMAT_B8G8R8_SSCALED: 
  case VK_FORMAT_B8G8R8_UINT   :   
  case VK_FORMAT_B8G8R8_SINT   :   
  case VK_FORMAT_B8G8R8_SRGB   :

  case VK_FORMAT_R8G8B8A8_UNORM:   
  case VK_FORMAT_R8G8B8A8_SNORM:   
  case VK_FORMAT_R8G8B8A8_USCALED: 
  case VK_FORMAT_R8G8B8A8_SSCALED: 
  case VK_FORMAT_R8G8B8A8_UINT   : 
  case VK_FORMAT_R8G8B8A8_SINT   : 
  case VK_FORMAT_R8G8B8A8_SRGB   : 
  case VK_FORMAT_B8G8R8A8_UNORM  : 
  case VK_FORMAT_B8G8R8A8_SNORM  : 
  case VK_FORMAT_B8G8R8A8_USCALED: 
  case VK_FORMAT_B8G8R8A8_SSCALED: 
  case VK_FORMAT_B8G8R8A8_UINT:
  case VK_FORMAT_B8G8R8A8_SINT:
  case VK_FORMAT_B8G8R8A8_SRGB:
  case VK_FORMAT_A8B8G8R8_UNORM_PACK32:  
  case VK_FORMAT_A8B8G8R8_SNORM_PACK32:  
  case VK_FORMAT_A8B8G8R8_USCALED_PACK32:  
  case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:  
  case VK_FORMAT_A8B8G8R8_UINT_PACK32   :  
  case VK_FORMAT_A8B8G8R8_SINT_PACK32   :  
  case VK_FORMAT_A8B8G8R8_SRGB_PACK32: 

    return 1;
    break;

    // + 2 bytes +
  case VK_FORMAT_R16_UNORM:
  case VK_FORMAT_R16_SNORM:
  case VK_FORMAT_R16_USCALED:
  case VK_FORMAT_R16_SSCALED:
  case VK_FORMAT_R16_UINT:
  case VK_FORMAT_R16_SINT: 
  case VK_FORMAT_R16_SFLOAT:
  case VK_FORMAT_D16_UNORM: 
  case VK_FORMAT_R16G16_UNORM  : 
  case VK_FORMAT_R16G16_SNORM  : 
  case VK_FORMAT_R16G16_USCALED: 
  case VK_FORMAT_R16G16_SSCALED: 
  case VK_FORMAT_R16G16_UINT   :    
  case VK_FORMAT_R16G16_SINT   :    
  case VK_FORMAT_R16G16_SFLOAT :    

  case VK_FORMAT_R16G16B16_UNORM  : 
  case VK_FORMAT_R16G16B16_SNORM  :
  case VK_FORMAT_R16G16B16_USCALED:
  case VK_FORMAT_R16G16B16_SSCALED:
  case VK_FORMAT_R16G16B16_UINT   :
  case VK_FORMAT_R16G16B16_SINT   :
  case VK_FORMAT_R16G16B16_SFLOAT :

  case VK_FORMAT_R16G16B16A16_UNORM :  
  case VK_FORMAT_R16G16B16A16_SNORM :  
  case VK_FORMAT_R16G16B16A16_USCALED: 
  case VK_FORMAT_R16G16B16A16_SSCALED: 
  case VK_FORMAT_R16G16B16A16_UINT   : 
  case VK_FORMAT_R16G16B16A16_SINT   : 
  case VK_FORMAT_R16G16B16A16_SFLOAT : 
      
    return 2;
    break;


    //  4 bytes
  case VK_FORMAT_R32_UINT  :
  case VK_FORMAT_R32_SINT  :
  case VK_FORMAT_R32_SFLOAT:
  case VK_FORMAT_D32_SFLOAT: 

  case VK_FORMAT_R32G32_UINT  :
  case VK_FORMAT_R32G32_SINT  :
  case VK_FORMAT_R32G32_SFLOAT:

  case VK_FORMAT_R32G32B32_UINT  :
  case VK_FORMAT_R32G32B32_SINT  :
  case VK_FORMAT_R32G32B32_SFLOAT:

  case VK_FORMAT_R32G32B32A32_UINT   :
  case VK_FORMAT_R32G32B32A32_SINT   :
  case VK_FORMAT_R32G32B32A32_SFLOAT :

    return 4;
    break;


    //case VK_FORMAT_R4G4_UNORM_PACK8 : // <-- hmmm
  case VK_FORMAT_R64_UINT  :    
  case VK_FORMAT_R64_SINT  :    
  case VK_FORMAT_R64_SFLOAT:    
  case VK_FORMAT_R64G64_UINT  :    
  case VK_FORMAT_R64G64_SINT  :    
  case VK_FORMAT_R64G64_SFLOAT:
  case VK_FORMAT_R64G64B64_UINT  :    
  case VK_FORMAT_R64G64B64_SINT  :    
  case VK_FORMAT_R64G64B64_SFLOAT:    
  case VK_FORMAT_R64G64B64A64_UINT:    
  case VK_FORMAT_R64G64B64A64_SINT:  
  case VK_FORMAT_R64G64B64A64_SFLOAT: 

    return 8;
    break;

    // + WAT TODO WITH THESE +
    
  // case VK_FORMAT_D16_UNORM_S8_UINT: 
  // case VK_FORMAT_D24_UNORM_S8_UINT:
  // case VK_FORMAT_D32_SFLOAT_S8_UINT:  
  // ??? VK_FORMAT_X8_D24_UNORM_PACK32: ???
  // case VK_FORMAT_R5G6B5_UNORM_PACK16:
  // case VK_FORMAT_B5G6R5_UNORM_PACK16:   
  // case VK_FORMAT_B10G11R11_UFLOAT_PACK32: 
  // case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
  // case VK_FORMAT_B4G4R4A4_UNORM_PACK16: 
  // case VK_FORMAT_R5G5B5A1_UNORM_PACK16: 
  // case VK_FORMAT_B5G5R5A1_UNORM_PACK16: 
  // case VK_FORMAT_A1R5G5B5_UNORM_PACK16: 
  // case VK_FORMAT_A2R10G10B10_UNORM_PACK32  :
  // case VK_FORMAT_A2R10G10B10_SNORM_PACK32  :
  // case VK_FORMAT_A2R10G10B10_USCALED_PACK32: 
  // case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: 
  // case VK_FORMAT_A2R10G10B10_UINT_PACK32   : 
  // case VK_FORMAT_A2R10G10B10_SINT_PACK32   : 
  // case VK_FORMAT_A2B10G10R10_UNORM_PACK32  : 
  // case VK_FORMAT_A2B10G10R10_SNORM_PACK32  : 
  // case VK_FORMAT_A2B10G10R10_USCALED_PACK32: 
  // case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: 
  // case VK_FORMAT_A2B10G10R10_UINT_PACK32   : 
  // case VK_FORMAT_A2B10G10R10_SINT_PACK32   : 
  // case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32 : 


  default:
    printf ( "%s [WARNING] --> unhandled format %u\n", __FUNCTION__, format);
    return 0;
    break;
  }

  return 0;

}


// ---------------------------------------------------------------------
// DESTROY ALL THE THINGS
// ---------------------------------------------------------------------
void darkroot::CleanupSwapchain (std::vector<rokz::ImageView>& sc_image_views,
                                 rokz::Image&                  msaa_color_image,
                                 rokz::ImageView&              msaa_color_imageview,

                                 rokz::Image&                  depth_image,
                                 rokz::ImageView&              depth_imageview,

                                 rokz::Swapchain&              swapchain,
                                 const rokz::Device&           device,
                                 const VmaAllocator&           allocator) {

  // for (auto fb : framebuffers) {
  //   vkDestroyFramebuffer (device.handle, fb.handle, nullptr); 
  // }

  for (auto sc_imageview : sc_image_views) {
    vkDestroyImageView(device.handle, sc_imageview.handle, nullptr);
  }

  rokz::cx::Destroy (msaa_color_image, allocator);
  rokz::cx::Destroy (msaa_color_imageview, device.handle);

  rokz::cx::Destroy (depth_image, allocator);
  rokz::cx::Destroy (depth_imageview, device.handle);

  vkDestroySwapchainKHR(device.handle, swapchain.handle, nullptr);
}

void darkroot::Cleanup (VkPipeline&                 pipeline,
                        //std::vector<Framebuffer>&         framebuffers,
                        std::vector<rokz::ImageView>&           imageviews,

                        rokz::Swapchain&                  swapchain,
                        VkSurfaceKHR&                     surf,
                        VkCommandPool&                    command_pool,
                        std::vector<rokz::RenderSync>&     syncs, 
                        std::vector<rokz::ShaderModule>&  shader_modules,
                        VkPipelineLayout&                 pipeline_layout,
                        //rokz::RenderPass&                 render_pass,
                        rokz::Image&                      msaa_color_image,
                        rokz::ImageView&                  msaa_color_imageview,

                        rokz::Image&                      depth_image,
                        rokz::ImageView&                  depth_imageview,

                        GLFWwindow*                       w,
                        rokz::Device&                     device,
                        VmaAllocator&                     allocator, 
                        VkInstance&                       inst) {

  //    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
  CleanupSwapchain (imageviews,
                    msaa_color_image, msaa_color_imageview,
                    depth_image, depth_imageview,
                    swapchain, device, allocator);
   
  vkDestroyPipeline (device.handle, pipeline, nullptr);
  vkDestroySurfaceKHR (inst, surf, nullptr);
  vkDestroyCommandPool (device.handle, command_pool, nullptr);

  for (size_t i = 0; i < syncs.size (); ++i) {
    vkDestroySemaphore(device.handle, syncs[i].image_available_sem, nullptr);
    vkDestroySemaphore(device.handle, syncs[i].render_finished_sem, nullptr);
    vkDestroyFence (device.handle, syncs[i].in_flight_fen, nullptr);
  }

  for (auto shmod : shader_modules) {
    vkDestroyShaderModule (device.handle, shmod.handle, nullptr); 
  }
  vkDestroyPipelineLayout (device.handle, pipeline_layout, nullptr);
  //vkDestroyRenderPass (device.handle, render_pass.handle, nullptr); 

  vmaDestroyAllocator(allocator);
  vkDestroyDevice    (device.handle, nullptr); 
  vkDestroyInstance  (inst, nullptr);
  glfwDestroyWindow(w);
}



bool darkroot::RecreateSwapchain(rokz::Swapchain&  swapchain,
                                 std::vector<rokz::Image>& swapchain_images, std::vector<rokz::ImageView>& imageviews,
                                 rokz::Image& depth_image, rokz::ImageView& depth_imageview,
                                 rokz::Image& multisamp_color_image, rokz::ImageView& multisamp_color_imageview,
                                 const VmaAllocator& allocator, GLFWwindow* glfwin, const rokz::Device& device) {

  printf ("%s\n", __FUNCTION__);

  int width = 0, height = 0;
  glfwGetFramebufferSize(glfwin, &width, &height);

  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(glfwin, &width, &height);
    glfwWaitEvents();
  }
  
  vkDeviceWaitIdle (device.handle);

  CleanupSwapchain (imageviews,
                    depth_image, depth_imageview,
                    multisamp_color_image,
                    multisamp_color_imageview,
                    swapchain, device, allocator);

  //CreateInfo_default (swapchain.ci, surf, extent, swapchain_support_info, 
  bool swapchain_res    = rokz::cx::CreateSwapchain (swapchain, device);
  bool imageviews_res   = rokz::cx::CreateImageViews (imageviews, swapchain_images, device);

  return (swapchain_res && imageviews_res); 
}





// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
auto darkroot::DarkRenderable::SetupRS (VkCommandBuffer commandbuffer) -> int {
  return 0; 
}


// --------------------------------------------------------------------
auto darkroot::DarkRenderable::Draw (VkCommandBuffer commandbuffer) -> void  {
  // rokz::Pipeline pipeline;
  // rokz::DescriptorGroup desc_group;
  VkBuffer vertex_buffers[] = { vb_dev.handle};
  VkDeviceSize offsets[] = {0};

  vkCmdBindVertexBuffers(commandbuffer, 0, 1, vertex_buffers, offsets);

  vkCmdBindIndexBuffer(commandbuffer, ib_dev.handle, 0, VK_INDEX_TYPE_UINT16);

  vkCmdDrawIndexed (commandbuffer, num_inds, 1, 0, 0, 0);
  //return 0; 
}


// --------------------------------------------------------------------
auto darkroot:: DarkRenderable::AllocRes (VmaAllocator& allocator) -> int {
    
  rokz::cx::CreateInfo_VB_device (vb_dev.ci, DarkrootMesh::VertexSize,  num_verts);
  rokz::cx::AllocCreateInfo_device (vb_dev.alloc_ci); 
  rokz::cx::CreateBuffer (vb_dev, allocator); 
    
  rokz::cx::CreateInfo_IB_16_device (ib_dev.ci, num_inds); 
  rokz::cx::AllocCreateInfo_device (ib_dev.alloc_ci);
  rokz::cx::CreateBuffer (ib_dev, allocator);

  return 0; 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
auto darkroot:: DarkRenderable::FreeRes (VmaAllocator& alloc) -> int {

  rokz::cx::Destroy (vb_dev, alloc);
  rokz::cx::Destroy (ib_dev, alloc);

  return 0; 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
constexpr size_t SizeOfDarkRenderable () {
  return sizeof ( darkroot::DarkRenderable); 
}

// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
 darkroot::DarkRenderable& Initialize ( darkroot::DarkRenderable& dr, uint32_t nv, uint32_t ni) {

  new (&dr) darkroot:: DarkRenderable (nv, ni);
  return dr;
}

 darkroot::DarkRenderable& Friendly (  darkroot::DarkRenderable& dr) {

  Initialize (dr, 0, 0);
  // dr.num_verts = 0;
  // dr.num_inds = 0;
  // dr.vb_dev;
  // dr.ib_dev;
  return dr;
}




// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
#ifdef DARKROOT_ENABLE_RENDERABLE_TEST
void SetupDarkGeometry (Glob& glob) {

  printf ("%s\n", __FUNCTION__); 

  const DarkrootMesh& darkmesh = darkroot::DarkOctohedron (); 

  HalfEdge::BRep darkboundary;
  BuildBoundaryRep (darkboundary, glob.darkmesh);

  //DarkrootMesh& darkmesh = glob.darkmesh; 

  // create the renderable
  Initialize (glob.darkobj, darkmesh.verts.size(), darkmesh.indices.size()); 
  glob.renderables.push_back (&glob.darkobj);

  void* pmapped  = nullptr;
  // VERTEX BUFFER allocat and fill transfer buffers
  rokz::Buffer vb_x;
  rokz::CreateInfo_VB_stage (vb_x.ci, DarkrootMesh::VertexSize, darkmesh.verts.size());
  rokz::AllocCreateInfo_stage (vb_x.alloc_ci);
  rokz::CreateBuffer (vb_x, glob.allocator);
  if (rokz::MapMemory (&pmapped, vb_x.allocation, glob.allocator)) {
    memcpy (pmapped,  &darkmesh.verts[0] , DarkrootMesh::VertexSize * darkmesh.verts.size()); 
    rokz::UnmapMemory (vb_x.allocation, glob.allocator); 
  }
  // INDEX BUFFER
  rokz::Buffer ib_x;
  rokz::CreateInfo_IB_16_stage (ib_x.ci, darkmesh.indices.size ()); 
  rokz::AllocCreateInfo_stage (ib_x.alloc_ci);
  rokz::CreateBuffer (ib_x, glob.allocator);

  if (rokz::MapMemory (&pmapped, ib_x.allocation, glob.allocator)) {
    memcpy (pmapped, &darkmesh.indices[0], DarkrootMesh::IndexSize * darkmesh.indices.size ()); 
    rokz::UnmapMemory (ib_x.allocation, glob.allocator); 
  }

  // 
  for (auto r : glob.renderables) { 
    r->AllocRes (glob.allocator); 
  }
  
  //rokz::Transfer_2_Device;
  rokz::MoveToBuffer_XB2DB (glob.darkobj.vb_dev, vb_x, DarkrootMesh::VertexSize * glob.darkmesh.verts.size(), 
                            glob.command_pool.handle, glob.queues.graphics, glob.device.handle); 

  rokz::MoveToBuffer_XB2DB  (glob.darkobj.ib_dev, ib_x, DarkrootMesh::IndexSize * glob.darkmesh.indices.size (),
                             glob.command_pool.handle, glob.queues.graphics, glob.device.handle); 

  rokz::Destroy (vb_x, glob.allocator); 
  rokz::Destroy (ib_x, glob.allocator); 

}

#endif // DARKROOT_ENABLE_RENDERABLE_TEST



// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
int darkroot::OpenImageFile (const std::string& fqname, DevILOpenFileCB cb, void* up) {

  DevILImageProps props; 

  ilInit ();
  ilBindImage (ilGenImage ());

  int res = 0;
  if (ilLoadImage(fqname.c_str())) {
    
    printf ("Opened [%s]\n", fqname.c_str() ); 
    props.width    = ilGetInteger (IL_IMAGE_WIDTH); 
    props.height   = ilGetInteger (IL_IMAGE_HEIGHT);
    props.depth    = ilGetInteger (IL_IMAGE_DEPTH);
    props.bytes_per_pixel= ilGetInteger (IL_IMAGE_BYTES_PER_PIXEL); 
    props.bpp      = ilGetInteger (IL_IMAGE_BPP);
    props.type     = ilGetInteger (IL_IMAGE_TYPE);
    props.format   = ilGetInteger (IL_IMAGE_FORMAT);


    res = cb (ilGetData (), props, up); 

    ilDeleteImage (ilGetInteger (IL_ACTIVE_IMAGE)); 

  }

  ilShutDown ();
  return res; 
}


// ---------------------------------------------------------------------
// load texture to device memory
// ---------------------------------------------------------------------
bool darkroot::LoadTexture_color_sampling (rokz::Image&             image,
                                           VkFormat                 format,
                                           const VkExtent2D&        ext2d,
                                           const void*              srcimage,
                                           const VmaAllocator&      allocator, 
                                           const VkQueue&           queue, 
                                           const rokz::CommandPool& commandpool, 
                                           const rokz::Device&      device) {

  //size_t image_size = image_width * image_height *  bytes_per_pixel; 
  auto image_size = SizeOfComponents (format)
                  * NumberOfComponents (format)
                  * ext2d.width * ext2d.height;
  assert (image_size); 

  rokz::Buffer stage_buff; 
  
  rokz::cx::CreateInfo_buffer_stage (stage_buff.ci, image_size);
  rokz::cx::AllocCreateInfo_stage (stage_buff.alloc_ci);
  rokz::cx::CreateBuffer (stage_buff, allocator); 

  void* mapped = nullptr; 
  if (rokz::cx::MapMemory (&mapped, stage_buff.allocation, allocator)) { 
  
    const uint8_t* image_data = reinterpret_cast<const unsigned char*> (srcimage); 
    std::copy (image_data, image_data + image_size, reinterpret_cast<uint8_t*> (mapped));
  }
  rokz::cx::UnmapMemory (stage_buff.allocation, allocator);

  rokz::cx::CreateInfo_2D_color_sampling  (image.ci, VK_SAMPLE_COUNT_1_BIT, ext2d.width, ext2d.height);
  rokz::cx::AllocCreateInfo_device (image.alloc_ci);
  if (!rokz::cx::CreateImage (image, allocator)) {
    printf ("[FAILED] %s setup test texture", __FUNCTION__);
    return false;
  }

  //VK_FORMAT_R8G8B8A8_SRGB
  rokz::cx::TransitionImageLayout (image.handle, format, VK_IMAGE_LAYOUT_UNDEFINED,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               queue, commandpool.handle, device.handle);

  rokz::cx::CopyBufferToImage (image.handle, stage_buff.handle, ext2d.width, ext2d.height,
                           queue, commandpool.handle, device.handle);

  rokz::cx::TransitionImageLayout (image.handle, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                               queue, commandpool.handle, device.handle);

  rokz::cx::Destroy (stage_buff, allocator); 
  return true; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
void darkroot::SetupViewportState (rokz::ViewportState & vps, const VkExtent2D& swapchain_extent) {

  const VkOffset2D offs0 {0, 0};

  vps.viewports.resize (1);
  vps.scissors.resize (1);
  
  vps.scissors[0] = { offs0, swapchain_extent };
  rokz::ViewportState_default (vps, vps.scissors[0], 1.0f); 

}
