
#include "draw_mars.h"


using namespace rokz;
// ------------------------------------------------------------------------------------------
//                        
// ------------------------------------------------------------------------------------------
DrawSequence::Ref rekz::CreateDrawMarsLandscape ()  {


  struct DrawMars : public DrawSequence {

    DrawMars () {}
    virtual ~DrawMars () {}

    virtual int Prep (uint32_t current_frame, const RenderEnv& env, const rokz::Device& device) {

      return __LINE__;
    }

    virtual int Exec (VkCommandBuffer comb, uint32_t current_frame, const RenderEnv& env) {



      return __LINE__;
    }

  }; 


  return std::make_shared<DrawMars> ();
  
}



