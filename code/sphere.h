#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

// NOTE: SPHERE-RELATED MATHS. Explained in Ray Tracing in One Weekend here:
//      https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere/ray-sphereintersection
// TLDR:
// * The equation of a sphere at the origin (x^2 + y^2 + z^2 = r^2) can be used 
//      to determine if a point is inside (< r^2), on (= r^2), or outside (> r^2) a sphere.
// * Applying this to an arbitrary point (Cx, Cy, Cz) allows that equation to be
//      rewritten as: (Cx - x)^2 + (Cy - y)^2 + (Cz - z)^2 = r^2
// * The dot product definition allows this to be written in vector form:
//      (C - P) . (C - P) = r^2
// * Representing a ray as a function P(t) = Q + td (see ray class) allows this adjustment,
//   which is satisfied at some t value, as long as the ray hits the sphere:
//      (C - P(t)) . (C - P(t)) = r^2
// * Replace P(t) with Q + td to begin solving for t:
//      (C - (Q+td)) . (C - (Q+td)) = r^2
// * Expand this equation out to a quadratic equation (ax^2 + bx + c = 0):
//      (d.d)t^2 -2td.(C-Q) + (C-Q).(C-Q) - r^2 = 0
// * Take the discriminant (b^2 - 4ac) from the quadratic formula (x = -b +/- sqrt(b^2 - 4ac) / 2a)
//   to find how many real solutions the equation has. 
// NOTE: a vector dotted with itself is equal to the squared length of that vector
//      a = d.d = d.length_squared()
//      b = -2d.(C-Q)
//      c = (C-Q).(C-Q) - r^2 = (C-Q).length_squared() - r^2
//      (reminder: d = direction, C is the sphere centre, Q is the origin)
//    * If the discriminant (b^2 - 4ac) is:
//        - positive  -> two real solutions (goes through the sphere)
//        - zero      -> one real solution (is a tangent to the sphere)
//        - negative  -> no real solutions (misses the sphere entirely)
// * HOWEVER, b having a factor of -2 allows for a simplification. 
//   (Explanation: https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects/simplifyingtheray-sphereintersectioncode)
//      Replacing b with -2h in the quadratic formula results in:
//      x = (h +/- sqrt(h^2 - ac)) / a
// * Using the previous b coefficient, solving for h results in:
//      h = d.(C-Q)
// 
// Visualisation (as seen in the Ray Tracing in One Weekend, Chapter 5.1): 
//      https://raytracing.github.io/images/fig-1.05-ray-sphere.jpg

class sphere : public hittable
{
public:
    // Constructor creates a sphere at point 'center' with radius 'radius'
    // (or zero, if radius < 0) and assigns values to members
    // NOTE: Using std::fmax() which returns a FLOAT
    sphere(const point3& center, double radius) : center(center), radius(std::fmax(0, radius)) {}

    /// <summary>
    /// Determines if ray hit the sphere. If so, stores hit data in rec
    /// </summary>
    /// <param name="r">- Ray to be checked</param>
    /// <param name="ray_tmin">- Minimum t value allowed</param>
    /// <param name="ray_tmax">- Maximum t value allowed</param>
    /// <param name="rec">- Stores hit data (point of intersection, normal vector ,and t value at point)</param>
    /// <returns>True if ray hit, false if not</returns>
    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override
    {
        // Vector from origin to centre of sphere
        vec3 oc = center - r.origin();

        // See NOTE above this class to understand why quadratic formula is being used.
        // a, b, and c coefficients as part of ax^2 + bx + c = 0
        // Any vector dotted against itself = vector's length squared
        auto a = r.direction().length_squared();
        // b with factor of -2 removed (replaced b with -2h in quadratic formula)
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;
        // Part of quadratic formula (b^2 - 4ac), adjusted due to h replacing b
        auto discriminant = h * h - a * c;
        // Ray missed the hittable in this case as the quadratic equation had no solutions
        if (discriminant < 0) return false;

        auto sqrt_discriminant = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrt_discriminant) / a;    // Newly simplified quadratic formula.
        // Check if lowest value of t is out of range
        if (root <= ray_tmin || ray_tmax <= root)
        {
            // If out of range, check the other value of t as well
            root = (h + sqrt_discriminant) / a;
            // If this value is also out of range, exit
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }

        // If reached this point, the value assigned to root is the nearest value of t 
        // within the allowed range.

        // Save values in passed hit_record object
        // t value at ray-hittable intersection
        rec.t = root;
        // Point at which ray hits
        rec.p = r.at(rec.t);
        // Outward-facing normal vector at point
        vec3 outward_normal = (rec.p - center) / radius;
        // Determine whether face normal is pointing outwards or inwards
        rec.set_face_normal(r, outward_normal);

        // Because ray hit
        return true;
    }

private:
    point3 center;
    double radius;
};

#endif