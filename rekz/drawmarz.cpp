
#include "drawmarz.h"
#include "marzdata.h"
using namespace rokz;
// ------------------------------------------------------------------------------------------
//                        
// ------------------------------------------------------------------------------------------
DrawSequence::Ref rekz::CreateDrawMarsLandscape (marz::Data& dat)  {

  struct drawmarz : public DrawSequence {

    marz::Data& dat;
    //
    drawmarz (marz::Data& d) : dat (d) {}
    //
    virtual ~drawmarz () { }

    //
    virtual int Prep (uint32_t current_frame, const RenderEnv& env, const rokz::Device& device) {

      
      return __LINE__;
    }

    //
    virtual int Exec (VkCommandBuffer comb, uint32_t current_frame, const RenderEnv& env) {



      return __LINE__;
    }

  }; 


  return std::make_shared<drawmarz> (dat);
}



