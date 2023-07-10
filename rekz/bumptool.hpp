#ifndef REK_BUMP_TOOL_H
#define REK_BUMP_TOOL_H

#include "rekz.h"


namespace rekz { namespace bump { 

    struct InHeight {};

    struct BumpParams {
    };

    struct OutBump {

    };

    OutBump& make_bump (OutBump& out,  const BumpParams&  params, const InHeight& in); 
  }}

#endif
