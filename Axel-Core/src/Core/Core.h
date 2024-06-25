#pragma once

#define AX_ASSERT(x, ...) { if(!(x)) { AX_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }