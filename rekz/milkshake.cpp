
#include "milkshake.h"
#include "dorito_pipe.h"

#include "rokz/attachment.hpp"
#include "rokz/context.hpp"
#include "rokz/rc_image.hpp"
#include <vulkan/vulkan_core.h>


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
bool setup_color_render_attachments (milkshake::Glob& glob) {

  using namespace milkshake;

  const Device& device = glob.device; 
  const VkSampleCountFlagBits sample_bit_count = VK_SAMPLE_COUNT_1_BIT; 
  const VkImageUsageFlags color_target_usage   = rokz::kColorTargetUsage | VK_IMAGE_USAGE_SAMPLED_BIT; 
  const VkImageUsageFlags depth_target_usage   = rokz::kDepthStencilUsage | VK_IMAGE_USAGE_SAMPLED_BIT; 

  glob.msaa_samples = sample_bit_count; 
  
  Arr<rc::Attachment, NUM_COLOR_ATTACHMENTS>& colorattach = glob.attachment.color; 
  rc::Attachment&                                   depthattach = glob.attachment.depth;

  { // position
    VkImageCreateInfo ci {}; 

    cx::CreateInfo_2D (ci, VK_FORMAT_R16G16B16A16_SFLOAT, color_target_usage, sample_bit_count,
                       kDefaultDimensions.width, kDefaultDimensions.height);

    colorattach[COLATT_POSITION].format = VK_FORMAT_R16G16B16A16_SFLOAT; 
    colorattach[COLATT_POSITION].image  = rc::CreateImage (ci, device);
    colorattach[COLATT_POSITION].view   ; 
    // create imageview...
    assert (false); 
  }

  { // normal
    VkImageCreateInfo ci {}; 

    cx::CreateInfo_2D (ci, VK_FORMAT_R16G16B16A16_SFLOAT, color_target_usage, sample_bit_count,
                       kDefaultDimensions.width, kDefaultDimensions.height);
    colorattach[COLATT_NORMAL].format  =  VK_FORMAT_R16G16B16A16_SFLOAT;
    colorattach[COLATT_NORMAL].image =  rc::CreateImage (ci, device);
    colorattach[COLATT_NORMAL].view; 
    // create imageview...
    assert (false); 

  }

  { // albedo
    VkImageCreateInfo ci {}; 

    cx::CreateInfo_2D (ci, VK_FORMAT_R16G16B16A16_SFLOAT, color_target_usage, sample_bit_count,
                       kDefaultDimensions.width, kDefaultDimensions.height);

    colorattach[COLATT_ALBEDO].format = VK_FORMAT_R16G16B16A16_SFLOAT;
    colorattach[COLATT_ALBEDO].image  = rc::CreateImage (ci, device);
    colorattach[COLATT_ALBEDO].view ; 

    // create imageview...
    assert (false); 
  }

  // { // specular
            
  // }
  
  { // depth 
    VkImageCreateInfo ci {}; 
    rokz::ut::FindDepthFormat (glob.depth_format, glob.device.physical.handle);
  
    cx::CreateInfo_2D (ci, glob.depth_format, depth_target_usage, sample_bit_count, 
                       kDefaultDimensions.width, kDefaultDimensions.height);

    //glob.msaa_samples = rokz::ut::MaxUsableSampleCount (glob.device.physical); 
    depthattach.image = rc::CreateImage (ci, device); 

    // create imageview...
    assert (false); 
  }

  return true; 
}

// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
rokz::SwapchainResetter::Ref  CreateResetDorito () {
  assert (false); 
  struct dorito_resetter : public SwapchainResetter {
    
  public:
    virtual bool Reset (const rokz::Display& display, const rokz::Device& device) {

      return false; 
    }

  };


  return std::make_shared<dorito_resetter> (); 
  
}
// --------------------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------------------
int milkshake::run (const Vec<std::string>& args) {
  HERE("hai");

  milkshake::Glob glob; 

  rc::SwapchainGroup&                     scg      = glob.swapchain_group;
  Arr<per_frame_set, kMaxFramesInFlight>& sync = glob.sync; 


  glob.mouse_prev.x = 0;
  glob.mouse_prev.y = 0;

  systempath pipe_path = "/home/djbuzzkill/owenslake/rokz/pipeline";
  systempath data_path = "/home/djbuzzkill/owenslake/rokz/data"; // 
  //Default (glob); 

  
  glfwInit();
  
  rokz::InitializeInstance (glob.instance); 

  rekz::SetupDisplay (glob.display, "milkshake", glob.input_state, kDefaultDimensions , glob.instance); 
  
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



  //rc::SetupMSAARenderingAttachments
  setup_color_render_attachments (glob); 
  
  glob.swapchain_resetter =  CreateResetDorito () ; 
  // rekz::CreateSwapchainResetter (scg.swapchain, scg.images, scg.imageviews,
  //                                                        glob.depth_image, glob.depth_imageview,
  //                                                        glob.msaa_color_image, glob.msaa_color_imageview); 

  // for BeginRendering ()
  assert (false); 
  //rokz::SetupDynamicRenderingInfo;//  (glob.rendering_info_group, glob.msaa_color_imageview->handle,
                                  // glob.depth_imageview->handle, scg.extent); 

  
  // define first 
  rokz::DefineDescriptorSetLayout (glob.grid_dslo, dorito::kDescriptorBindings, glob.device); 

  rokz::DefineDescriptorSetLayout (glob.dorito_dslo, dorito::kDescriptorBindings, glob.device); 

  
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
