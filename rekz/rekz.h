
#ifndef ROCKZ_INCLUDE
#define ROCKZ_INCLUDE


#include "rokz/rokz.h"
#include "rokz/vert_defs.h"
#include "meshery.h"
#include "shaderc/shaderc.hpp"
#include <vulkan/vulkan_core.h>

#include <shaderc/shaderc.h>

//#include <vulkan/vulkan_core.h>

//#include "rokz/rokz_funcs.h"

namespace rekz {

  using namespace rokz;

  // ----------------------------------------------------------------------------------------------
  //                           
  // ----------------------------------------------------------------------------------------------
  template<typename Ty> struct RGBx {
    Ty r;
    Ty g;
    Ty b;
  };

  typedef RGBx<unsigned char> RGBu8;
  typedef RGBx<float>         RGBf;
  
  // ---------------------------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------------------------
  namespace platonic {

    typedef rokz::PNCTx_Vert Vert;

    typedef rekz::geom::TriMesh<Vert> Mesh;

    const Mesh& Octohedron ();
  }


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

    std::array<glm::vec3, 32> objrot;   // scene objects 
    std::array<glm::vec3, 32> objpos;
  };

  // ---------------------------------------------------------------------------------------
  // what is this
  // ---------------------------------------------------------------------------------------
  struct Obdat { 

    rc::Buffer::Ref    devicebuffer;
    // image/texture
    Sampler            sampler;   // 
    Vec<rc::Image::Ref>      textures;  // color texture
    Vec<rc::ImageView::Ref>  imageviews;

    std::array<glm::vec4, 32> objrot;     // scene objects 
    std::array<glm::vec4, 32> objpos;
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
  
  // ---------------------------------------------------------------------------------------
  //                   
  // ---------------------------------------------------------------------------------------
  rc::Buffer::Ref SetupGridData (size_t& vertoffset, size_t& indoffset, 
                                 uint32 xvertcount, uint32 zvertcount,
                                 float xsize, float zsize,
                                 const Device& device); 


  rokz::DrawSequence::Ref CreateDrawGrid (rokz::rc::Buffer::Ref griddata, size_t voffs, size_t ioffs); 

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------

  glm::vec3& unit_angle_xz (glm::vec3& v, float theta);
  glm::vec3& unit_angle_yz (glm::vec3& v, float theta);


  // ----------------------------------------------------------------------------------------
  //                     
  // ----------------------------------------------------------------------------------------
  void CleanupSwapchain (Vec<rc::ImageView::Ref>& sc_image_views,
                         rc::Image::Ref&       msaa_color_image,
                         rc::ImageView::Ref&   msaa_color_imageview,
                         rc::Image::Ref&       depth_image,
                         rc::ImageView::Ref&   depth_imageview,
                         rc::Swapchain::Ref&   swapchain,
                         const rokz::Device&   device);

  bool RecreateSwapchain (rc::Swapchain::Ref& swapchain, const rokz::Display& win, 
                          Vec<VkImage>& swapchain_images, Vec<rc::ImageView::Ref>& imageviews,
                          rc::Image::Ref& depth_image, rc::ImageView::Ref& depth_imageview,
                          rc::Image::Ref& msaa_color_image, rc::ImageView::Ref& msaa_color_imageview,
                          const Device& device) ;


  rokz::SwapchainResetter::Ref
  CreateSwapchainResetter (rc::Swapchain::Ref& swapchain, 
                           Vec<VkImage>& scis, Vec<rc::ImageView::Ref>& scivs,
                           rc::Image::Ref& depthim, rc::ImageView::Ref& depthiv,
                           rc::Image::Ref& msaacolorim, rc::ImageView::Ref& msaacoloriv); 

  
  // ----------------------------------------------------------------------------------------
  //                     
  // ----------------------------------------------------------------------------------------
  inline void printmat (glm::mat4& mat) { 
      for (int iy = 0; iy < 4; ++iy) {
        for (int ix = 0; ix < 4; ++ix) {

          if  (ix == 0) 
            printf ( "[ %f", mat[iy][ix]);
          else
            printf ( " %f", mat[iy][ix]);
        }
        printf ( " ]\n");
      }
  }



} // rekz



#endif
