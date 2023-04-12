
#ifndef ROKZ_UT_OFFSET_INCLUDE
#define ROKZ_UT_OFFSET_INCLUDE

#include "common.hpp"
#include "shared_types.hpp"


namespace rokz { namespace ut { 

    // ---------------------------------------------------------------------------------------
    template<int N> inline size_t
      offset_at (const size_t sizez[N], size_t index) {

      if (index >= N)
        return 0;

      size_t acc = 0;

      for (size_t i = 0; i < index; ++i)
        acc += sizez[i];

      return acc;
    
    }
  
    template<size_t N> inline size_t
      offset_at (const std::array<size_t, N>& sizez, size_t index) {

      if (index >= N)
        return 0;

      size_t acc = 0;

      for (size_t i = 0; i < index; ++i)
        acc += sizez[i];

      return acc;

    }

    inline size_t
      offset_at (const Vec<size_t>& sizez, size_t index) {

      if (index >= sizez.size ())
        return 0;

      size_t acc = 0;

      for (size_t i = 0; i < index; ++i)
        acc += sizez[i];

      return acc;
    }


  }} // rokz::ut

#endif
