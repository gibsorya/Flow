#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdexcept>
#include <cstdlib>
#include <cstdint>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <memory>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <chrono>

#define global static
#define local static

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t    i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

#include "error_macros.hpp"
#include "error.hpp"
#include <debug_tools.hpp>