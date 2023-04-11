//

#ifndef ROKZ_SWAPCHAIN_RESET
#define ROKZ_SWAPCHAIN_RESET


#include "common.hpp"
#include "rokz_types.hpp"
#include "display.hpp"



namespace rokz {

  // --------------------------------------------------------------------------------------------
  //                         
  // --------------------------------------------------------------------------------------------
  struct SwapchainResetter {
    
    typedef std::shared_ptr<SwapchainResetter> Ref; 

  public:

    virtual bool Reset (const rokz::Display& display, const rokz::Device& device) = 0;

  protected:
  
    SwapchainResetter () {}

  };

}
#endif
