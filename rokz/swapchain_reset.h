#ifndef ROKZ_SWAPCHAIN_RESET
#define ROKZ_SWAPCHAIN_RESET


#include "common.h"
#include <memory>
#include "rokz_types.h"



namespace rokz {



  // ---------------------------------------------------------------------
  // 
  // ---------------------------------------------------------------------
  struct ResetSwapchainCB {
    
  public:

    typedef std::shared_ptr<ResetSwapchainCB> Ref; 
    
    virtual bool ResetSwapchain  (const rokz::Window& win, const rokz::Allocator& allocator, const rokz::Device& device) = 0;


  protected:
  
    ResetSwapchainCB () {}
  };




  

}
#endif
