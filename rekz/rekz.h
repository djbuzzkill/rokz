
#ifndef ROCKZ_INCLUDE
#define ROCKZ_INCLUDE


//#include "rekz/dark_obj_pipeline.h"
#include "rokz/rokz.hpp"
#include "rokz/vert_defs.hpp"
#include "meshery.h"
#include "shaderc/shaderc.hpp"
#include <vulkan/vulkan.h>


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
    Ty r, g, b;
  };

  typedef RGBx<unsigned char> RGBu8;
  typedef RGBx<float>         RGBf;
  

  // ---------------------------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------------------------
  namespace geomz {

    typedef rokz::PNTx_Vert Vert;

    typedef rekz::geom::TriMesh<Vert> Mesh;
      

    struct PT { enum GEOM_TYPE { 
      CUBE = 0,
      OCTOHEDRON, 
      //
      NUM_GEOM_TYPES} ;
    };

    extern const std::array<geomz::Mesh, PT::NUM_GEOM_TYPES> Geomz;

    inline const Mesh& Octohedron () { return Geomz[PT::OCTOHEDRON]; }
    inline const Mesh& Cube       () { return Geomz[PT::CUBE];  }
    
  }

  // ---------------------------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------------------------
  struct transform {
    glm::vec3 position;
    glm::vec3 rotation_eu;
  }; 
  // ---------------------------------------------------------------------------------------
  //                   
  // ---------------------------------------------------------------------------------------
  rc::Buffer::Ref SetupGridData (std::size_t& vertoffset, size_t& indoffset, 
                                 uint32 xvertcount, uint32 zvertcount,
                                 float xsize, float zsize,
                                 const Device& device); 

  rokz::DrawSequence::Ref CreateDrawGrid (rokz::rc::Buffer::Ref& griddata,
                                          rekz::DescriptorGroup& dg, std::size_t voffs, std::size_t ioffs); 
  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------

  glm::vec3 unit_angle_xz (float theta);
  glm::vec3 unit_angle_yz (float theta);
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


  void CleanupSwapchain (Vec<rc::ImageView::Ref>& sc_image_views,
                         Vec<rc::Image::Ref>&     color_images,
                         Vec<rc::ImageView::Ref>& color_views,
                         rc::Image::Ref&          depth_image,
                         rc::ImageView::Ref&      depth_view,
                         rc::Swapchain::Ref&      swapchain,
                         const Device&            device); 

  
  bool RecreateSwapchain (rc::Swapchain::Ref& swapchain, const rokz::Display& win, 
                          Vec<VkImage>& swapchain_images, Vec<rc::ImageView::Ref>& imageviews,
                          rc::Image::Ref& depth_image, rc::ImageView::Ref& depth_imageview,
                          rc::Image::Ref& msaa_color_image, rc::ImageView::Ref& msaa_color_imageview,
                          const Device& device) ;

  rokz::SwapchainResetter::Ref
  CreateSwapchainResetter (rc::Swapchain::Ref& swapchain, rokz::Display& displ, 
                           Vec<VkImage>& scis, Vec<rc::ImageView::Ref>& scivs,
                           rc::Image::Ref& depthim, rc::ImageView::Ref& depthiv,
                           rc::Image::Ref& msaacolorim, rc::ImageView::Ref& msaacoloriv); 

  // ----------------------------------------------------------------------------------------
  //                     
  // ----------------------------------------------------------------------------------------

} // rekz



#endif
