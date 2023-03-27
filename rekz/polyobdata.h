

#ifndef REKZ_POLYOBDATA_INCLUDE
#define REKZ_POLYOBDATA_INCLUDE

#include "rekz.h"
#include "dark_obj_pipeline.h"

namespace rekz {
  // ---------------------------------------------------------------------------------------
  // what is this
  // ---------------------------------------------------------------------------------------
  struct PolygonData { 
    size_t                    indexoffs;
    size_t                    vertexoffs;
    rc::Buffer::Ref           devicebuffer;
    // image/texture
    Vec<rc::Image::Ref>       textures;   // color texture
    Vec<rc::ImageView::Ref>   imageviews;
    rc::Sampler::Ref          sampler;    // just color samp

    std::array<glm::vec3, obz::kMaxCount> objrot;   // scene objects 
    std::array<glm::vec3, obz::kMaxCount> objpos;
  };

  // ?? a pipeline is tied to a drawlist.. no.
  // ?? a drawlist is tied to data..       mebe
  // ?? a data is tied to a drawlist..     no
  // ?? a drawlist is tied to a pipeline.. no

  DrawSequence::Ref CreatePolygonDraw      (const PolygonData& d, const std::vector<rc::Buffer::Ref>& objres, const rokz::DescriptorGroup& descg); 
  DrawSequence::Ref CreatePolygonWireframe (const PolygonData& d); 
  DrawSequence::Ref CreateDrawWireframe    (const PolygonData& d); 

  rekz::PolygonData& SetupPolygonData   (rekz::PolygonData& pd, uint32_t num_frames, const std::string& data_root, const rokz::Device& device); 
  void               CleanupPolygonData (rekz::PolygonData& pd, const rokz::Device& device);
}

#endif
