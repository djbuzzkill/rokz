
#ifndef ROKZ_FUNCS_INCLUDE
#define ROKZ_FUNCS_INCLUDE


#include "common.h"
#include "utility.h"
#include "rokz_types.h"


namespace rokz {


  // ------------------------------------------------------------------
  //
  // ------------------------------------------------------------------
  
  bool CreateFrameBuffers ();
  bool CreateCommandPool (); 
  bool CreateCommandBuffer ();
  bool CreateSyncObjects ();

} // namespace rokz

#endif
