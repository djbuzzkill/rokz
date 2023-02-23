
#ifndef DARKROOT_GLOB_INCLUDE
#define DARKROOT_GLOB_INCLUDE

#include "rokz/utility.h"
#include "rokz/image.h"
#include "rokz/rokz.h"
#include "rekz.h"
#include "rokz/rokz_types.h"
#include <vulkan/vulkan_core.h>

namespace darkroot {

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  typedef rekz::Vertex_pos_nrm_txc_col DarkVert;

  // --------------------------------------------------------------------
  typedef rekz::TriMesh<DarkVert> DarkMesh;

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  struct PushConstants {

    glm::ivec4 drawIDs; 
    // x: object index
    // y: unused
    // z: unused
    // w: unused
  }; 
  
  // --------------------------------------------------------------------
  // UBO
  // --------------------------------------------------------------------
  struct SceneObjParam {
    glm::mat4 modelmat;
    //  glm::mat4 unused0;
  };


  template<typename Ty> struct Trait {

    int    Fn ();
    float  Fl ();
    Ty     Ft ();
    
  };

  // --------------------------------------------------------------------
  // PolygonData polyd resources the polygon pipeline will use
  // --------------------------------------------------------------------
  struct PolygonData { 
    rokz::Buffer          vb_device;
    rokz::Buffer          ib_device;
    rokz::DescriptorGroup descrgroup;
    // image/texture
    rokz::Image           texture;   // color texture
    rokz::ImageView       imageview; // 
    rokz::Sampler         sampler;   // 

    float obj_theta[2];     // scene objects 

    float                       dt;

    // pipeline resources
    std::vector<rokz::Buffer>    vma_uniform_buffs;
    std::vector<rokz::Buffer>    vma_objparam_buffs;

  } ;



  // --------------------------------------------------------------------
  struct GridData {
    // some shit like this
    rokz::Buffer          vb_device;
    rokz::Buffer          ib_device;
    rokz::DescriptorGroup descrgroup;
    
  };
  
  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  struct pipeline_assembly  {
    rokz::Pipeline&       pipeline;
    VkPipelineLayout      plo;
    VkDescriptorSet       descrset;
  }; 
  // ---------------------------------------------------------------------
  // ?? is a pipeline  tied to a drawsequence?  
  // --  DrawPolygons is part of obj_pipeline 
  // ---------------------------------------------------------------------
  struct DrawSequence : public rokz::ut::destructor {

    //virtual int Exec (VkCommandBuffer comb, const rokz::Pipeline& pl, const VkDescriptorSet* ds) = 0;
    // 
    // do crap before recording ("UpdateDescriptors()", etc)
    virtual int Prep (const pipeline_assembly& pa, const rokz::Device& device) = 0; 
    //
    // the draw sequence recording, mebe rename to DrawSeq::Rec() 
    virtual int Exec (VkCommandBuffer comb, const pipeline_assembly& pa, const VkDescriptorSet* ds) = 0;

    //virtual int UpdateDescriptors (const rokz::DescriptorPool& descpool, const rokz::Device& device) = 0; 
    // ?? who owns the descriptor set, should it be allocated with the drawseq or the pipeline
    
  protected:

    DrawSequence () {}

  }; 


  std::shared_ptr<DrawSequence> CreatePolygonTextured (const darkroot::PolygonData& d); 
  std::shared_ptr<DrawSequence> CreatePolygonWireframe (const darkroot::PolygonData& d); 
  std::shared_ptr<DrawSequence> CreateGridDraw (int dumb); 
  // ---------------------------------------------------------------------
  // 
  // ---------------------------------------------------------------------
  struct ResetSwapchainCB {

  public:
  
    virtual bool ResetSwapchain  (const rokz::Window& win, const rokz::Allocator& allocator, const rokz::Device& device) = 0;

  protected:
  
    ResetSwapchainCB () {}
  };

 
  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  struct Glob {

    //#ifdef GLOB_COMMENT_OUT   
    enum { MaxFramesInFlight = 2 }; 
    Glob();


    rekz::Polarf                view_orie;
    int                         prev_x;
    int                         prev_y; 
    int                         prev_inside;

    rekz::InputState            input_state;


    // struct Display
    rokz::Window                window;
    VkSurfaceKHR                surface; // 

    // device props
    VkFormat                     depth_format;
    //VkSurfaceFormatKHR           surface_format;
    VkSampleCountFlagBits        msaa_samples; //  = VK_SAMPLE_COUNT_1_BIT;

    // system
    rokz::Instance               instance;
    //rokz::PhysicalDevice         physical_device;
    rokz::Device                 device;
    rokz::SwapchainGroup         swapchain_group;
    rokz::SwapchainSupportInfo   swapchain_support_info;
    rokz::FrameSyncGroup         framesyncgroup;

    // 
    rokz::Pipeline                polys_pl  ;
    rokz::PipelineLayout          polys_plo ;
    rokz::DescriptorSetLayout     polys_dslo;
    //rokz::DescriptorGroup         polys_descrg;
    PolygonData                   polyd;
    std::shared_ptr<DrawSequence> poly_draw;
    
    // 
    rokz::Pipeline                grid_pl  ;
    rokz::PipelineLayout          grid_plo ;
    rokz::DescriptorSetLayout     grid_dslo;
    //rokz::DescriptorGroup         grid_descrg;
    std::shared_ptr<DrawSequence> grid_draw;
    GridData                      gridata;

    
    // attachement set
    rokz::Image                  depth_image;
    rokz::ImageView              depth_imageview; 
    rokz::Image                  msaa_color_image;
    rokz::ImageView              msaa_color_imageview; 
  
    // DYNAMIC RENDERING
    rokz::RenderingInfoGroup     rendering_info_group;

    //VkViewport                  viewport;
    //VkRect2D                    scissor_rect; 

    std::map<std::string, std::shared_ptr<DrawSequence>> draw_seqs; 
    
    std::map<DrawSequence*, rokz::DescriptorGroup>       descrgmap; 
    
    std::shared_ptr<darkroot::DrawSequence>     polygons; 
    std::shared_ptr<darkroot::ResetSwapchainCB> swapchain_reset_cb;

    double                      sim_time; 

  };
  // --------------------------------------------------------------------


  
  
  
#ifdef DARkROOT_ENABLE_RENDERABLE
  // --------------------------------------------------------------------
  // struct Renderable 
  // --------------------------------------------------------------------
  struct Renderable {

    virtual auto SetupRS (VkCommandBuffer commandbuffer) -> int = 0;
    virtual auto Draw    (VkCommandBuffer commandbuffer) -> void = 0;
    
    virtual auto AllocRes (VmaAllocator& alloc) -> int = 0; 
    virtual auto FreeRes  (VmaAllocator& alloc) -> int = 0; 

  protected:
    Renderable () = default; 
  
  }; 
#endif
} // darkroot

#endif
