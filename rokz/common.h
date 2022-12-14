

#ifndef ROKZ_COMMON_INCLUDE
#define ROKZ_COMMON_INCLUDE


#define VK_USE_PLATFORM_XCB_KHR      // we r on X 
#define GLFW_INCLUDE_VULKAN          // <-- GLFW_INCLUDE_VULKAN will do vulkan/vulkan.h for us
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_X11       // native X 
#include <GLFW/glfw3native.h>        // 

#define GLM_FORCE_RADIANS            // value always mean radians (not degrees)
#define GLM_FORCE_DEPTH_ZERO_TO_ONE  // Vulkan depth is [0,1] 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vulkan.hpp>  // not yet
// vulkan memo allocator


// !!! In exactly one CPP file define following macro before this
// include. It enables also internal definitions.
//#define VMA_IMPLEMENTATION
#define ROKZ_USE_VMA_ALLOCATOR 1

#include "vk_mem_alloc.h"       // mem alloc sucks



// To initialize the allocator with support for higher Vulkan version,
// you need to set member VmaAllocatorCreateInfo::vulkanApiVersion
// to an appropriate value, e.g. using constants like VK_API_VERSION_1_2.
// See code sample below.



#include <ctime>
#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <cmath>
#include <memory>
#include <map>
#include <set>
#include <array>

#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <tuple>
#include <functional>
#include <optional>
#include <random>

#include <chrono>
#include <thread>
#include <iostream>
#include <filesystem>



#endif
