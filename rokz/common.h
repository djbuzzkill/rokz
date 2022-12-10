

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

//#include <vulkan/vulkan.hpp>  // not yet
// vulkan memo allocator
#include "vk_mem_alloc.h"       // mem alloc sucks

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
