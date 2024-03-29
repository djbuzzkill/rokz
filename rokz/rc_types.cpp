
#include "rc_types.hpp"
#include "allocation.hpp"
#include "buffer.hpp"
#include "image.hpp"
#include "sampler.hpp"

using namespace rokz;
// -----------------------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------------------
rc::Sampler::~Sampler () {

  if (handle) cx::Destroy (handle, device.handle);
  
}

// -----------------------------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------------------------

rc::Sampler::Ref rc::CreateSampler_default (const Device& device) {

  VkSamplerCreateInfo ci {};

  cx::CreateInfo (ci, device.physical.properties); 

  return CreateSampler (ci, device); 
}

rc::Sampler::Ref rc::CreateSampler (const VkSamplerCreateInfo ci, const Device& device) {

  Sampler::Ref obj = std::make_shared<rc::Sampler> (device);

  if (! cx::CreateSampler (obj->handle, ci, device.handle))  {
    printf ("FAILED make sampler");
    return rc::Sampler::Ref  (nullptr);
  }

  return obj;
}
