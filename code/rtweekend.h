#pragma once

#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory> // for smart pointers

//// C++ Std Usings
//using std::make_shared;
//using std::shared_ptr;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180.0;
}

// NOTE: In interval notation, different bracket symbols indicate different end point descriptions
//	* a square bracket [ is used when the set includes the endpoint
//  * a parenthesis ( is used when the endpoint is either not included or the interval is unbounded

// Returns a random real in [0, 1).
inline double random_double()
{
	// Gets a random number between 0 and RAND_MAX (HEX: 0x7fff, DEC: 32767)
	// Scales this number to between 0 and 1
	return std::rand() / (RAND_MAX + 1.0);
}

// Returns a random real in [min, max).
inline double random_double(double min, double max)
{
	// Offsets a random number in [0, 1) by the minimum value,
	// and scales the value between the min and max values
	return min + (max - min) * random_double();
}
// Common Headers
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif
