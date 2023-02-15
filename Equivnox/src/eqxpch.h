#pragma once

#include <iostream>
#include <fstream>

#include <cstring>
#include <ctime>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <memory>

#include <string>
#include <array>
#include <vector>
#include <set>
#include <list>
#include <iterator>

#include "Math/MathHeader.h"

/**
 * Convention:
 * * Float instead of Double is used.
 * * The pixels of an outcome image is arranged as:
 *    (0, height - 1) ---------------------  (width, height)
 *			 |                                      |
 *			 |                                      |
 *			 |                                      |
 *			 |                                      |
 *		  (0, 0) ---------------------------- (width - 1, 0)
 */

#ifdef EQX_DEBUG
/*  Macros for Debug Msg Printing COntrol  */
#define EQX_PRINT_TRIG_CLIPPING

using std::cout, std::endl;
#define _CRT_SECURE_NO_WARNINGS
#endif

#define EQX_OUT


