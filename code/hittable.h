#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

class material;

class hit_record
{
public:
    // Point of intersection
    point3 p;
    // Normal vector of the surface at point p
    vec3 normal;
    // Information about the material hit by the ray
    std::shared_ptr<material> mat;
    // t value at intersection closest to camera
    double t;
    // Whether the front of a face is outside (true) or inside (false) the hittable
    bool front_face;

    // Sets the hit record normal vector.
    // NOTE: the parameter 'outward_normal' is assumed to have UNIT length
    void set_face_normal(const ray& r, const vec3& outward_normal)
    {
        // Ray and face normal pointing OPPOSITE directions? Then dot < 0 (ray is OUTSIDE)
        front_face = dot(r.direction(), outward_normal) < 0;
        // Normal is outward normal if front_face is facing outward, otherwise invert it.
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    // Default destructor is generated automatically (doesn't use a user-provided
    // destructor). Typically this would allow the class to still be trivial, 
    // however, the presence of a virtual function prevents this.
    virtual ~hittable() = default;

    // Pure virtual function (= 0) - a definition MUST be provided in a derived class.
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif