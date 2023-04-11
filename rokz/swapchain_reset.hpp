//

#ifndef ROKZ_SWAPCHAIN_RESET
#define ROKZ_SWAPCHAIN_RESET


#include "common.h"
#include "rokz_types.h"
#include "display.h"



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
