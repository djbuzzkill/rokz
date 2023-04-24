
#include "milkshake.h"
#include "rokz/attachment.hpp"

#include "dust_pipe.h"
#include "rokz/context.hpp"


using namespace rokz; 


const VkExtent2D kDefaultDimensions { 1024, 768 }; 
// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
void cleanup_milkshake (milkshake::Glob& glob) {

}




// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
struct MilkLoop {


  MilkLoop (milkshake::Glob& glob, double Dt) {
  }
  
  bool cond () { return false; } 
  bool loop () { return false; } 
 
};


// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
bool SetupColorRenderAttachment (); 


// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
rokz::SwapchainResetter::Ref  CreateDustyResetter () {

  assert (false); 

  struct dusty_resetter : public SwapchainResetter {
    
  public:
    virtual bool Reset (const rokz::Display& display, const rokz::Device& device) {

      return false; 
    }

  };


  return std::make_shared<dusty_resetter> (); 
  
}
// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
int milkshake::run (const Vec<std::string>& args) {
  HERE("hai");

  milkshake::Glob glob; 

  rc::SwapchainGroup&  scg   = glob.swapchain_group;
  std::array<PerFrameSet, milkshake::kMaxFramesInFlight>& framsets = glob.framesets; 


  glob.mouse_prev.x = 0;
  glob.mouse_prev.y = 0;

  systempath pipe_path = "/home/djbuzzkill/owenslake/rokz/pipeline";
  systempath data_path = "/home/djbuzzkill/owenslake/rokz/data"; // 
  //Default (glob); 

  
  glfwInit();
  
  rokz::InitializeInstance (glob.instance); 

  rekz::SetupDisplay (glob.display, "marz", glob.input_state, kDefaultDimensions , glob.instance); 
  
  rokz::cx::SelectPhysicalDevice (glob.device.physical, glob.display.surface, glob.instance.handle);
  //
  rokz::cx::QuerySwapchainSupport (glob.swapchain_support_info, glob.display.surface, glob.device.physical.handle);

  VkPhysicalDeviceFeatures2 f2 {};
  rokz::ConfigureFeatures  (f2, glob.device.physical);
  // this does a lot of shit
  //rokz::InitializeDevice (glob.device, glob.device.physical, glob.instance);
  rokz::InitializeDevice (glob.device, f2, glob.device.physical, glob.instance);
  
  // put these somwehere
  glob.msaa_samples = rokz::ut::MaxUsableSampleCount (glob.device.physical); 
  rokz::ut::FindDepthFormat (glob.depth_format, glob.device.physical.handle);
  // InitializeSwapchain ()
  rc::InitializeSwapchain (scg, glob.swapchain_support_info, glob.display.surface,
                            kDefaultDimensions, glob.device.physical, glob.device);
  //
  const size_t NuberOfColorTargets = 2; 
  // SetupColorRenderAttachment ( glob.colortargets, glob.colorviews, NuberOfColorTargets,  );
  // SetupColorRenderAttachment ();
  SetupColorRenderAttachment;   
  
  // rc::SetupMSAARenderingAttachments (glob.msaa_color_image, glob.msaa_color_imageview, 
  //                                    glob.depth_image, glob.depth_imageview,
  //                                    glob.msaa_samples, scg.image_format,
  //                                    glob.depth_format, scg.extent,
  //                                    glob.device); // <-- this does all the additional  attachmentes
  // //



  glob.swapchain_resetter =   CreateDustyResetter () ; 
  // rekz::CreateSwapchainResetter (scg.swapchain, scg.images, scg.imageviews,
  //                                                        glob.depth_image, glob.depth_imageview,
  //                                                        glob.msaa_color_image, glob.msaa_color_imageview); 

  // for BeginRendering ()
  assert (false); 
  //rokz::SetupDynamicRenderingInfo;//  (glob.rendering_info_group, glob.msaa_color_imageview->handle,
                                  // glob.depth_imageview->handle, scg.extent); 

  
  // define first 
  rokz::DefineDescriptorSetLayout (glob.grid_dslo, dust::kDescriptorBindings, glob.device); 

  rokz::DefineDescriptorSetLayout (glob.dust_dslo, dust::kDescriptorBindings, glob.device); 

  
  //
  rokz::SetupGlobalUniforms (glob.global_bu, kMaxFramesInFlight, glob.device); 

  // pipelines...

  // data...

  // descriptor sets..

  // draw drawlists

    
  // create frame syncs
  assert (false); 
  // fsg.command_buffers.resize (kMaxFramesInFlight);
  // fsg.syncs.resize           (kMaxFramesInFlight);
  // rokz::cx::AllocateInfo (fsg.command_buffer_alloc_info, glob.device.command_pool.handle); 

  // for (size_t i = 0; i < kMaxFramesInFlight; ++i) {
  //   // ^^ 'CreateCommandBuffers' should be called, we call it 
  //   rokz::cx::CreateCommandBuffer(fsg.command_buffers[i], fsg.command_buffer_alloc_info, glob.device.handle);
  //   rokz::cx::CreateFrameSync (fsg.syncs[i], fsg.syncs[i].ci, glob.device.handle);
  // } 

  
  const double time_per_frame_sec = 1.0 / 60.0;
  double Dt = time_per_frame_sec; // just do this for now
  // auto t0 = std::chrono::high_resolution_clock::now(); 
  // std::chrono::system_clock::time_point then = t0; 
  MilkLoop milkloop (glob, Dt); 
  rokz::FrameLoop  (milkloop);


  HERE(" milkloope exited "); 
  vkDeviceWaitIdle(glob.device.handle);

//   // CLEAN UP
   cleanup_milkshake (glob); 


  printf ("res path:%s\n", OWENS_LAKE); 
  
  return 0; 
}
