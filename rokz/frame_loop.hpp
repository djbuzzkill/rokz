#ifndef ROKZ_FRAME_LOOP
#define ROKZ_FRAME_LOOP


namespace rokz {

  template<typename LoopTy>
    inline bool FrameLoop (LoopTy& apploop) {

    while (apploop.cond ()) { if (!apploop.loop ())  return false; }

    return true;
  }


}


#endif 
