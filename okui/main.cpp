
#include "okui.h"

using namespace rokz;

// -------------------------------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------------------------------
struct OK_DeviceFeatures : public VkPhysicalDeviceFeatures2 {

  OK_DeviceFeatures (const rokz::PhysicalDevice& physdev) : VkPhysicalDeviceFeatures2 {} {

    separate_depth_stencil_layout_feature.sType                       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SEPARATE_DEPTH_STENCIL_LAYOUTS_FEATURES;
    separate_depth_stencil_layout_feature.pNext                       = VK_NULL_HANDLE;
    separate_depth_stencil_layout_feature.separateDepthStencilLayouts = VK_TRUE;

    sType    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2; 
    pNext    = &separate_depth_stencil_layout_feature; 
    features = physdev.features2.features;             
  }
  
  // ext structs
  VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures separate_depth_stencil_layout_feature {}; 
 // VK_KHR_synchronization2
 // VK_KHR_timeline_semaphore
 // VK_EXT_color_write_enable
 // VK_EXT_extended_dynamic_state3
 // VK_EXT_separate_stencil_usage
 // VK_EXT_transform_feedback
}; 

// -------------------------------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------------------------------
int okui::run (const Vec<std::string>& args) {

  okui::Glob glob; 

  rc::SwapchainGroup&                     scg  = glob.swapchain_group;
  //  Arr<per_frame_set, kMaxFramesInFlight>& sync = glob.sync; 


  glob.mouse_prev.x = 0;
  glob.mouse_prev.y = 0;

  systempath pipe_path = OWENS_LAKE"/rokz/pipeline"; //
  systempath data_path = OWENS_LAKE"/rokz/data";     // 

  //Default (glob); 
  glfwInit();
  
  rokz::InitializeInstance (glob.instance); 

  rokz::SetupDisplay (glob.display, glob.input_state, "milkshake", kDefaultDimensions , glob.instance); 
  
  rokz::cx::SelectPhysicalDevice (glob.device.physical, glob.display.surface, glob.instance.handle);
  //
  rokz::cx::QuerySwapchainSupport (glob.swapchain_info, glob.display.surface, glob.device.physical.handle);

  // -- replace rokz::ConfigureFeatures  (feats, glob.device.physical);
  OK_DeviceFeatures ok_feats  (glob.device.physical);

  // this does a lot of shit
  //rokz::InitializeDevice (glob.device, glob.device.physical, glob.instance);
  rokz::InitializeDevice (glob.device, ok_feats, glob.device.physical, glob.instance);


  return 0; 
}

// -------------------------------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------------------------------
int main (int argv, char** argc) {

  Vec<std::string> args (argc, argc + argv); 
  // return okui::imgui_test (argv, argc);
  //return im_demo (argv, argc);
  return okui::run (args); 
}
