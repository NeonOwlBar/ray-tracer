#pragma once

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray
{
public:
	// Default constructor
	ray() {}

	// Constructor which assigns the origin and direction of the ray
	ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}


	// NOTE: An immutable reference is just a reference that cannot be changed 
	//		(due to the below functions returning a const reference).
	//		This helps to ensure that the origin and direction of this vector cannot be changed,
	//		but if the program wants a version that they *can* change, they simply need to do:
	//			ray r();
	//			point3 point_copy = r.origin();	// makes a copy
	//			point_copy.x() = 10;	// modify the copy

	// Returns an immutable reference to the origin of this ray
	const point3& origin() const { return orig; }
	// Returns an immutable reference to the direction vector of this ray
	const vec3& direction() const { return dir; }

	// Returns a copy of the point this vector reaches at the provided t.
	point3 at(double t) const
	{
		return orig + (t * dir);
	}

private:
	
	// Imagine a ray as a function (as in f(x) type of function, not code function).
	// A ray can be represented as P(t) = A + tb.
	// P is a 3D position along a line in 3D, A is the ray origin, b is the ray direction.
	// The ray parameter t is a real number (double in code).
	// Plug in a different t and P(t) moves the point along the ray.
	// If t = 1 (seconds, for example), you could think that the point that the ray has reached at time t is
	//	origin + (1 * dir). 
	// Origin is a point, it moves in the direction of a 3D vector, and moves by the length of that vector each second.
	// In 2 seconds, it has moved 2 lengths of the direction vector away from the origin.

	point3 orig;
	vec3 dir;
};

#endif
