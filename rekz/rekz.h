
#ifndef ROCKZ_INCLUDE
#define ROCKZ_INCLUDE


#include "rokz/rokz.h"

//#include <vulkan/vulkan_core.h>

//#include "rokz/rokz_funcs.h"

namespace rekz {

  using namespace rokz;
  // ----------------------------------------------------------------------------------------------
  //                                    
  // ----------------------------------------------------------------------------------------------
  const size_t kMaxObjectCount = 128;

  extern const Vec<VkDescriptorSetLayoutBinding>      kObjDescriptorBindings;
  // extern const VkVertexInputBindingDescription        kVertexInputBindingDesc;
  // extern const Vec<VkVertexInputAttributeDescription> kVertexInputBindingAttributeDesc;
  //extern const Vec<VkDescriptorSetLayoutBinding> kGlobalDescriptorBindings;
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
  
  // // ----------------------------------------------------------------------------------------------
  //                           
  // ----------------------------------------------------------------------------------------------
  template<typename VTy, typename IndTy>
  struct GeometryData {

    typedef  VTy         VertexType; 
    typedef  IndTy       IndexType; 

    enum { VertexSize = sizeof(VTy) }; 
    enum { IndexSize  = sizeof(IndTy) }; 
  
    Vec<VTy>   verts;
    Vec<IndTy> indices;
  }; 

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  template<typename VTy> 
  using TriMesh = rekz::GeometryData<VTy, uint16_t>; 

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  struct PushConstants {

    uint32_t resourceID;
    uint32_t _unused_01;
    uint32_t _unused_02;
    uint32_t _unused_03;

  }; 

  
  // ---------------------------------------------------------------------------------------
  //
  // ---------------------------------------------------------------------------------------
  typedef rokz::Vertex_pos_nrm_col_txc PolyVert;
  typedef rekz::TriMesh<PolyVert>      PolyMesh;

  // ---------------------------------------------------------------------------------------
  namespace platonic {
    const PolyMesh& Octohedron ();
  }



  // ---------------------------------------------------------------------------------------
  // what is this
  // ---------------------------------------------------------------------------------------
  struct PolygonData { 

    rokz::Buffer          vb_device;
    rokz::Buffer          ib_device;
    // rokz::DescriptorGroup descrgroup;
    // image/texture
    rokz::Image           texture;   // color texture
    rokz::ImageView       imageview; // 
    rokz::Sampler         sampler;   // 

    float                 obj_theta[2];     // scene objects 
    //rekz::Sphericalf      objatt;
    std::array<glm::vec3, 32> objrot;     // scene objects 
    std::array<glm::vec3, 32> objpos;
  } ;

  // ?? a pipeline is tied to a drawlist.. no.
  // ?? a drawlist is tied to data..       ??
  // ?? a data is tied to a drawlist..     no
  // ?? a drawlist is tied to a pipeline.. no

  rokz::DrawSequence::Ref CreatePolygonDraw      (const PolygonData& d, const std::vector<rokz::Buffer>& objres, const rokz::DescriptorGroup& descg); 
  rokz::DrawSequence::Ref CreatePolygonWireframe (const PolygonData& d); 
  rokz::DrawSequence::Ref CreateDrawWireframe    (const PolygonData& d); 

  rekz::PolygonData& SetupPolygonData   (rekz::PolygonData& pd, uint32_t num_frames, const std::string& data_root, const rokz::Device& device); 
  void               CleanupPolygonData (rekz::PolygonData& pd, const rokz::Device& device);
  
  // ---------------------------------------------------------------------------------------
  //                   
  // ---------------------------------------------------------------------------------------
  struct GridData {
    // some shit like this
    rokz::Buffer          vb_device;
    rokz::Buffer          ib_device;
    //rokz::DescriptorGroup descrgroup;
  };
  
  bool SetupGridData   (GridData& gd, const rokz::Device& device);
  void CleanupGridData (GridData& gd, const rokz::Device& device);
  
  rokz::DrawSequence::Ref CreateDrawGrid (const GridData& dat);

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------

  glm::vec3& unit_angle_xz (glm::vec3& v, float theta);
  glm::vec3& unit_angle_yz (glm::vec3& v, float theta);

  // ----------------------------------------------------------------------------------------
  //                     
  // ----------------------------------------------------------------------------------------
  bool InitializeSwapchain (rokz::SwapchainGroup& scg,
                            const rokz::SwapchainSupportInfo& swapchain_support_info,
                            const VkSurfaceKHR& surface,
                            const VkExtent2D&   extent, 
                            const rokz::PhysicalDevice& physdev,
                            const rokz::Device& device) ; 
  //
  // actually Initize,Instance,SurfaceAndDevice ()
  // bool InitializeInstance (rokz::Instance& instance); 
  // bool InitializeDevice (rokz::Device& device, const rokz::PhysicalDevice& physical_device, const rokz::Instance& instance); 
  // bool InitializeDevice (rokz::Instance& instance, rokz::Device& device, rokz::Window& window,  VkSurfaceKHR& surface, rokz::PhysicalDevice& physical_device); 

  // bool SetupGlobalUniforms (Vec<rokz::Buffer>& uniform_buffs, uint32_t num_sets, const rokz::Device& device); 
  // bool BindGlobalDescriptorResources (Vec<VkDescriptorSet>& descs, const Vec<rokz::Buffer>& buffs, const rokz::Device& device);
  // void UpdateGlobals (rokz::DrawSequence::Globals& shared, const rokz::Buffer& buf, const VkExtent2D& viewext, double dt);

  // bool SetupRenderingAttachments (rokz::Image&          msaa_color_image       ,
  //                                 rokz::ImageView&      msaa_color_imageview   ,

  //                                 rokz::Image&          msaa_depth_image       ,
  //                                 rokz::ImageView&      msaa_depth_imageview   ,

  //                                 VkSampleCountFlagBits msaa_samples           ,
  //                                 VkFormat              swapchain_image_format ,
  //                                 VkFormat              msaa_depth_format      ,
  //                                 const VkExtent2D&     image_ext, 
  //                                 const rokz::Device&   device);

  
  // ----------------------------------------------------------------------------------------
  //                     
  // ----------------------------------------------------------------------------------------
  void CleanupSwapchain (Vec<rokz::ImageView>& sc_image_views,
                         rokz::Image&          msaa_color_image,
                         rokz::ImageView&      msaa_color_imageview,
                         rokz::Image&          depth_image,
                         rokz::ImageView&      depth_imageview,
                         rokz::Swapchain&      swapchain,
                         const rokz::Device&   device);
  

  bool RecreateSwapchain (rokz::Swapchain&  swapchain, const rokz::Window& win, 
                          Vec<rokz::Image>& swapchain_images, Vec<rokz::ImageView>& imageviews,
                          rokz::Image& msaa_depth_image, rokz::ImageView& msaa_depth_imageview,
                          rokz::Image& msaa_color_image, rokz::ImageView& msaa_color_imageview,
                          const VmaAllocator& allocator, const rokz::Device& device); 

  //void SetupViewportState (rokz::ViewportState & vps, const VkExtent2D& swapchain_extent); 
  rokz::SwapchainResetter::Ref
  CreateSwapchainResetter (rokz::Swapchain& sc, Vec<rokz::Image>& scis, Vec<rokz::ImageView>& scivs,
                           rokz::Image& dp, rokz::ImageView& div, rokz::Image& mscim, rokz::ImageView& mscimv); 


  rokz::SwapchainResetter::Ref CreateSwapchainResetter (rokz::Swapchain& sc, Vec<rokz::Image>& scis,
                                                       Vec<rokz::ImageView>& scivs,
                                                       rokz::Image& dp, rokz::ImageView& div,
                                                       rokz::Image& mscim, rokz::ImageView& mscimv);

  bool SetupDynamicRenderingInfo (rokz::RenderingInfoGroup& ri,
                                  const rokz::ImageView&    msaa_color_imageview ,
                                  const rokz::ImageView&    msaa_depth_imageview ,
                                  const VkExtent2D&         image_extent); 
  void UpdateDynamicRenderingInfo (rokz::RenderingInfoGroup& ri,
                                   const rokz::ImageView&    msaa_color_imageview ,
                                   const rokz::ImageView&    target_imageview); 
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
