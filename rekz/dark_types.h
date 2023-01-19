
#ifndef DARKROOT_GLOB_INCLUDE
#define DARKROOT_GLOB_INCLUDE

#include "rokz/rokz.h"


namespace darkroot {

  // --------------------------------------------------------------------
  // Geometry
  // --------------------------------------------------------------------
  struct PushConstants {

    glm::ivec4 drawIDs; 
    // x: object index
    // y: unused
    // z: unused
    // w: unused

  }; 
  

  // --------------------------------------------------------------------
  // Geometry
  // --------------------------------------------------------------------
  struct KeyState {

    std::map<int, int> keys; 
  };

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  struct MouseState {

    int inside; 
    
    int left_butt;   
    int right_butt;  
    int middle_butt; 
    
    int x_pos;
    int y_pos;

    int dx;
    int dy;
  }; 


  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  template<typename Ty> struct PolarCoord {
    Ty theta; // "longitude" 0-360
    Ty phi;   // "latitude"  -90 - +90
  }; 

  typedef PolarCoord<float>  Polarf;
  typedef PolarCoord<double> Polard;
  
  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  struct InputState {

    std::map<int, int> keys;
    MouseState         mouse;
    Polarf             polar;
  };

  
  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  struct PipelineGroup { 

    rokz::Pipeline        pipeline;
    rokz::DescriptorGroup descrgroup;

    
  };

  // --------------------------------------------------------------------
  // Geometry
  // --------------------------------------------------------------------
  template<typename VTy, typename IndTy>
    struct GeometryData {

      typedef  VTy         VertexType; 
      typedef  IndTy       IndexType; 

      enum { VertexSize = sizeof(VTy) }; 
      enum { IndexSize  = sizeof(IndTy) }; 
  
      std::vector<VTy> verts;
      std::vector<IndTy> indices;
    }; 



  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  template<typename VTy> 
    using TriMesh = GeometryData<VTy, uint16_t>; 


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

  // --------------------------------------------------------------------
  // 
  // --------------------------------------------------------------------
  namespace HalfEdge {

    constexpr int32_t NullIndex = -1; 
    typedef   int32_t Index; 

    // --------------------------------------------------------------------
    struct Face {
      Index edgei;
    }; 
        
    // --------------------------------------------------------------------
    struct Edge {
      Index verti; 
      Index pairi; 
      Index facei; 
      Index nexti; 
    }; 
      
    // --------------------------------------------------------------------
    typedef std::vector<Edge> Edges;
    typedef std::vector<Face> Faces;

    // --------------------------------------------------------------------
    struct BRep {
      // edgei[i] -> geometry.verts[i] 
      std::vector<Index> edgei;
      // edge store
      Edges edges;
      // face store
      Faces faces;
    }; 

  } // HalfEdge


  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  struct DarkrootVert {
    glm::vec3 pos; 
    glm::vec3 nrm; 
    glm::vec3 col; 
    glm::vec2 txc0; 
  };

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  typedef TriMesh<DarkrootVert> DarkrootMesh;

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  struct DarkRenderable : public Renderable {

    DarkRenderable () : num_verts (0), num_inds (0), vb_dev() , ib_dev () {
    }

    DarkRenderable (uint32_t v, uint32_t i) : num_verts (v), num_inds (i), vb_dev() , ib_dev () {
    }

    virtual auto SetupRS (VkCommandBuffer commandbuffer) -> int;
    virtual auto Draw (VkCommandBuffer commandbuffer) -> void;
    virtual auto AllocRes (VmaAllocator& allocator) -> int;
    virtual auto FreeRes (VmaAllocator& alloc) -> int;

    uint32_t num_verts;
    uint32_t num_inds;
  
    rokz::Buffer vb_dev;
    rokz::Buffer ib_dev;

    glm::vec3 pos;
    glm::quat qrot;
  
    friend DarkRenderable& Friendly ( DarkRenderable& dr); 

  protected:
  };

  

  // --------------------------------------------------------------------
  //
  // --------------------------------------------------------------------
  struct Glob {


    InputState input_state;
    //#ifdef GLOB_COMMENT_OUT   
    enum { MaxFramesInFlight = 2 }; 
    Glob();

    // device props
    VkFormat                     depth_format;
    VkSurfaceFormatKHR           surface_format;
    VkSampleCountFlagBits        msaa_samples; //  = VK_SAMPLE_COUNT_1_BIT;

    // system
    rokz::Instance               instance;
    rokz::PhysicalDevice         physical_device;
    rokz::Device                 device;
    rokz::SwapchainGroup         swapchain_group;


    
    rokz::FrameSequencing        frame_sequence;
    rokz::SwapchainSupportInfo   swapchain_support_info;

    // global pools
    //    rokz::CommandPool            command_pool;
    rokz::DescriptorPool         descr_pool;

    // pipelines
    PipelineGroup                obj_pipeline;
    PipelineGroup                grid_pipeline;

    // DYNAMIC RENDERING
    rokz::RenderingInfoGroup     rendering_info_group;
    


    // attachement set
    rokz::Image                  depth_image;
    rokz::ImageView              depth_imageview; 
    rokz::Image                  msaa_color_image;
    rokz::ImageView              msaa_color_imageview; 
  

    // scene objects 
    float obj_theta[2];

    // object data
    rokz::Buffer                vma_ib_device;
    rokz::Buffer                vma_vb_device;
    // image/texture
    rokz::Image                 texture_image; 
    rokz::ImageView             texture_imageview; 
    rokz::Sampler               sampler;

    // pipeline resources
    std::vector<rokz::Buffer>   vma_uniform_buffs;
    std::vector<rokz::Buffer>   vma_objparam_buffs;


    // window
    rokz::Window                window;
    VkSurfaceKHR                surface; // 
    bool                        fb_resize; 

    //VkViewport                  viewport;
    //VkRect2D                    scissor_rect; 

    double                      sim_time; 
    float                       dt;

  };
  // --------------------------------------------------------------------


  // ---------------------------------------------------------------------
  // image handling - should we keep
  // ---------------------------------------------------------------------
  struct DevILImageProps {

    int width          ;//= ilGetInteger (IL_IMAGE_WIDTH); 
    int height         ;//= ilGetInteger (IL_IMAGE_HEIGHT);
    int depth          ;//= ilGetInteger (IL_IMAGE_DEPTH);
    int bytes_per_pixel; //= ilGetInteger (IL_IMAGE_BYTES_PER_PIXEL); 
    int bpp            ;//= ilGetInteger (IL_IMAGE_BPP);
    int type           ;//= ilGetInteger (IL_IMAGE_TYPE);
    int format         ;//= ilGetInteger (IL_IMAGE_FORMAT); 
  };


  typedef int (*DevILOpenFileCB) (const unsigned char* dat, const DevILImageProps& props, void* up); 

  int OpenImageFile (const std::string& fqname, DevILOpenFileCB cb, void* up);

  // ---------------------------------------------------------------------
  // load texture to device memory
  // ---------------------------------------------------------------------
  bool LoadTexture_color_sampling (rokz::Image&             image,
                                 VkFormat                 format,
                                 const VkExtent2D&        ext2d,
                                 const void*              srcimage,
                                 const VmaAllocator&      allocator, 
                                 const VkQueue&           queue, 
                                 const rokz::CommandPool& commandpool, 
                                 const rokz::Device&      device);
  
  
  
} // darkroot

#endif
