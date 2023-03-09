
#include "darkrootgarden.h"
#include "rekz/dark_types.h"

#include "dark_obj_pipeline.h"
#include <vulkan/vulkan_core.h>


using namespace rekz;




//
// ?? a different DrawSequence subtype for each pipeline,
// yes: ex:  textured drawvs wireframe draw but can share same data  
struct DrawWireframe : public rokz::DrawSequence
{

public:
  
  DrawWireframe  (const rekz::PolygonData& d) : polyd (d) {
  }

  virtual ~DrawWireframe () {
  }

  virtual int Prep (uint32_t current_frame, const RenderEnv& env, const rokz::Device& device) { 
    HERE ("wireframe does nothing");
    return 0;
  }
  virtual int Exec (VkCommandBuffer comb, uint32_t current_frame, const RenderEnv& env) { 
    HERE ("wireframe does nothing");
    
    return 0;
  }

protected:

  const rekz::PolygonData& polyd;
  
}; 

// -------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------
std::shared_ptr<rokz::DrawSequence> rekz::CreateDrawWireframe (const rekz::PolygonData& d) {

  return std::make_shared<DrawWireframe> (d);
  
}

