
#include "rekz.h"
#include "rekz/dark_types.h"
#include "rokz/draw_sequence.h"



struct DrawGrid : public rokz::DrawSequence {

  DrawGrid (const rekz::GridData& dat) : data (dat) {
  }

  virtual ~DrawGrid () {
  }
  
    // do crap before recording ("UpdateDescriptors()", etc)
  virtual int Prep (const shared_globals& globals, const pipeline_assembly& pa, const rokz::Device& device);
    // the draw sequence recording, mebe rename to DrawSeq::Rec() 
  virtual int Exec (VkCommandBuffer comb, const pipeline_assembly& pa, const VkDescriptorSet& ds);
  
  const rekz::GridData& data;

};


// 
int DrawGrid::Prep (const shared_globals& globals, const pipeline_assembly& pa, const rokz::Device& device) {

  
  return 0; 
}



// the draw sequence recording, mebe rename to DrawSeq::Rec() 
int DrawGrid::Exec (VkCommandBuffer comb, const pipeline_assembly& pa, const VkDescriptorSet& ds)  {

  return 0; 
}


rokz::DrawSequence::Ref rekz::CreateDrawGrid (const GridData& dat) {

  return std::make_shared<DrawGrid> (dat); 
}


