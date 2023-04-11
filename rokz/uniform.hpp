
#ifndef ROKZ_UNIFORM_H
#define ROKZ_UNIFORM_H

#include "common.hpp"
#include "utility.hpp"
#include "rokz_types.hpp"
#include "buffer.hpp"


namespace rokz{

  bool CreateUniformBuffer (rokz::Buffer& buf, size_t sizeOf_el, size_t numberOf_el, const rokz::Device& device); 
  // nada
}

#endif
