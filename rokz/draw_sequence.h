#ifndef ROKZ_DRAW_SEQUENCE
#define ROKZ_DRAW_SEQUENCE

#include "common.h"
#include "utility.h"
#include "rokz_types.h"
#include "rokz_types.h"



namespace rokz {

  // ---------------------------------------------------------------------
  // INTENT:  encapsulate a list of draw commands
  // DrawLisst renamed to DrawSequence to avoid confusion
  // ---------------------------------------------------------------------

  struct DrawSequence : public rokz::ut::destructor {

    typedef std::shared_ptr<DrawSequence>  Ref;
    // --------------------------------------------------------------------
    // part of DrawSequence
    // --------------------------------------------------------------------
    struct pipeline_assembly  {
      rokz::Pipeline&       pipeline;
      VkPipelineLayout      plo;
      VkDescriptorSet       descrset;
    }; 
    typedef pipeline_assembly PipelineAssembly; 
    // --------------------------------------------------------------------
    // part of DrawSequence
    // --------------------------------------------------------------------
    struct shared_globals {

      shared_globals () : viewport_ext (), dt(0.0f), sim_time(0.0f), current_frame (0), view_ypr(0.0), view_pos(0.0) {
        // pd.view_orie.theta = 0.0f;
        // pd.view_orie.phi   = kPi;
      }
    
      VkExtent2D     viewport_ext;
      float          dt;
      float          sim_time;
      uint32_t       current_frame;
      glm::vec3      view_ypr; // yaw pitch roll
      glm::vec3      view_pos;

    };

    typedef shared_globals Globals;
    
    // do crap before recording ("UpdateDescriptors()", etc)
    virtual int Prep (const shared_globals& globals, const pipeline_assembly& pa, const rokz::Device& device) = 0; 
    // the draw sequence recording, mebe rename to DrawSeq::Rec() 
    virtual int Exec (VkCommandBuffer comb, const pipeline_assembly& pa, const std::vector<VkDescriptorSet>& descrsets) = 0;

  protected:

    DrawSequence () {} // dont allow abstract

  }; 



}

#endif
