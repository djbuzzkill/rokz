
#include "rockz.h"
#include "rokz/rokz.h"
#include <vulkan/vulkan_core.h>

//#include "skittlez/skittlez.h"


void SetupScene () {
  printf ("lolz\n"); 
}

void ShutdownScene () {
  printf ("lolz\n"); 
}


//
void UpdateInput (rokz::Glob& glob, double dt) {

}

  //
void UpdateScene (rokz::Glob& glob, double dt) {

  // for (int i = 0; i < 1000000; ++i ) {
      
  //   std::cos ( 3.1459265 * i * 0.000001); 
  // }
}

//
void RenderScene (rokz::Glob &glob, double dt) {
  
}

//
//
int main (int argv, char** argc) {

  //VkInstance  vkinst;
  //GLFWwindow* glfwin = nullptr; 
  double      dt = 0.0;

  rokz::Glob glob; // something representing the app state

  Default (glob); 
  
  glfwInit();

  rokz::CreateWindow_glfw (glob.glfwin);

  rokz::CreateInstance    (glob);

  rokz::CreateSurface (&glob.surface, glob.glfwin , glob.instance);

  rokz::SelectPhysicalDevice (glob.physical_device, glob.queue_fams, glob.surface, glob.instance);

  // queue info

  //rokz:: QueueFamilyIndices fam_inds;

  //fam_inds.graphics =  glob.queue_fams.graphics;
  //fam_inds.present =  glob.queue_fams.present;
  
  
  glob.queue_priority = 1.0f;

  if (glob.queue_fams.graphics.has_value()) {
    printf ("HAS_VALUE:TRUE\n"); 
  }
  else  {
    printf ("HAS_VALUE:FALSE\n"); 
  }
  
  rokz::Default (glob.create_info.queue, glob.queue_fams.graphics.value() , &glob.queue_priority);

  // device info
  rokz::Default (glob.create_info.device, &glob.create_info.queue, &glob.device_features); 

  
  printf (" present ind[%u]\n",  glob.queue_fams.present.value());
  rokz::CreateLogicalDevice (&glob.device, &glob.create_info.device, glob.physical_device); 
  printf (" graphics ind[%u]\n",  glob.queue_fams.graphics.value());

  // get queue handle
  rokz::GetDeviceQueue (&glob.queues.graphics, glob.queue_fams.graphics.value(), glob.device);

  rokz::GetDeviceQueue (&glob.queues.present, glob.queue_fams.present.value(), glob.device);


  
  rokz::CreateSwapchain (glob.swapchain, glob.create_info.swapchain,
                         glob.surface, glob.physical_device,
                         glob.device,  glob.glfwin);

  
  rokz::GetSwapChainImages (glob.swapchain_images, glob.swapchain, glob.device); 

 
  rokz::CreateImageViews (glob.swapchain_imageviews,
                          glob.swapchain_images,
                          glob.create_info.swapchain.imageFormat, 
                          glob.device); //  (std::vector<VkImageView>& swapchain_imageviews);

  rokz::CreatePipeline; 


  SetupScene (); 
  //
  //
  const double time_per_frame_sec = 1.0 / 60.0;
  dt = time_per_frame_sec; // just do this for now
  
  std::chrono::microseconds
    time_per_frame_us (static_cast<size_t> (time_per_frame_sec * 1000000.0));

  std::chrono::duration<size_t, std::chrono::microseconds::period>
    time_per_frame (time_per_frame_us);
  
  size_t frame_counter = 0;

  // loop 
  bool run = false; 
  while (run) {
    printf("frame[%zu]\n", frame_counter++);
    auto start = std::chrono::high_resolution_clock::now();

    // while (!glfwWindowShouldClose(glob.glfwin)) {
    //     glfwPollEvents(); <-- not tested
    // }
    UpdateInput(glob, dt);

    UpdateScene (glob, dt);

    RenderScene (glob, dt);


    // how long did we take
    auto time_to_make_frame = std::chrono::high_resolution_clock::now() - start;
    if (time_to_make_frame  < time_per_frame) {
      auto sleep_time = time_per_frame - time_to_make_frame;
      std::this_thread::sleep_for (sleep_time); 
    }

  }
  // end loop

  ShutdownScene(); 

  
  if (!run) {
    printf ("config just exit atm\n"); 
    std::this_thread::sleep_for (std::chrono::milliseconds (300)); 
  }

  // CLEAN UP
  rokz::Cleanup (glob.swapchain, glob.surface,
                 glob.swapchain_imageviews, 
                 glob.glfwin ,
                 glob.device,
                 glob.instance);
  //void Cleanup (VkSurfaceKHR& surf, GLFWwindow* w, VkDevice& dev, VkInstance &vkinst);
  glfwTerminate();

  return 0; 
}




void look_at_this_shhhhi () {
  // auto tup = std::tuple{ 420, "wtf", 3.15f}; 
  // auto& [a1, b1, c1] = tup; 
  }
