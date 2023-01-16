
// 
#include "rokz.h"

#include "common.h"
#include "utility.h"
#include <GLFW/glfw3.h>


// -------------------------------------------------------------+
// + Vulkan Memory Allocator + 
// 
// In exactly one CPP file define following macro before this include.
// It enables also internal definitions.
//
#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"
// -------------------------------------------------------------+


const char* rokz::kVersionstr = "0.0.1"; 
