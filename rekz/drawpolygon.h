
#ifndef REKZ_DRAWPOLYGON_INCLUDE 
#define  REKZ_DRAWPOLYGON_INCLUDE


#include "rekz.h"

#include "polyobdata.h"

namespace rekz { namespace polyob {

    using namespace rokz; 

    DrawSequence::Ref CreateDrawPoly (const PolygonData&                  dat,
                                      const std::vector<rc::Buffer::Ref>& objres,
                                      const rokz::DescriptorGroup&        descg); 


  }}


#endif
