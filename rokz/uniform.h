
#ifndef ROKZ_UNIFORM_H
#define ROKZ_UNIFORM_H

#include "common.h"
#include "utility.h"
#include "rokz_types.h"
#include "buffer.h"


namespace rokz{

  bool CreateUniformBuffer (rokz::Buffer& buf, size_t sizeOf_el, size_t numberOf_el, const rokz::Device& device); 
  // nada
}

#endif
