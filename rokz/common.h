

#ifndef ROKZ_COMMON_INCLUDE
#define ROKZ_COMMON_INCLUDE


//#include <vulkan/vulkan.h> <--  GLFW_INCLUDE_VULKAN will do it for us
#define VK_USE_PLATFORM_XCB_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan_core.h>


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
