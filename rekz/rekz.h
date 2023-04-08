
#ifndef ROCKZ_INCLUDE
#define ROCKZ_INCLUDE


//#include "rekz/dark_obj_pipeline.h"
#include "rokz/rokz.h"
#include "rokz/vert_defs.h"
#include "meshery.h"
#include "shaderc/shaderc.hpp"
#include <vulkan/vulkan_core.h>
//#include <vulkan/vulkan_core.h>
//#include "rokz/rokz_funcs.h"
namespace rekz {

  using namespace rokz;

  const auto kPi  = rokz::kPi;
  const auto k2Pi = rokz::k2Pi;
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


  namespace fonttool {
    // between 32 - 126 of printable ascii symbols
    std::string font_glyph_filename (uint32 asciicode); 
  
  }
  // ---------------------------------------------------------------------------------------
  //                   
  // ---------------------------------------------------------------------------------------
  rc::Buffer::Ref SetupGridData (size_t& vertoffset, size_t& indoffset, 
                                 uint32 xvertcount, uint32 zvertcount,
                                 float xsize, float zsize,
                                 const Device& device); 

  rokz::DrawSequence::Ref CreateDrawGrid (rokz::rc::Buffer::Ref& griddata,
                                          rekz::DescriptorGroup& dg, size_t voffs, size_t ioffs); 
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
