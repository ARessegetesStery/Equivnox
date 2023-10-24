#pragma once

#include "Core/Type/EQXType.h"
#include "Core/CoreDefines.h"

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
