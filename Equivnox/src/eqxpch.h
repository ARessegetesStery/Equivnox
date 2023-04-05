#pragma once

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

#define EQX_OUT

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

using std::cout, std::endl;

#include "Math/MathHeader.h"
#include "Util/Log/Log.h"

#ifdef EQX_DEBUG
 /*  Macros for Debug Msg Printing Control  */
#define EQX_PRINT_STATUS
#define EQX_PRINT_TRIG_CLIPPING

#define _CRT_SECURE_NO_WARNINGS

#endif
