

#include "onscreen_draw.h"




using namespace rokz;



DrawSequence::Ref rekz::onscreen::CreateDrawText (const onscreen::Data& dat) {


  struct osd_draw_text : public DrawSequence  {
    
    const onscreen::Data& data;

    
    osd_draw_text (const onscreen::Data& dat) :data (dat) {
      }
    
    virtual int Prep (uint32_t current_frame, const RenderEnv& env, const rokz::Device& device) {

      return __LINE__;
    }


    virtual int Exec (VkCommandBuffer comb, uint32_t current_frame, const RenderEnv& env) {

      return __LINE__;
    }

  }; 
  
  return  std::make_shared<osd_draw_text> (dat); 

}
