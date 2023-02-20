
#include "darkrootgarden.h"
#include "rekz/dark_types.h"



using namespace darkroot;



// --------------------------------------------------------------------
//
// --------------------------------------------------------------------
PolygonData& darkroot::Setup (PolygonData& pd) {

    // SetupObjectUniforms ; 
    // SetupObjectTextureAndSampler;
    // SetupObjResources;
    
    return  pd;
  }

PolygonData& darkroot::Cleanup (PolygonData& pd, const VmaAllocator& allocator) {

    // SetupObjectUniforms ; 
    // SetupObjectTextureAndSampler;
    // SetupObjResources;
    for (auto& ub : pd.vma_uniform_buffs) {
      rokz::cx::Destroy (ub, allocator); 
    }

    for (auto buf : pd.vma_objparam_buffs) {  
      rokz::cx::Destroy (buf, allocator);
    }
    
    return  pd;
  }
