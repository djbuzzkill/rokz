
#ifndef ROKZ_DRAW_SEQUENCE
#define ROKZ_DRAW_SEQUENCE

#include "common.hpp"
#include "utility.hpp"
#include "rokz_types.hpp"
#include <vulkan/vulkan_core.h>



namespace rokz {

  // ---------------------------------------------------------------------
  // 
  // ---------------------------------------------------------------------
  enum DescriptorSetTypes {

    DS_GLOBAL,
    DS_OBJECT,
    
  }; 

  // ---------------------------------------------------------------------
  //  encapsulate a list of draw commands
  // DrawList renamed to DrawSequence to avoid confusion
  // ---------------------------------------------------------------------
  struct DrawSequence : public destructor {

    typedef std::shared_ptr<DrawSequence>  Ref;

    //typedef std::map<std::string, VkDescriptorSet>       DescriptorMap;
    typedef std::map<std::string, VkDescriptorSetLayout> DescriptorLayoutMap;
    // --------------------------------------------------------------------
    // part of DrawSequence
    // --------------------------------------------------------------------
    // struct pipeline_assembly  {
    //   rokz::Pipeline&            pipeline;
    //   VkPipelineLayout           plo;
    // }; 
    // typedef pipeline_assembly PipelineAssembly; 
    // --------------------------------------------------------------------
    // part of DrawSequence
    // --------------------------------------------------------------------
    struct shared_globals {

      shared_globals () : viewport_ext (), dt(0.0f), sim_time(0.0f), view_rot(), view_pos(0.0) {
        // pd.view_orie.theta = 0.0f;
        // pd.view_orie.phi   = kPi;
      }
    
      VkExtent2D     viewport_ext;
      float          dt;
      float          sim_time;
      glm::vec3      view_rot; // yaw pitch roll
      glm::vec3      view_pos;

    };

    typedef shared_globals Globals;

    struct render_environment {
      rokz::Pipeline&     pipeline;
      VkPipelineLayout    layout;

      //PipelineAssembly&  pa;
      Globals&           globals;
      //DescriptorMap&     descriptormap;
    }; 
    typedef render_environment RenderEnv; 


    virtual int Prep (uint32_t current_frame, const RenderEnv& env, const rokz::Device& device) = 0; 
    virtual int Exec (VkCommandBuffer comb, uint32_t current_frame, const RenderEnv& env) = 0;


    
    // do crap before recording ("UpdateDescriptors()", etc)
    //virtual int Prep (const shared_globals& globals, const pipeline_assembly& pa, const rokz::Device& device) = 0; 

    // the draw sequence recording, mebe rename to DrawSeq::Rec() 
    //virtual int Exec (VkCommandBuffer comb, const shared_globals& globals, const pipeline_assembly& pa, const DescriptorMap& descrmap) = 0;

  protected:

    DrawSequence () {} // dont allow abstract

  }; 



}

#endif
